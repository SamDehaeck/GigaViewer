#include "cambackend.h"
#include <QDebug>

CamBackend::CamBackend(QObject *parent) :
    QThread(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
}

bool CamBackend::IsLive() {
    if (liveMode) {
        if (camera->isOpened()) {
            return TRUE;
        }
    }
    return FALSE;
}

void CamBackend::run()
{
    if (camera->isOpened()) {
        timer.setInterval(20);
        timer.start();
        exec(); //will go beyond this point when quit() is send from within this thread
        timer.stop();
    } else {
        qDebug()<<"Could not open camera";
    }
}

void CamBackend::GrabFrame()
{
    if (!camera->isOpened()) quit();
    if (liveMode) {
        *camera >> currImage.image;
        emit NewImageReady(currImage);
    }
}

bool CamBackend::StartAcquisition()
{
//    camera = new cv::VideoCapture("/home/sam/ULB/Movies/VapourCloudDynamics.mp4");
    camera = new cv::VideoCapture(0);
    if (camera->isOpened()) {
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
    delete camera;
}

bool CamBackend::Init()
{
    //No real need to init opencv separately
    return TRUE;
}
