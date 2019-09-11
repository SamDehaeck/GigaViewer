#include "imagesourcesink.h"

bool ImageSourceSink::Init(QString) {
    return false;
}

bool ImageSourceSink::StartAcquisition(QString) {
    return false;
}

bool ImageSourceSink::GrabFrame(ImagePacket&,int) {
    return false;
}

bool ImageSourceSink::StopAcquisition() {
    return false;
}

bool ImageSourceSink::ReleaseCamera() {
    return false;
}

QString ImageSourceSink::StartRecording(QString,QString,int,int,int) {
    return QString("");
}

bool ImageSourceSink::RecordFrame(ImagePacket&) {
    return false;
}

bool ImageSourceSink::StopRecording() {
    return false;
}

bool ImageSourceSink::IsOpened() {
    return false;
}

double ImageSourceSink::SetInterval(double) {
    qInfo("Setting fps not supported for this source.");
    return 0;
}

bool ImageSourceSink::SetRoiRows(int) {
    return false;
}

bool ImageSourceSink::SetRoiCols(int) {
    return false;
}

bool ImageSourceSink::SetShutter(int) {
    qInfo("Setting shutter not supported for this source.");
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


