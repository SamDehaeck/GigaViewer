#ifndef IMAGESOURCESINK_H
#define IMAGESOURCESINK_H

#include "imagepacket.h"
#include "QtGui"

class ImageSourceSink
{
public:

    virtual bool Init()=0;
    virtual bool StartAcquisition(QString dev="0")=0;
    virtual bool StopAcquisition()=0;
    virtual bool ReleaseCamera()=0;
    virtual bool GrabFrame(ImagePacket& target,int indexIncrement=1)=0;
    virtual bool RecordFrame(ImagePacket& source)=0;
    virtual bool StartRecording(QString recFold, QString codec,int fps, int cols, int rows)=0;
    virtual bool StopRecording()=0;
    virtual bool IsOpened()=0;
};

#endif // IMAGESOURCESINK_H
