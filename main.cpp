#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include "mainwindow.h"
#include "maingui.h"
#include "coordinator.h"
#include "imagepacket.h"

#ifdef KAFKA
#include "kafkafrontend.h"
#endif


int main(int argc, char *argv[])
{
    Coordinator theBoss;

    bool showGui(true);
    bool showGuiControls(true);
    bool remoteLog(false);
    bool remoteControls(false);

#ifdef KAFKA
    QString settingFile("");
    QString logTopic("");
    QString cmdTopic("");
    QString cfgTopic("");
    QString groupid("");
    QString driver("0");
    QString devicenumber("-1");
    QString serial("-1");
    QString pixelclock("-1");
    bool autostartCam(false);
    QString host("localhost:9092");
    QString topicBasename("");

    if (argc==2) {
        QString theFile=QString::fromUtf8(argv[1]);
        if (QFile::exists(theFile)) {
            if (theFile.contains(".json")) {
                // given a json file => should read configuration data
                //qInfo("Found a json file; should interpret it");
                QFile file(theFile);
                file.open(QIODevice::ReadOnly|QIODevice::Text);
                QString val=file.readAll();
                file.close();
                QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
                if (d.isEmpty()) {
                    qInfo("Problem parsing launch json file");
                }
                QJsonObject oo =d.object();
                QStringList kk=oo.keys();
                //qInfo("Got here: %s",val.toUtf8().data());
                if (kk.contains("kafka")) {
                    QJsonObject kafkaSettings=oo["kafka"].toObject();

                    // Now read the settings-file
                    settingFile=kafkaSettings["settingsfile"].toString();
                    qInfo("Found the file: %s",settingFile.toLatin1().data());
                    if (QFile::exists(settingFile)) {
                        if (settingFile.contains(".json")) {
                            // given a json file => should read configuration data
                            QFile fileSett(settingFile);
                            fileSett.open(QIODevice::ReadOnly|QIODevice::Text);
                            QString valSett=fileSett.readAll();
                            fileSett.close();
                            QJsonDocument dSett = QJsonDocument::fromJson(valSett.toUtf8());
                            if (dSett.isEmpty()) {
                                qInfo("Problem parsing kafka-settings json file");
                            }
                            QJsonObject ooSett =dSett.object();
                            QStringList kkSett=ooSett.keys();
                            //foreach (QString kitem, kkSett) {
                            //    qInfo("Found the key %s",kitem.toLatin1().data());
                            //}
                            //qInfo("Will try to read the settings file");

                            if (kkSett.contains("connectParams")) {
                                host=ooSett["connectParams"].toObject()["bootstrap.servers"].toString();
                                qInfo("Found the host %s",host.toLatin1().data());
                            }
                            if (kkSett.contains("topicBasename")) {
                                topicBasename=ooSett["topicBasename"].toString() + QString(".");
                                qInfo("Found a topicBasename %s",topicBasename.toLatin1().data());
                            }
                        }
                    }

                    // these topics do not yet contain the topicBasename
                    logTopic=topicBasename+kafkaSettings["logtopic"].toString();
                    cmdTopic=topicBasename+kafkaSettings["cmdtopic"].toString();
                    cfgTopic=topicBasename+kafkaSettings["cfgtopic"].toString();
                    groupid=kafkaSettings["groupid"].toString();


                }
                if (kk.contains("interface")) {
                    QJsonObject controls=oo["interface"].toObject();
                    showGui=controls["showgui"].toBool(true);
                    showGuiControls=controls["showguicontrols"].toBool(true);
                    remoteLog=controls["remotelog"].toBool(false);
                    remoteControls=controls["remotecontrols"].toBool(false);
                    qInfo("Found settings: %i, %i, %i, %i",showGui,showGuiControls,remoteLog,remoteControls);
                }
                if (kk.contains("camera")) {
                    QJsonObject camera=oo["camera"].toObject();
                    driver=camera["driver"].toString();
                    devicenumber=camera["devicenumber"].toString("-1");
                    serial=camera["cameraserial"].toString("-1");
                    pixelclock=camera["pixelclock"].toString("-1");

                    // now combine all these parameters into driver
                    driver=driver+QString("@")+devicenumber+QString("@")+serial+QString("@")+pixelclock;
                    qInfo()<<"The full driver name will be "<<driver;

                    autostartCam=camera["autostart"].toBool(false);
                }
            }
        }
    }

    if (not showGui) {
        QCoreApplication a(argc,argv);
        qRegisterMetaType<ImagePacket>("ImagePacket");
        KafkaFrontend kafka(&theBoss);
        if (remoteLog) {
            qInfo("Doing the remote log");
            kafka.makePublisher(logTopic);
        }
        if (remoteControls) {
            qInfo("Doing the remote control");
            kafka.makeConsumers(cfgTopic,cmdTopic,groupid);
        }

        if (autostartCam) {
            //theBoss.controlCameraThread(true);
            emit theBoss.LoadNewMovie(driver);
        }
        return a.exec();
    }
#endif

    // fall through to original gui if second argument is not kafka
    QApplication a(argc, argv);
    qRegisterMetaType<ImagePacket>("ImagePacket");
    MainWindow win;
    MainGui view(&theBoss);
    win.setCentralWidget(&view);


    QShortcut *fullToggle = new QShortcut(QKeySequence(Qt::Key_F),&win);
    QShortcut *closeEv = new QShortcut(QKeySequence(Qt::Key_Escape),&win);

    QObject::connect(&win,SIGNAL(windowClosed()),&theBoss,SLOT(stopAcquisition()));
    QObject::connect(&view,SIGNAL(closeApplic()),&win,SLOT(close()));
    QObject::connect(fullToggle,SIGNAL(activated()),&win,SLOT(toggleFullscreen()));
    QObject::connect(closeEv,SIGNAL(activated()),&win,SLOT(close()));

    win.showMaximized();

    if (argc==2) {
        QString theFile=QString::fromUtf8(argv[1]);
        if (not theFile.contains(".json")) {
                view.newMoviePressed(theFile);
        }
    }

#ifdef KAFKA
    // placed here, gui and logging or gui and controls can be activated!
    KafkaFrontend kafka(&theBoss); // cannot be placed inside the if-statement or else not responding to events! => will always exist if Kafka enabled..
    if (remoteLog or remoteControls) {
        //KafkaFrontend kafka(&theBoss);
        if (remoteLog) {
            kafka.makePublisher(logTopic,host);
            qInfo("Making the remote log %s",logTopic.toLatin1().data());
        }
        if (remoteControls) {
            kafka.makeConsumers(cfgTopic,cmdTopic,groupid,host);
            qInfo("Doing the remote control %s",cmdTopic.toLatin1().data());
        }
    }
    if (autostartCam) {
        //theBoss.controlCameraThread(true);
        view.newMoviePressed(driver);
    }

    if (not showGuiControls) {
        view.showInputControls(false);
        view.showCameraControls(false);
        view.showPlaybackControls(false);
    }
#endif

    return a.exec();
}


