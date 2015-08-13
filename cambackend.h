#ifndef CAMBACKEND_H
#define CAMBACKEND_H

#include <QObject>
#include <QtGui>
#include "imagepacket.h"
#include "imagesourcesink.h"

class CamBackend : public QThread
{
    Q_OBJECT
public:
    explicit CamBackend(QObject *parent = 0);
    bool StartAcquisition(QString dev="0");
    void StopAcquisition();
    void ReleaseCamera();
    void SetInterval(int newInt);
    void SetShutter(int shut);
    void SetAutoShutter(bool fitRange);

signals:
    void NewImageReady(ImagePacket im);
    void shutterChanged(int newTime);
    void startTheTimer(int interval);
    void stopTheTimer();

public slots:
    void GrabFrame();
    void StartRecording(bool start, QString recFold="", QString codec="");
    void skipForwardBackward(bool forward);
    void willStartTheTimer(int interval);
    void willStopTheTimer();
    void setRoiRows(int rows);
    void setRoiCols(int cols);

private:
    void run();
    void record();


    ImageSourceSink *currSink, *currSource;
    ImagePacket currImage;
    bool liveMode;
    bool recording;
    double timerInterval;
    QTimer timer;
    bool reversePlay;
    bool needTimer;
    bool doesCallBack;
    bool running;
    QString format;

};

#endif // CAMBACKEND_H
