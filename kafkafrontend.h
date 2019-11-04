#ifndef KAFKAFRONTEND_H
#define KAFKAFRONTEND_H

#include <QObject>
#include <QJsonDocument>
#include <QThread>

#include "imagepacket.h"
#include "coordinator.h"
#include "librdkafka/rdkafka.h"
#include "kafkacontroller.h"

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
    void actOnCommands(QList<QJsonDocument> commands);
    void actOnConfig(QList<QJsonDocument> configs);

private:
    void publishMsg(QString key, QJsonDocument value);    
    void changeParameters(QJsonObject instructs);
    void changeRecording(bool start,QJsonObject parameters);

    // producer vars
    rd_kafka_t *prodHndl;
    rd_kafka_topic_t *prodTopicHndl;
    // consumer vars
    KafkaController *cmdPointer;
    KafkaController *cfgPointer;
    QThread* cmdThread;
    QThread* cfgThread;
    int skipLogging;

    int timeout;
    // camera parameters
    bool isRecording;
};

#endif // KAFKAFRONTEND_H
