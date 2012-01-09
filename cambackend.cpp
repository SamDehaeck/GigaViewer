#include "cambackend.h"
#include <QDebug>

CamBackend::CamBackend(QObject *parent) :
    QThread(parent)
{
}

bool CamBackend::IsLive() {
    if (liveMode) {
        if (camera.isOpened()) {
            return TRUE;
        }
    }
    return FALSE;
}

void CamBackend::run()
{
    qDebug() <<"Starting new thread";
    QTimer timer;
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
    timer.setInterval(20);
    timer.start();   // puts one event in the threads event queue
    exec();
    timer.stop();
    StopAcquisition();
}

void CamBackend::GrabFrame()
{
    if (!camera.isOpened()) StartAcquisition();
    if (liveMode) {

        camera >> currImage.image;
        emit NewImageReady(currImage);
    }
}

void CamBackend::StartAcquisition()
{
    camera=cv::VideoCapture(0);
    if (camera.isOpened()) {
        liveMode=TRUE;
    } else {
        liveMode=FALSE;
    }
}

void CamBackend::StopAcquisition()
{
    liveMode=FALSE;
//    camera.release();
}
