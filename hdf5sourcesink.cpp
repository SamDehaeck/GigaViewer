#include "hdf5sourcesink.h"
#include "opencv2/opencv.hpp"

bool Hdf5SourceSink::Init()
{
    return true;
}

bool Hdf5SourceSink::StartAcquisition(QString dev)
{
    return true;
}

bool Hdf5SourceSink::StopAcquisition()
{
    return true;
}

bool Hdf5SourceSink::ReleaseCamera()
{
    return true;
}

bool Hdf5SourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    return false;
}

bool Hdf5SourceSink::RecordFrame(ImagePacket &source)
{
    return false;
}

bool Hdf5SourceSink::StartRecording(QString recFold, QString codec, int, int cols, int rows)
{
    return true;
}

bool Hdf5SourceSink::StopRecording()
{
    return true;
}

bool Hdf5SourceSink::IsOpened()
{
    return true;
}

bool Hdf5SourceSink::SkipFrames(bool forward) {
    return true;
}
