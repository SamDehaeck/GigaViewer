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

signals:
    void NewImageReady(ImagePacket im);

public slots:
    void GrabFrame();
    void StartRecording(bool start, QString recFold, QString codec);

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

};

#endif // CAMBACKEND_H
