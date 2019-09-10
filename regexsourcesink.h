#ifndef REGEXSOURCESINK_H
#define REGEXSOURCESINK_H

#include "imagesourcesink.h"
#include <QtGui>

class RegexSourceSink : public ImageSourceSink
{
public:
    bool Init(QString params="");
    bool StartAcquisition(QString dev="0");
    bool StopAcquisition();
    bool ReleaseCamera();
    bool GrabFrame(ImagePacket& target,int indexIncrement=1);
    bool RecordFrame(ImagePacket& source);
    QString StartRecording(QString recFold, QString codec, int fps, int cols, int rows);
    bool StopRecording();
    bool IsOpened();
    bool SkipFrames(bool forward);

private:

    QStringList *goodFiles;
    QString dir;
    QString basename;
    QString extension;
    int index;
    int nFrames;
    QString pixFormat;

    QVector<double> timestamps;
    QVector<cv::Mat> frames;
    double startTime;

};

#endif // REGEXSOURCESINK_H
