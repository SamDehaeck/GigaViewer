#ifndef CAMBACKEND_H
#define CAMBACKEND_H

#include <QObject>
#include <QtGui>
#include "opencv2/opencv.hpp"
#include "imagepacket.h"

class CamBackend : public QThread
{
    Q_OBJECT
public:
    explicit CamBackend(QObject *parent = 0);
    bool Init();
    bool StartAcquisition(QString dev="0");
    void StopAcquisition();
    void ReleaseCamera();
    void SetInterval(int newInt);

    bool IsLive();

signals:
    void NewImageReady(ImagePacket im);

public slots:
    void GrabFrame();
    void StartRecording(bool start, QString recFold, QString codec);

private:
    void run();
    void record();

    cv::VideoCapture camera;
    cv::VideoWriter recFile;

    ImagePacket currImage;
    bool liveMode;
    bool recording;
    double timerInterval;
    QTimer timer;
    bool reversePlay;

};

#endif // CAMBACKEND_H
