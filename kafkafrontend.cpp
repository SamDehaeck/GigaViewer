#include "kafkafrontend.h"
#include <QVariantMap>
//#include <QRandomGenerator>

/* msgDeliveryCB: Is the delivery callback.
 * The delivery report callback will be called once for each message
 * accepted by rd_kafka_produce() with err set to indicate
 * the result of the produce request. An application must call rd_kafka_poll()
 * at regular intervals to serve queued delivery report callbacks.
 */
static void msgDeliveryCB (rd_kafka_t *,
                           const rd_kafka_message_t *rkmessage, void *) {
    if (rkmessage->err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        qInfo("FAILURE: Message not delivered to partition.\n");
        qInfo("ERROR: %s", rd_kafka_err2str(rkmessage->err));
    } else {
        qInfo("Produced: %.*s\n",static_cast<int>(rkmessage->len), static_cast<const char*>(rkmessage->payload));
    }
}

KafkaFrontend::KafkaFrontend(Coordinator *boss,QObject *parent) : QObject(parent),prodHndl(nullptr),cmdPointer(nullptr),cfgPointer(nullptr),skipLogging(5),timeout(100),isRecording(false) {
    connect(boss,SIGNAL(NewImageReady(ImagePacket)),this,SLOT(newImageReceived(ImagePacket)));
    connect(this,SIGNAL(startRecording(bool,QString,QString,int)),boss,SLOT(StartRecording(bool,QString,QString,int)));
    connect(this,SIGNAL(implementNewFps(double)),boss,SLOT(changeFps(double)));
    connect(this,SIGNAL(setShutter(int)),boss,SLOT(changeShutter(int)));
    connect(this,SIGNAL(setRoiRows(int)),boss,SLOT(setRoiRows(int)));
    connect(this,SIGNAL(setRoiCols(int)),boss,SLOT(setRoiCols(int)));

    qRegisterMetaType< QList<QJsonDocument> >( "QList<QJsonDocument>" );

}

// this slot is called for each new image that arrives
void KafkaFrontend::newImageReceived(ImagePacket theMatrix) {
    if (prodHndl!=nullptr) {
    //    qInfo("Was contacted for a new image!");
        QJsonDocument qjson=QJsonDocument::fromVariant(theMatrix.message);
    //    qInfo("Want to publish %s",qjson.toJson().data());
        //qInfo("skipLogging=%i",skipLogging);
        if (theMatrix.seqNumber%skipLogging==0) {
            publishMsg(QString("GigaViewer"),qjson);
        }
    }
}

// This will publish a JSon to the connected topic
void KafkaFrontend::publishMsg(QString key, QJsonDocument value) {
    QByteArray msgQ=value.toJson();
    char* payload=msgQ.data();
    char* kkey=key.toLocal8Bit().data();
    if (rd_kafka_produce(prodTopicHndl,
                         RD_KAFKA_PARTITION_UA,
                         RD_KAFKA_MSG_F_COPY,
                         payload,
                         strlen(payload),
                         kkey,
                         strlen(kkey),
                         nullptr) == -1) {
        int errNum = errno;
        qInfo("Failed to produce to topic : %s\n", rd_kafka_topic_name(prodTopicHndl));
        qInfo("Error Number: %d ERROR NAME: %s\n"
                   ,errNum, rd_kafka_err2str(rd_kafka_last_error()));
    }
}

// this will put the commands into effect - look at all commands
void KafkaFrontend::actOnCommands(QList<QJsonDocument> commands) {
    if (commands.size()!=0) {
        for (int i=0;i<commands.size();i++) {
            QJsonObject qObj=commands[i].object();
            QStringList kk=qObj.keys();
            if (kk.size()!=0) {
                if (qObj["command"]=="recstart") {
                    //qInfo("Should start recording");
                    QJsonObject newParams=qObj["params"].toObject();
                    if (not newParams.isEmpty()) {
                        if (not isRecording) changeRecording(true,newParams); // only start if not yet recording
                        isRecording=true;
                    } else {
                        qInfo("did not find params");
                    }
                } else if (qObj["command"]=="recstop") {
                    //qInfo("Should stop recording");
                    if (isRecording) changeRecording(false,QJsonObject());
                    isRecording=false;
                } else {
                    qInfo("Received unkown command: %s",commands[i].toJson().data());
                }
            }
        }
    }
    return;
}

// this will put the configuration into effect - only apply last one from list
void KafkaFrontend::actOnConfig(QList<QJsonDocument> configs) {
    if (configs.size()!=0) {
        QJsonObject qObj=configs[configs.size()-1].object(); //only apply last configuration
        QStringList kk=qObj.keys();
        if (kk.contains("exposure") and kk.contains("fps")) {
            changeParameters(qObj);
        } else {
            qInfo("Missing at least one config setting!");
        }
    }
    return;
}

