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
#ifdef KAFKA
   if (argc>1) {
       QCoreApplication a(argc,argv);
       qRegisterMetaType<ImagePacket>("ImagePacket");
       QString subCommand=QString::fromUtf8(argv[1]);
       if (subCommand==QString("kafka")) {
           qInfo("Should start Kafka frontend!");
       }
       KafkaFrontend kafka(&theBoss);
       theBoss.controlCameraThread(true);

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
        if (QFile::exists(theFile)) {
            view.newMoviePressed(theFile);
        }
    }
    return a.exec();
}
