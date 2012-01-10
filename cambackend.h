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
    bool StartAcquisition();

    bool IsLive();

signals:
    void NewImageReady(ImagePacket im);

public slots:
    void StopAcquisition();
    void GrabFrame();

private:
    void run();
    void ReleaseCamera();

    cv::VideoCapture* camera;
    ImagePacket currImage;
    bool liveMode;
    QTimer timer;

};

#endif // CAMBACKEND_H
