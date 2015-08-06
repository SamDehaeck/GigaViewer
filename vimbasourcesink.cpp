#include "vimbasourcesink.h"

bool VimbaSourceSink::IsOpened()
{
    return true;
}

bool VimbaSourceSink::Init()
{
    qDebug()<<"Vimba Init";
    return true;
}

bool VimbaSourceSink::StartAcquisition(QString dev)
{
    qDebug()<<"Vimba Start Acquisition";
    return true;
}

bool VimbaSourceSink::StopAcquisition()
{
    qDebug()<<"Vimba Stop Acquisition";
    return true;
}

bool VimbaSourceSink::ReleaseCamera()
{
    qDebug()<<"Vimba Release Camera";
    return true;
}

bool VimbaSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
//    qDebug()<<"VimbaSourceSink Grab Frame";
    return true;

}

bool VimbaSourceSink::SetInterval(int msec)
{
    qDebug()<<"Vimba Set Interval";
    return true;
}

bool VimbaSourceSink::SetShutter(int shutTime)
{
    qDebug()<<"Vimba Set Shutter";
    return true;
}

int VimbaSourceSink::SetAutoShutter(bool fitRange)
{
    qDebug()<<"Vimba set auto shutter";
    return true;
}

