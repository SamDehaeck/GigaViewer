#include "imagesourcesink.h"

bool ImageSourceSink::Init() {
    return false;
}

bool ImageSourceSink::StartAcquisition(QString) {
    return false;
}

bool ImageSourceSink::StopAcquisition() {
    return false;
}

bool ImageSourceSink::ReleaseCamera() {
    return false;
}

bool ImageSourceSink::GrabFrame(ImagePacket&,int) {
    return false;
}

bool ImageSourceSink::RecordFrame(ImagePacket&) {
    return false;
}

bool ImageSourceSink::StartRecording(QString,QString,int,int,int) {
    return false;
}

bool ImageSourceSink::StopRecording() {
    return false;
}

bool ImageSourceSink::IsOpened() {
    return false;
}

int ImageSourceSink::SetInterval(int) {
    return 0;
}

bool ImageSourceSink::SetRoiRows(int) {
    return false;
}

bool ImageSourceSink::SetRoiCols(int) {
    return false;
}

bool ImageSourceSink::SetShutter(int) {
    return false;
}

int ImageSourceSink::SetAutoShutter(bool)
{
    return 0;
}

bool ImageSourceSink::SkipFrames(bool)
{
    return false;
}


