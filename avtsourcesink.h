#ifndef AVTSOURCESINK_H
#define AVTSOURCESINK_H

//defines necessary for PvApi header..
#define _LINUX
#define _x64
#include <PvApi.h>

#include <QThread>
#include "imagepacket.h"
#include "imagesourcesink.h"
#include "opencv2/opencv.hpp"

class Sleeper : public QThread {
public:
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
};

class AvtSourceSink : public ImageSourceSink
{
public:
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool RecordFrame(ImagePacket& source);
    bool StartRecording(QString , QString , int , int , int );
    bool StopRecording();
    bool IsOpened();
    bool SetInterval(int msec);
    bool SetShutter(int shutTime);
    int SetAutoShutter(bool fitRange);

private:

#define FRAMESCOUNT 50

    typedef struct
    {
        unsigned long   UID;
        tPvHandle       Handle;
        tPvFrame        Frames[FRAMESCOUNT];
        bool            Stop;
        unsigned long   Discarded;
    } tCamera;

    tCamera         GCamera;
    int Index;
    unsigned long Last;
    int rows,cols;
    double camTimeStep;
    double camTimeOffset;
    cv::Mat buffer;
    QVector<cv::Mat> matFrames;

};

#endif // AVTSOURCESINK_H
