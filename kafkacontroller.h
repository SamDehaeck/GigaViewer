#ifndef KAFKACONTROLLER_H
#define KAFKACONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QJsonDocument>
#include "librdkafka/rdkafka.h"

class KafkaController : public QObject
{
    Q_OBJECT
public:
    explicit KafkaController(QString topic,QString groupId,QString host,QString earliestLatest,bool randomId,int timeout,QObject *parent = nullptr);

    rd_kafka_t* makeConsumerHandle(QString topic,QString groupId,QString host,QString earliestLatest,bool randomId);
    QList<QJsonDocument> consumeMsg(rd_kafka_t* handle,int timeout,int maxMessages=100);

signals:
    void sendNewData(QList<QJsonDocument> data);
    void testNewData(QString sometext);

public slots:
    void willStartTheTimer(int interval);
    void willStopTheTimer();
    void timedConsumption();

private:
    QString topic;
    QTimer* timer;
    rd_kafka_t* consumerHdle;
    int timeout; //this is the timeout value for asking for new messages.
};

#endif // KAFKACONTROLLER_H
