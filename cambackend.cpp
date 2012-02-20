#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include <QDebug>

CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(0),currSource(0), recording(FALSE),timerInterval(100),reversePlay(FALSE)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
}

void CamBackend::run()
{
    if (currSource->IsOpened()) {
        timer.setInterval(timerInterval);
        timer.start();
        exec(); //will go beyond this point when quit() is send from within this thread
        timer.stop();
    } else {
        qDebug()<<"Could not open camera";
    }
}

void CamBackend::GrabFrame()
{
    int incr=1;
    if (reversePlay) incr=-1;
    currSource->GrabFrame(currImage,incr);
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
        currSource=new FmfSourceSink;
    } else {
        currSource=new OpencvSourceSink;
    }
    currSource->Init();
    currSource->StartAcquisition(dev);
    return TRUE;
}

void CamBackend::StopAcquisition()
{
    currSource->StopAcquisition();
    quit();
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
    timer.setInterval(abs(newInt));
}

void CamBackend::StartRecording(bool startRec,QString recFold, QString codec)
{
    if (startRec) {
        currSink=new OpencvSourceSink;
        int fps=timer.interval()/10;
        currSink->StartRecording(recFold,codec,fps,currImage.image.cols,currImage.image.rows);
    } else { // stopping recording
        currSink->StopRecording();
        delete currSink;
        currSink=0;
    }
    recording=startRec;
}


