#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include "regexsourcesink.h"
#include <QDebug>

#ifdef Q_OS_LINUX
#include "avtsourcesink.h"
#endif


CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(0),currSource(0), recording(FALSE),timerInterval(100),reversePlay(FALSE),needTimer(TRUE),running(FALSE)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
}

void CamBackend::run()
{
    if (currSource->IsOpened()) {
        if (needTimer) {
            timer.setInterval(timerInterval);
            timer.start();
            exec(); //will go beyond this point when quit() is send from within this thread
            timer.stop();
        } else {  // the AVT backend will block itself when waiting for the next frame. No need for an extra timer
            running=TRUE;
            while (running) {
                GrabFrame();
            }
        }
    } else {
        qDebug()<<"Camera is not opened";
    }
}

void CamBackend::GrabFrame()
{
    int incr=1;
    if (reversePlay) incr=-1;

    if (!currSource->GrabFrame(currImage,incr)) {
        qDebug()<<"Some error occured while grabbing the frame";
        return;
    }
    if (currImage.image.rows==0) {
//        StopAcquisition();
        return;
    }
    if (recording && currSink) currSink->RecordFrame(currImage);
    emit NewImageReady(currImage);
}

bool CamBackend::StartAcquisition(QString dev)
{
    if (dev.contains(".fmf")) {
        currSource=new FmfSourceSink();
        needTimer=TRUE;
    } else if ((dev.contains(".png")) or (dev.contains(".bmp")) or (dev.contains(".jpg")) or (dev.contains(".JPG"))) {
        currSource=new RegexSourceSink();
        needTimer=TRUE;
    } else if (dev=="AVT") {
#ifdef Q_OS_LINUX
        currSource=new AvtSourceSink();
        needTimer=FALSE;
#else
        qDebug()<<"Prosilica backend not supported on windows"
        return FALSE;
#endif
    } else {
        currSource=new OpencvSourceSink();
        needTimer=TRUE;
    }
    if (currSource->Init()) {
        return currSource->StartAcquisition(dev);
    } else {
        return FALSE;
    }
}

void CamBackend::StopAcquisition()
{
    running=FALSE;
    if (recording) {
        StartRecording(FALSE);
    }
    currSource->StopAcquisition();
    if (needTimer) quit();
}

void CamBackend::ReleaseCamera()
{
    currSource->ReleaseCamera();
    delete currSource;
    currSource=0;
}

void CamBackend::SetInterval(int newInt)
{
    reversePlay=newInt<0;
    if (needTimer) {
        timer.setInterval(abs(newInt));
    } else {  // the source handles the interval by itself
        currSource->SetInterval(abs(newInt));
    }
}

void CamBackend::StartRecording(bool startRec,QString recFold, QString codec)
{
    if (startRec) {
        if (codec=="FMF") {
            currSink=new FmfSourceSink();
        } else if (codec=="BMP" or codec=="PNG" or codec=="JPG") {
            currSink=new RegexSourceSink();
        } else {
            currSink=new OpencvSourceSink();
        }
        int fps=timer.interval()/10;
        currSink->StartRecording(recFold,codec,fps,currImage.image.cols,currImage.image.rows);
    } else { // stopping recording
        currSink->StopRecording();
        delete currSink;
        currSink=0;
    }
    recording=startRec;
}

void CamBackend::skipForwardBackward(bool forward)
{
    if (!currSource->SkipFrames(forward)) {
        qDebug()<<"Skipping not yet implemented";
    }
}

void CamBackend::SetShutter(int shut)
{
    if (currSource!=0) {
        if (currSource->SetShutter(shut)) {
            emit shutterChanged(shut);
        }
    }
}

void CamBackend::SetAutoShutter(bool fitRange)
{
    if (currSource!=0) {
        int val= currSource->SetAutoShutter(fitRange);
        if (val!=0) {
            emit shutterChanged(val);
        }
    }
}



