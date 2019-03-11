#include "kafkafrontend.h"
#include <QVariantMap>

/* msgDeliveryCB: Is the delivery callback.
 * The delivery report callback will be called once for each message
 * accepted by rd_kafka_produce() with err set to indicate
 * the result of the produce request. An application must call rd_kafka_poll()
 * at regular intervals to serve queued delivery report callbacks.
 */
static void msgDeliveryCB (rd_kafka_t *rk,
                           const rd_kafka_message_t *rkmessage, void *opaque) {
    if (rkmessage->err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        qInfo("FAILURE: Message not delivered to partition.\n");
        qInfo("ERROR: %s", rd_kafka_err2str(rkmessage->err));
    } else {
        qInfo("Produced: %.*s\n",(int)rkmessage->len, (const char*)rkmessage->payload);
    }
}

KafkaFrontend::KafkaFrontend(Coordinator *boss,QObject *parent) : QObject(parent)
{
    connect(boss,SIGNAL(NewImageReady(ImagePacket)),this,SLOT(newImageReceived(ImagePacket)));

    QString bootstrap=QString("localhost:9092");
    QString topic=QString("GigaViewerTopic");

    // something to collect the error messages
    char errstr[1000];

    // producer handle and configuration object
//    rd_kafka_t *prodHndle;
    rd_kafka_conf_t *prodCfg;

    prodCfg = rd_kafka_conf_new();
    if (prodCfg == nullptr) {
        qInfo("Failed to create conf\n");
    }

    if (rd_kafka_conf_set(prodCfg, "bootstrap.servers", bootstrap.toLocal8Bit(), errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
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

// this slot is called for each new image that arrives
void KafkaFrontend::newImageReceived(ImagePacket theMatrix) {
    //qInfo("Received a new image: %d",theMatrix.seqNumber);
    QVariantMap message;
    message.insert("@timestamp", theMatrix.timeStamp);
    message.insert("SeqNumber", theMatrix.seqNumber);
    message.insert("FileName",QString::fromStdString(theMatrix.fileName));
    message.insert("Recording",1);

    QJsonDocument qjson=QJsonDocument::fromVariant(message);
    publishMsg(QString("GigaViewer"),qjson);
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
