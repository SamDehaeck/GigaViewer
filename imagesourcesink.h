#ifndef IMAGESOURCESINK_H
#define IMAGESOURCESINK_H

#include "imagepacket.h"
#include "QtGui"

class ImageSourceSink
{
public:

    virtual bool Init();
    virtual bool StartAcquisition(QString dev="0");
    virtual bool StopAcquisition();
    virtual bool ReleaseCamera();
    virtual bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    virtual bool RecordFrame(ImagePacket& source);
    virtual bool StartRecording(QString recFold, QString codec,int fps, int cols, int rows);
    virtual bool StopRecording();
    virtual bool IsOpened();
    virtual bool SetInterval(int msec);
    virtual bool SetShutter(int shutTime);
    virtual int SetAutoShutter(bool fitRange);
};

#endif // IMAGESOURCESINK_H
