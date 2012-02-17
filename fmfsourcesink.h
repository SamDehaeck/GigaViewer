#ifndef FMFSOURCESINK_H
#define FMFSOURCESINK_H

#include "imagepacket.h"
#include "imagesourcesink.h"

class FmfSourceSink : public ImageSourceSink
{
public:
    bool Init();
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool RecordFrame(ImagePacket& source);
    bool StartRecording(QString recFold, QString codec, int fps, int cols, int rows);
    bool StopRecording();
    bool IsOpened();

private:
    FILE * fmf;
    int fmfnpixels;
    uint64_t headersize;
    uint64_t bytesperchunk;
    uint32_t bitsperpixel;
    int rows,cols;
    int nFrames;
    int currPos;
};

#endif // FMFBACKEND_H
