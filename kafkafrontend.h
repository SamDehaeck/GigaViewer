#ifndef KAFKAFRONTEND_H
#define KAFKAFRONTEND_H

#include <QObject>
#include <QJsonDocument>

#include "imagepacket.h"
#include "coordinator.h"
#include "librdkafka/rdkafka.h"


//static void msgDeliveryCB (rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque);

class KafkaFrontend : public QObject
{
    Q_OBJECT
public:
    explicit KafkaFrontend(Coordinator *boss,QObject *parent = nullptr);


signals:

public slots:
    void newImageReceived(ImagePacket theMatrix);

private:
    void publishMsg(QString key, QJsonDocument value);

    rd_kafka_t *prodHndl;
    rd_kafka_topic_t *prodTopicHndl;
};

#endif // KAFKAFRONTEND_H
