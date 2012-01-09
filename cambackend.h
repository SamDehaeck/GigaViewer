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
    void StopAcquisition();

    bool IsLive();

signals:
    void NewImageReady(ImagePacket im);

public slots:
    void StartAcquisition();
    void GrabFrame();

private:
    void run();

    cv::VideoCapture camera;
    ImagePacket currImage;
    bool liveMode;

};

#endif // CAMBACKEND_H
