#include "cambackend.h"
#include <QDebug>

CamBackend::CamBackend(QObject *parent) :
    QThread(parent),recording(FALSE),timerInterval(20)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
}

bool CamBackend::IsLive() {
    if (liveMode) {
        if (camera.isOpened()) {
            return TRUE;
        }
    }
    return FALSE;
}

void CamBackend::run()
{
    if (camera.isOpened()) {
        timer.setInterval(timerInterval);
        timer.start();
        exec(); //will go beyond this point when quit() is send from within this thread
        timer.stop();
    } else {
        qDebug()<<"Could not open camera";
    }
}

void CamBackend::record()
{
    recFile<<currImage.image;
}

void CamBackend::GrabFrame()
{
    if (!camera.isOpened()) quit();
    if (liveMode) {
        camera >> currImage.image;
        if (currImage.image.rows==0) {
            StopAcquisition();
            return;
        }
        if (recording) record();
        emit NewImageReady(currImage);
    }
}

bool CamBackend::StartAcquisition(QString dev)
{
    if (dev=="0") {
        camera.open(0);
    } else {
        camera.open(dev.toStdString());
    }
    if (camera.isOpened()) {
        liveMode=TRUE;
        return TRUE;
    } else {
        liveMode=FALSE;
        return FALSE;
    }
    return FALSE;
}

void CamBackend::StopAcquisition()
{
    quit();
    liveMode=FALSE;
}

void CamBackend::ReleaseCamera()
{
    if (camera.isOpened()) camera.release();
}

void CamBackend::SetInterval(int newInt)
{
    timer.setInterval(newInt);
}

void CamBackend::StartRecording(bool startRec)
{
    if (startRec) {
        QDateTime mom = QDateTime::currentDateTime();
        QString filenam="/home/sam/"+mom.toString("yyyyMMdd-hh:mm:ss")+".avi";
        int fps=timer.interval()/10;
        recFile=cv::VideoWriter(filenam.toStdString(),CV_FOURCC('F','M','P','4'),fps,cv::Size(currImage.image.cols,currImage.image.rows));
    }
    recording=startRec;
}


bool CamBackend::Init()
{
    //No real need to init opencv separately
    return TRUE;
}
