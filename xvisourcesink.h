#ifndef XVISOURCESINK_H
#define XVISOURCESINK_H

#include "imagepacket.h"
#include "imagesourcesink.h"

class XviSourceSink : public ImageSourceSink
{
public:
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
//    bool RecordFrame(ImagePacket& source);
//    bool StartRecording(QString recFold, QString codec, int fps, int cols, int rows);
//    bool StopRecording();
    bool IsOpened();
    bool SkipFrames(bool forward);

private:
    FILE * xvi;
    int xvinpixels;
    uint64_t headersize;
    uint64_t bytesperchunk;
    uint64_t footersize;
    uint32_t bitsperpixel;
    QString dataformat;
    int rows,cols;
    ulong nFrames;
    int currPos;
};

#endif // XVIBACKEND_H
