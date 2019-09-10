#ifndef MRFSOURCESINK_H
#define MRFSOURCESINK_H

#include "imagepacket.h"
#include "imagesourcesink.h"

class MrfSourceSink : public ImageSourceSink
{
public:
    bool Init(QString params="");
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool IsOpened();
    bool SkipFrames(bool forward);

private:
    FILE * mrf;
    int mrf_npixels;
    uint64_t headersize;
    uint64_t bytesperchunk;
    uint64_t footersize;
    uint32_t bitsperpixel;
    QString dataformat;
    int rows,cols;
    uint nFrames;
    int currPos;
};

#endif // MRFSOURCESINK_H
