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

KafkaFrontend::KafkaFrontend(Coordinator *boss,QObject *parent) : QObject(parent),prodHndl(nullptr),consCmdHndle(nullptr),consCfgHndle(nullptr),timeout(100),isRecording(false) {
    connect(boss,SIGNAL(NewImageReady(ImagePacket)),this,SLOT(newImageReceived(ImagePacket)));
    connect(this,SIGNAL(startRecording(bool,QString,QString,int)),boss,SLOT(StartRecording(bool,QString,QString,int)));
    connect(this,SIGNAL(implementNewFps(double)),boss,SLOT(changeFps(double)));
    connect(this,SIGNAL(setShutter(int)),boss,SLOT(changeShutter(int)));
    connect(this,SIGNAL(setRoiRows(int)),boss,SLOT(setRoiRows(int)));
    connect(this,SIGNAL(setRoiCols(int)),boss,SLOT(setRoiCols(int)));

    connect(&timer, SIGNAL(timeout()), this, SLOT(timedConsumption()));
    connect(this,SIGNAL(startTheTimer(int)),this,SLOT(willStartTheTimer(int)));
    connect(this,SIGNAL(stopTheTimer()),this,SLOT(willStopTheTimer()));

}

// this slot is called for each new image that arrives
void KafkaFrontend::newImageReceived(ImagePacket theMatrix) {
    if (prodHndl!=nullptr) {
    //    qInfo("Was contacted for a new image!");
        QJsonDocument qjson=QJsonDocument::fromVariant(theMatrix.message);
    //    qInfo("Want to publish %s",qjson.toJson().data());
        publishMsg(QString("GigaViewer"),qjson);
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

// this is the slot called by the timer
void KafkaFrontend::timedConsumption() {
    //should first check if I am connected to a camera!
    //qInfo("Received a timeout");
    QList<QJsonDocument> newCommands=consumeMsg(consCmdHndle,timeout);
    actOnCommands(newCommands);
    QList<QJsonDocument> newConfigs=consumeMsg(consCfgHndle,timeout);
    actOnConfig(newConfigs);
}

// this will poll for new messages until the queue is empty and return a list of json's
// Other info that is discarded: key, offset, partition, topic
QList<QJsonDocument> KafkaFrontend::consumeMsg(rd_kafka_t* handle,int timeout,int maxMessages) {
    bool breakFromLoop=false;
    int receivedNumber=0;
    QList<QJsonDocument> messageList;
    while (not breakFromLoop) {
        rd_kafka_message_t *msg = rd_kafka_consumer_poll(handle, timeout);
        if (msg != nullptr) {
            if (msg->err == RD_KAFKA_RESP_ERR_NO_ERROR) {
                QByteArray topp=rd_kafka_topic_name(msg->rkt);
                QByteArray message=QByteArray(static_cast<const char*>(msg->payload));
                //int64_t offset=msg->offset;  // this parameter is interesting so as to see if you are lagging!
                QJsonDocument qjson=QJsonDocument::fromJson(message);
                if (qjson.isNull()){
                   qInfo("Problem parsing message from topic %s: %s",topp.data(),static_cast<const char*>(msg->payload));
                } else {
                    messageList.append(qjson);
                    receivedNumber++;
                }
            } else {
                qInfo("Some error message received in consumption");
                breakFromLoop=true; // not the same, but should probably break from loop
            }
            rd_kafka_message_destroy(msg);
            if (receivedNumber>maxMessages) {
                breakFromLoop=true;
            }
        } else {
            breakFromLoop=true;
        }
    }
    return messageList;
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

    double newDelay=1000.0/newconfig["fps"].toDouble();
    qInfo("Want to set delay to: %f",newDelay);
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

rd_kafka_t* KafkaFrontend::makeConsumerHandle(QString topic,QString groupId,QString host,QString earliestLatest,bool randomId) {
    rd_kafka_conf_t *consCfg;
    rd_kafka_topic_conf_t *consTopicCfg;
    char errstr[1000];
    rd_kafka_resp_err_t errCode;
    rd_kafka_t* myConsumer;

    consCfg = rd_kafka_conf_new();
    if(consCfg == nullptr) {
        qInfo("Failed to create consumer conf\n");
    }

    if (rd_kafka_conf_set(consCfg, "bootstrap.servers", host.toLocal8Bit(), errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        qInfo("rd_kafka_conf_set() failed with error: %s\n", errstr);
    }

    if (randomId) {
        //should modify the groupid by adding a random number!!
        //QString suffix= random(10);
        //int x = (QRandomGenerator::global()->bounded(99999));  // introduced in QT 5.10 and 5.9 on Ubuntu!
        qsrand(time(NULL));
        int x=qrand() % 1000000;
//        qInfo("Random number is: %d",x);
        groupId=(groupId.append("-%1")).arg(x);
//        qInfo(groupId.toLocal8Bit());
    }
    if(RD_KAFKA_CONF_OK != rd_kafka_conf_set(consCfg, "group.id", groupId.toLocal8Bit(),       // group id is set here!
                      errstr, sizeof(errstr))) {
        qInfo("rd_kafka_conf_set() failed with error: %s\n", errstr);
    }

    // now start with configuration for topic
    consTopicCfg = rd_kafka_topic_conf_new();
    if (RD_KAFKA_CONF_OK != rd_kafka_topic_conf_set(consTopicCfg, "auto.offset.reset", earliestLatest.toLocal8Bit()   // offset 'earliest' is set here!!
                                                    ,errstr, sizeof(errstr))) {
        qInfo("rd_kafka_topic_conf_set() failed with error: %s\n", errstr);
    }
    // use this topicconfiguration as the default for all topics I will subscribe to
    rd_kafka_conf_set_default_topic_conf(consCfg, consTopicCfg);

    // Now create the consumer handle
    myConsumer = rd_kafka_new(RD_KAFKA_CONSUMER, consCfg, errstr, sizeof(errstr));
    if(myConsumer == nullptr) {
        qInfo("Failed to create consumer:%s", errstr);
    }

    /* rd_kafka_poll_set_consumer() is used to redirect the main queue which is
    *  serviced using rd_kafka_poll() to the rd_kafka_consumer_poll(). With one api
    *  'rd_kafka_consumer_poll()' both callbacks and message are serviced.
    *  Once queue is forwarded using this API, it is not permitted to call
    *  rd_kafka_poll to service non message delivery callbacks.
    */
    rd_kafka_poll_set_consumer(myConsumer);

    /* Topic partition list (tp_list) is supplied as an input to the consumer
    *  subscribe(using rd_kafka_subscribe()). The api rd_kafka_subscribe() expects
    *  that the partition argument to be set to RD_KAFKA_PARTITION_UA and internally
    *  all partitions are assigned to the consumer.
    *  Note: partition balancing/assignment is done if more consumers are part
    *  of the same consumer group.
    */

    rd_kafka_topic_partition_list_t *tp_list = rd_kafka_topic_partition_list_new(0);
    rd_kafka_topic_partition_t* tpObj = rd_kafka_topic_partition_list_add(tp_list,
                                                topic.toLocal8Bit(), RD_KAFKA_PARTITION_UA);
    if (nullptr == tpObj) {
        qInfo("Could not add the topic partition to the list.\n");
    }

    errCode = rd_kafka_subscribe(myConsumer, tp_list);
    if (errCode  != RD_KAFKA_RESP_ERR_NO_ERROR) {
        qInfo("Topic partition subscription failed. ERROR: %d\n", errCode);
    }
    // now remove topiclist
    rd_kafka_topic_partition_list_destroy(tp_list);
    return myConsumer;
}

void KafkaFrontend::makeConsumers(QString cfgtopic,QString cmdtopic,QString groupId,QString host) {
    consCfgHndle=makeConsumerHandle(cfgtopic,groupId,host,"earliest",true);
    consCmdHndle=makeConsumerHandle(cmdtopic,groupId,host,"latest",true); // also a random groupid
    int timerInterval=100; // check once per second for new messages
    emit startTheTimer(timerInterval); // needs to be started in a slot for interthread communication purposes...
}

void KafkaFrontend::willStartTheTimer(int interval)
{
    timer.setInterval(interval);
    timer.start();
}

void KafkaFrontend::willStopTheTimer()
{
    timer.stop();
}
