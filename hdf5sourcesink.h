#ifndef HDF5SOURCESINK_H
#define HDF5SOURCESINK_H

#include "imagepacket.h"
#include "imagesourcesink.h"

class Hdf5SourceSink : public ImageSourceSink
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
    bool SkipFrames(bool forward);

private:

};

#endif // HDF5BACKEND_H
