#ifndef FMFBUFFEREDSOURCESINK_H
#define FMFBUFFEREDSOURCESINK_H

#include "imagepacket.h"
#include "imagesourcesink.h"

class FmfBufferedSourceSink : public ImageSourceSink
{
public:
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool RecordFrame(ImagePacket& source);
    bool StartRecording(QString recFold, QString codec, int fps, int reccols, int recrows);
    bool StopRecording();
    bool IsOpened();
    bool SkipFrames(bool forward);

private:
    FILE * fmf;
    FILE * fmfrec;
    int fmfnpixels;
    long headersize;
    long recheadersize;
    long recNframespos;
    uint64_t bytesperchunk;
    uint32_t bitsperpixel;
    QString dataformat;
    int rows,cols;
    int nFrames;
    int currPos;
    QString basename;

    QVector<double> timestamps;
    QVector<cv::Mat> frames;
    double startTime;
    bool recording;
};

QDataStream &operator<<(QDataStream &out, const cv::Mat &matrix);
//QDataStream &operator>>(QDataStream &in, cv::Mat &matrix);  // do later perhaps

#endif // FMFBUFFERED_H
