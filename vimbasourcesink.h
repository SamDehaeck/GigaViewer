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
#include <QObject>
#include <QtGui>
#include "imagepacket.h"
#include "imagesourcesink.h"
#include "vimbaframeobserver.h"
#include <opencv2/opencv.hpp>
#include "cambackend.h"
#include "avtsourcesink.h"

class VimbaSourceSink : public ImageSourceSink
{
public:
    VimbaSourceSink(CamBackend* parent, QString formatstring);
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool IsOpened();
    bool SetInterval(int msec);
    bool SetShutter(int shutTime);
    int SetAutoShutter(bool fitRange);
    std::vector<std::string> listPixelFormats();
    void setFormat(QString formatstring);


private:
    std::vector<std::string> listOptions(AVT::VmbAPI::FeaturePtr pFeature);

    AVT::VmbAPI::VimbaSystem & system;
    AVT::VmbAPI::CameraPtr     pCamera;
    VimbaFrameObserver* frameWatcher;
    CamBackend* parent;
    int bufCount;
    VmbInt64_t height,width;
    VmbInt64_t maxHeight,maxWidth;
    double frameRate,exposure;
    VmbInt64_t camFreq,initialStamp;
    int timeOffset;
    QString format;


};

#endif

