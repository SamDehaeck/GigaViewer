#ifndef IMAGESOURCESINK_H
#define IMAGESOURCESINK_H

#include "imagepacket.h"
#include "QtGui"

class Sleeper : public QThread {
public:
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
};

class ImageSourceSink
{
public:

    virtual bool Init(QString params="");
    virtual bool StartAcquisition(QString dev="0");
    virtual bool StopAcquisition();
    virtual bool ReleaseCamera();
    virtual bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    virtual bool RecordFrame(ImagePacket& source);
    virtual QString StartRecording(QString recFold, QString codec,int fps, int cols, int rows);
    virtual bool StopRecording();
    virtual bool IsOpened();
    virtual double SetInterval(double msec);
    virtual bool SetRoiRows(int rows);
    virtual bool SetRoiCols(int cols);
    virtual bool SetShutter(int shutTime);
    virtual int SetAutoShutter(bool fitRange);
    virtual bool SkipFrames(bool forward);
    virtual ~ImageSourceSink() {}
};

#endif // IMAGESOURCESINK_H
