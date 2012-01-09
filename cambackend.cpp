#include "cambackend.h"

CamBackend::CamBackend(QObject *parent) :
    QObject(parent)
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

void CamBackend::StartAcquisition()
{
    if (!camera.isOpened()) {
        camera=cv::VideoCapture(0);
    }
    liveMode=TRUE;
    if (camera.isOpened()) {
        camera >> currImage;
        emit NewImageReady(currImage);
    }

}

void CamBackend::StopAcquisition()
{
    liveMode=FALSE;
}
