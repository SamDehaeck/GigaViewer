#ifndef CAMBACKEND_H
#define CAMBACKEND_H

#include <QObject>
#include "opencv2/opencv.hpp"

class CamBackend : public QObject
{
    Q_OBJECT
public:
    explicit CamBackend(QObject *parent = 0);
    bool IsLive();

signals:
    void NewImageReady(cv::Mat im);

public slots:
    void StartAcquisition(QString camNumber);
    void StopAcquisition();

private:
    cv::VideoCapture camera;
    cv::Mat currImage;
    bool liveMode;

};

#endif // CAMBACKEND_H
