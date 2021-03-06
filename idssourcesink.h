#ifndef IDSSOURCESINK_H
#define IDSSOURCESINK_H

#include <ueye.h>

#include <QThread>
#include "imagepacket.h"
#include "imagesourcesink.h"
#include <opencv2/opencv.hpp>

class IdsSourceSink : public ImageSourceSink
{
public:
    bool Init(QString params="");
    bool StartAcquisition(QString dev="-1");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool IsOpened();
    double SetInterval(double msec);
    bool SetShutter(int shutTime);
    int SetAutoShutter(bool fitRange);
    bool SetRoiRows(int rows);
    bool SetRoiCols(int cols);
    bool SetColourMode(bool useHighQuality);  // quality mode for debayering
    bool SetPixelClock(int selection);

private:
    HIDS hCam;
    char* imgMem;
    int memId;
    int flagIDS;
    UEYEIMAGEINFO ImageInfo;
#ifdef Q_OS_WIN32
    HANDLE hEvent;
#endif

    int Index;
    unsigned long Last;
    int rows,cols;
    int maxHeight,maxWidth;
    double camTimeStep;
    double camTimeOffset;
    cv::Mat buffer;
    double timeOffset;
//    QVector<cv::Mat> matFrames;
    QString format;
    QString source;

};

#endif // IDSSOURCESINK_H
