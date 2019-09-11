#include "kafkacontroller.h"

KafkaController::KafkaController(QString topic,QString groupId,QString host,QString earliestLatest,bool randomId,int timeout,QObject *parent) : QObject(parent),topic(topic),timer(nullptr),timeout(timeout)
{
    consumerHdle=makeConsumerHandle(topic,groupId,host,earliestLatest,randomId);
    timer=new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(timedConsumption()));  // should not start timer before moveToThread is executed!!
}

rd_kafka_t* KafkaController::makeConsumerHandle(QString topic,QString groupId,QString host,QString earliestLatest,bool randomId) {
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
        //int x = (QRandomGenerator::global()->bounded(99999));  // introduced in QT 5.10 and only 5.9 available on Ubuntu!
        qsrand(time(nullptr));
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

// this is the slot called by the timer
void KafkaController::timedConsumption() {
    //should first check if I am connected to a camera!
    QList<QJsonDocument> newCommands=consumeMsg(consumerHdle,timeout);

    if (newCommands.count()>0) {
        //qInfo("Received %i message(s) on topic %s",newCommands.count(),qUtf8Printable(topic));
        emit sendNewData(newCommands);
    }
}

// this will poll for new messages until the queue is empty and return a list of json's
// Other info that is discarded: key, offset, partition, topic
QList<QJsonDocument> KafkaController::consumeMsg(rd_kafka_t* handle,int timeout,int maxMessages) {
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

void KafkaController::willStartTheTimer(int interval)
{
    timer->setInterval(interval);
    timer->start();
}

void KafkaController::willStopTheTimer()
{
    timer->stop();
}
