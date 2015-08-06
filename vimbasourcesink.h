#ifndef VIMBASOURCESINK_H
#define VIMBASOURCESINK_H

//defines necessary for VIMBA header..
#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <time.h>
#endif

#include "VimbaCPP/Include/VimbaCPP.h"


// now other imports
#include <QThread>
#include "imagepacket.h"
#include "imagesourcesink.h"
#include <opencv2/opencv.hpp>


class VimbaSourceSink : public ImageSourceSink
{
public:
    VimbaSourceSink();
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool IsOpened();
    bool SetInterval(int msec);
    bool SetShutter(int shutTime);
    int SetAutoShutter(bool fitRange);

private:
    AVT::VmbAPI::VimbaSystem & system;
    AVT::VmbAPI::CameraPtr     pCamera;

};

#endif

