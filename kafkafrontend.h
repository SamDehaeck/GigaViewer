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
    void makePublisher(QString topic,QString host = QString("localhost:9092"));
    void makeConsumers(QString cfgtopic,QString cmdtopic,QString groupId,QString host = QString("localhost:9092"));

signals:
    void startRecording(bool start,QString recfold,QString codec,int skipping);
    void implementNewFps(double fps);
    void setShutter(int time);
    void setRoiRows(int rows);
    void setRoiCols(int cols);

    void startTheTimer(int interval);
    void stopTheTimer();

public slots:
    void newImageReceived(ImagePacket theMatrix);
    void timedConsumption();
    void willStartTheTimer(int interval);
    void willStopTheTimer();

private:
    void publishMsg(QString key, QJsonDocument value);    
    QList<QJsonDocument> consumeMsg(rd_kafka_t* handle,int timeout,int maxMessages=100);
    void actOnCommands(QList<QJsonDocument> commands);
    void actOnConfig(QList<QJsonDocument> configs);
    void changeParameters(QJsonObject instructs);
    void changeRecording(bool start,QJsonObject parameters);
    rd_kafka_t* makeConsumerHandle(QString topic,QString groupId,QString host,QString earliestLatest,bool randomId);

    // producer vars
    rd_kafka_t *prodHndl;
    rd_kafka_topic_t *prodTopicHndl;
    // consumer vars
    rd_kafka_t *consCmdHndle;
    rd_kafka_t *consCfgHndle;
    QTimer timer;
    int timeout;
    // camera parameters
    bool isRecording;
};

#endif // KAFKAFRONTEND_H
