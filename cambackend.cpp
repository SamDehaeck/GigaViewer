#include "cambackend.h"
#include <QDebug>

CamBackend::CamBackend(QObject *parent) :
    QThread(parent),timerInterval(20)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
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
    if (camera.isOpened()) {
        timer.setInterval(timerInterval);
        timer.start();
        exec(); //will go beyond this point when quit() is send from within this thread
        timer.stop();
    } else {
        qDebug()<<"Could not open camera";
    }
}

void CamBackend::GrabFrame()
{
    if (!camera.isOpened()) quit();
    if (liveMode) {
        camera >> currImage.image;
        if (currImage.image.rows==0) {
            StopAcquisition();
        }
        emit NewImageReady(currImage);
    }
}

bool CamBackend::StartAcquisition(QString dev)
{
//    camera = new cv::VideoCapture("/home/sam/ULB/Movies/VapourCloudDynamics.mp4");
    if (dev=="0") {
        camera.open(0);
    } else {
        camera.open(dev.toStdString());
    }
    if (camera.isOpened()) {
        liveMode=TRUE;
        return TRUE;
    } else {
        liveMode=FALSE;
        return FALSE;
    }
    return FALSE;
}

void CamBackend::StopAcquisition()
{
    quit();
    liveMode=FALSE;
}

void CamBackend::ReleaseCamera()
{
    if (camera.isOpened()) camera.release();
}

bool CamBackend::Init()
{
    //No real need to init opencv separately
    return TRUE;
}
