#include "cambackend.h"

CamBackend::CamBackend(QObject *parent) :
    QObject(parent),camera(0)
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

void CamBackend::StartAcquisition(QString camString)
{
    liveMode=TRUE;
    if (camera.isOpened()) {
        camera >> currImage;
    }
    emit NewImageReady(currImage);
}

void CamBackend::StopAcquisition()
{
    liveMode=FALSE;
}
