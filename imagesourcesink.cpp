#include "imagesourcesink.h"

bool ImageSourceSink::Init() {
    return FALSE;
}

bool ImageSourceSink::StartAcquisition(QString) {
    return FALSE;
}

bool ImageSourceSink::StopAcquisition() {
    return FALSE;
}

bool ImageSourceSink::ReleaseCamera() {
    return FALSE;
}

bool ImageSourceSink::GrabFrame(ImagePacket&,int) {
    return FALSE;
}

bool ImageSourceSink::RecordFrame(ImagePacket&) {
    return FALSE;
}

bool ImageSourceSink::StartRecording(QString,QString,int,int,int) {
    return FALSE;
}

bool ImageSourceSink::StopRecording() {
    return FALSE;
}

bool ImageSourceSink::IsOpened() {
    return FALSE;
}

bool ImageSourceSink::SetInterval(int) {
    return FALSE;
}

bool ImageSourceSink::SetShutter(int) {
    return FALSE;
}

int ImageSourceSink::SetAutoShutter(bool)
{
    return 0;
}