// This will change the camera parameters
void KafkaFrontend::changeParameters(QJsonObject newconfig) {
    //qInfo("Should change some camera parameters");
    int sshut=newconfig["exposure"].toInt();
    if (sshut==0) {
        //probably a double was given!
        sshut=static_cast<int>(newconfig["exposure"].toDouble());
    }
    qInfo("Want to set shutter to: %i",sshut);
    emit setShutter(sshut);

    //double fps=newconfig["fps"].toDouble();
    //if (fps>2) {
    //    skipLogging=static_cast<int>(floor(fps/2)); // half of the framerate should lead to 2 kafka-logs per second..
    //} else {
    //    skipLogging=1;
    //}
    //skipLogging=1;

    double newDelay=1000.0/newconfig["fps"].toDouble();

    int emitSkipped=static_cast<int>(floor(25.0/newDelay)+1); // recalculate this value
    if (newDelay>100) {
        skipLogging=emitSkipped;
    } else {
        skipLogging=emitSkipped*10;
    }
    qInfo("Found delay %f and emitSkipped %i setting skipLogging to %i",newDelay,emitSkipped,skipLogging);

    //qInfo("Want to set delay to: %f",newDelay);
    emit implementNewFps(newDelay);
    /*int ccols=newconfig["roicols"].toInt();
    emit setRoiCols(ccols);
    int rrows=newconfig["roirows"].toInt();
    emit setRoiRows(rrows);*/
}

// this will start/stop recording
void KafkaFrontend::changeRecording(bool start,QJsonObject parameters) {
//    qInfo("Should change some recording parameters");
//    QJsonObject commParams=instructs["command_params"].toObject();
    if (start) {
//        qInfo("Will start recording");
        QString recdirname= parameters["dirname"].toString();
        QDir basedir(recdirname);
        if (not basedir.exists()) recdirname="";
        emit startRecording(true, recdirname,parameters["codec"].toString(),parameters["recskip"].toInt());
    } else {
//        qInfo("Will stop recording");
        emit startRecording(false, "","",0);
//        isRecording=false;
//    } else {
//        qInfo("Didn't understand the recording sub-command");
    }
}



// this will make the publisher and connect to the publishing topic
void KafkaFrontend::makePublisher(QString topic,QString host) {
    // something to collect the error messages
    char errstr[1000];

    // producer configuration object
    rd_kafka_conf_t *prodCfg;

    prodCfg = rd_kafka_conf_new();
    if (prodCfg == nullptr) {
        qInfo("Failed to create conf\n");
    }

    if (rd_kafka_conf_set(prodCfg, "bootstrap.servers", host.toLocal8Bit(), errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        qInfo("rd_kafka_conf_set() failed with error: %s\n", errstr);
    }

    // set the callback for missed messages
    rd_kafka_conf_set_dr_msg_cb(prodCfg, msgDeliveryCB);

    // New that the configuration is ready, make the PRODUCER
    prodHndl = rd_kafka_new(RD_KAFKA_PRODUCER, prodCfg, errstr, sizeof(errstr));
    if (prodHndl == nullptr) {
        qInfo("Failed to create producer: %s\n", errstr);
    }

    // now do the same for the topic, first make configuration
    rd_kafka_topic_conf_t *prodTopicCfg;
    prodTopicCfg = rd_kafka_topic_conf_new();
    if (prodTopicCfg == nullptr) {
        qInfo("Failed to create new topic conf\n");
    }

    prodTopicHndl = rd_kafka_topic_new(prodHndl, topic.toLocal8Bit(), prodTopicCfg);
    if (prodTopicHndl == nullptr) {
        qInfo("Failed to create new topic handle\n");
    }
    prodTopicCfg = nullptr; /* Now owned by topic */
}

void KafkaFrontend::makeConsumers(QString cfgtopic,QString cmdtopic,QString groupId,QString host) {
    cfgPointer=new KafkaController(cfgtopic,groupId,host,"earliest",true,timeout);
    cfgThread=new QThread();
    cfgThread->start();
    cfgPointer->moveToThread(cfgThread);

    connect(this,SIGNAL(startTheTimer(int)),cfgPointer,SLOT(willStartTheTimer(int)));
    connect(this,SIGNAL(stopTheTimer()),cfgPointer,SLOT(willStopTheTimer()));
    connect(cfgPointer,SIGNAL(sendNewData(QList<QJsonDocument>)),this,SLOT(actOnConfig(QList<QJsonDocument>)));

    cmdPointer=new KafkaController(cmdtopic,groupId,host,"latest",true,timeout);
    cmdThread=new QThread();
    cmdThread->start();
    cmdPointer->moveToThread(cmdThread);

    connect(this,SIGNAL(startTheTimer(int)),cmdPointer,SLOT(willStartTheTimer(int)));
    connect(this,SIGNAL(stopTheTimer()),cmdPointer,SLOT(willStopTheTimer()));
    connect(cmdPointer,SIGNAL(sendNewData(QList<QJsonDocument>)),this,SLOT(actOnCommands(QList<QJsonDocument>)));

    emit startTheTimer(100); // both kafkacontrollers are listening to this signal to start their timers.
}
