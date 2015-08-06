#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include "regexsourcesink.h"
#include <QDebug>
#include "avtsourcesink.h"
#include "vimbasourcesink.h"


CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(0),currSource(0), recording(false),timerInterval(100),reversePlay(false),needTimer(true),running(false)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()));
    connect(this,SIGNAL(startTheTimer(int)),this,SLOT(willStartTheTimer(int)));
    connect(this,SIGNAL(stopTheTimer()),this,SLOT(willStopTheTimer()));
}


// in coordinator, first the StartAcquisition method is called and then the Cambackend Thread is started which executes
// this run method. Two use cases here for the moment:
// 1. you need a timer: this is for sources which don't provide one (fmf, regex, opencv). In this case, we start a Qtimer
//    which calls the Grabframe method each time the timer ticks
// 2. you don't need a timer (Prosilica): you can get correct timing from the camera. Therefore, do a continuous loop
//    in which you directly call GrabFrame. This grabframe method blocks for the avtsource until the timing is right
//    (Function waitForQueuedFrame)
void CamBackend::run()
{
    if (currSource->IsOpened()) {
        if (needTimer) {
            emit startTheTimer(timerInterval); // needs to be started in a slot for interthread communication purposes...
            exec(); //will go beyond this point when quit() is send from within this thread
            emit stopTheTimer();
        } else {  // the AVT backend will block itself when waiting for the next frame. No need for an extra timer
            running=true;
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
        needTimer=true;
    } else if ((dev.contains(".png")) || (dev.contains(".bmp")) || (dev.contains(".jpg")) || (dev.contains(".JPG")) || (dev.contains(".tif")) || (dev.contains(".TIF"))) {
        currSource=new RegexSourceSink();
        needTimer=true;
    } else if (dev=="AVT") {
        currSource=new AvtSourceSink();
        needTimer=false;
    } else if (dev=="Vimba") {
        currSource=new VimbaSourceSink();
        needTimer=false;
    } else {
        currSource=new OpencvSourceSink();
        needTimer=true;
    }
    if (currSource->Init()) {
        return currSource->StartAcquisition(dev);
    } else {
        return false;
    }
}

void CamBackend::StopAcquisition()
{
    running=false;
    if (recording) {
        StartRecording(false);
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
        } else if (codec=="BMP" || codec=="PNG" || codec=="JPG") {
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
        qDebug()<<"Skipping did not work";
    }
}

void CamBackend::willStartTheTimer(int interval)
{
    timer.setInterval(interval);
    timer.start();
}

void CamBackend::willStopTheTimer()
{
    timer.stop();
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



