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
    void ReleaseCamera();

    QTimer timer;
    bool IsLive();

signals:
    void NewImageReady(ImagePacket im);

public slots:
    void StopAcquisition();
    void GrabFrame();

private:
    void run();

    cv::VideoCapture camera;
    ImagePacket currImage;
    bool liveMode;
    double timerInterval;

};

#endif // CAMBACKEND_H
