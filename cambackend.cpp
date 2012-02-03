#include "cambackend.h"
#include <QDebug>

CamBackend::CamBackend(QObject *parent) :
    QThread(parent),recording(FALSE),timerInterval(20),reversePlay(FALSE)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()), Qt::DirectConnection);
}

bool CamBackend::IsLive() {
    return liveMode;
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
    if ((!liveMode)&&reversePlay) { // looking at movie
        double newpos=camera.get(CV_CAP_PROP_POS_FRAMES)-2;
//        qDebug()<<"calculated:"<<newpos;
        if (newpos>=0) {
            camera.set(CV_CAP_PROP_POS_FRAMES,newpos);
        }
    }
//    qDebug()<<camera.get(CV_CAP_PROP_POS_FRAMES);
    camera >> currImage.image;

    if (currImage.image.rows==0) {
        StopAcquisition();
        return;
    }
    if (recording) record();
    emit NewImageReady(currImage);

}

bool CamBackend::StartAcquisition(QString dev)
{
    if (dev=="0") {
        camera.open(0);
        liveMode=TRUE;
    } else {
        camera.open(dev.toStdString());
        liveMode=FALSE;
    }
    return camera.isOpened();
}

void CamBackend::StopAcquisition()
{
    quit();
}

void CamBackend::ReleaseCamera()
{
    if (camera.isOpened()) camera.release();
}

void CamBackend::SetInterval(int newInt)
{
    reversePlay=newInt<0;
    timer.setInterval(abs(newInt));
}

void CamBackend::StartRecording(bool startRec,QString recFold, QString codec)
{
    if (startRec) {
        QDateTime mom = QDateTime::currentDateTime();
        QString filenam=recFold+"/"+mom.toString("yyyyMMdd-hhmmss")+".avi";
        int fourcc=0;
        if (codec=="MSMPEG4V2") {
            fourcc=CV_FOURCC('M','P','4','2'); // for mpeg4 from windows
        } else if (codec=="XVID") {
            fourcc=CV_FOURCC('F','M','P','4'); //for xvid
        } else {
            fourcc=0;// uncompressed raw format
        }
        int fps=timer.interval()/10;
        recFile=cv::VideoWriter(filenam.toStdString(),fourcc,fps,cv::Size(currImage.image.cols,currImage.image.rows));
    }
    recording=startRec;
}


bool CamBackend::Init()
{
    //No real need to init opencv separately
    return TRUE;
}
