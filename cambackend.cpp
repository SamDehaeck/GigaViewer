#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include "regexsourcesink.h"
#include <QDebug>
#include "avtsourcesink.h"


CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(0),currSource(0), recording(false),timerInterval(100),reversePlay(false),needTimer(true),running(false)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()));
    connect(this,SIGNAL(startTheTimer(int)),this,SLOT(willStartTheTimer(int)));
    connect(this,SIGNAL(stopTheTimer()),this,SLOT(willStopTheTimer()));
}

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

    // record the image if necessary, could add some skipping here to improve max display speed
    if (recording && currSink) currSink->RecordFrame(currImage);

    // do some processing of the image;
    cv::Mat intImage;
    if (currImage.image.type()==CV_8UC3) {
        cv::cvtColor(currImage.image,intImage,CV_BGR2GRAY);
    } else {
        intImage=currImage.image;
    }

    // Fastest approach, but only if there is no saturation, I could find max intensity
    // peak (minMaxLoc) and do a two-way 3-point Gaussian fit on that.
    // perhaps add a Gaussian blurring step?
    // to avoid 'jitter' in the new location I could do a moving average location
    // accumulation over several images => store 5 old locations and take the average or
    // extrapolated location.

//    cv::Mat mask;
//    cv::GaussianBlur(intImage,mask,cv::Size(0,0),5);

    double maxVal;
    cv::Point maxPos;
    cv::minMaxLoc(intImage,0,&maxVal,0,&maxPos);

    int x=maxPos.x;
    int y=maxPos.y;
    int ymin=y-1;
    int yplus=y+1;
    int xmin=x-1;
    int xplus=x+1;
    int valXmin=intImage.at<uint8_t>(ymin,x);
    int valXplus=intImage.at<uint8_t>(yplus,x);
    int valYmin=intImage.at<uint8_t>(y,xmin);
    int valYplus=intImage.at<uint8_t>(y,xplus);

    double LM1,LM2,LM3;
    LM1=cv::log(valXmin);
    LM2=cv::log(maxVal);
    LM3=cv::log(valXplus);
    double N=LM1-LM3;
    double D=2.*(LM1-2*LM2+LM3);
    double Rx=x+N/D;


    LM1=cv::log(valYmin);
    LM2=cv::log(maxVal);
    LM3=cv::log(valYplus);
    N=LM1-LM3;
    D=2.*(LM1-2*LM2+LM3);
    double Ry=y+N/D;
    cv::Point cent(Rx,Ry);

//    std::cout<<"Maxpos: "<<x<<", "<<y<<" and gauss interpol: "<<Rx<<", "<<Ry<<" for maxval: "<<maxVal<<std::endl;
    cv::circle(currImage.image,cent,4,100,-1);


    // could also use cv::Moments if the peak is not Gaussian due to saturation.
    // This takes the original image and a mask
    // If the wanted spot is the only thing bypassing the threshold => use
    // mask as a mask. To remove small points I could use drawContours and only
    // plot the biggest one or faster is just use erosion filter
    cv::Mat mask;
//    cv::threshold(intImage,mask,0,255,cv::THRESH_OTSU|cv::THRESH_BINARY_INV);
    cv::threshold(intImage,mask,200,255,cv::THRESH_BINARY_INV);
//    cv::threshold(intImage,mask,200,255,cv::THRESH_BINARY);
    cv::Mat nMat;
    intImage.copyTo(nMat,mask);
//    nMat=mask.mul(intImage);
//    cv::vector<cv::vector<cv::Point> > magContours;
//    cv::findContours(mask,magContours,cv::RETR_LIST,cv::CHAIN_APPROX_NONE);

    cv::Moments caract;
    caract=cv::moments(nMat,true);
//    cv::Size nsiz=nMat.size();
//    std::cout<<"Moment: "<<caract.m00/nsiz.width<<std::endl;//" - "<<caract.m00%nsiz.width<<std::endl;

    // once the object is detected, I could use for instance cv::phaseCorrelate in a local neighbourhood
    // of the pixel (128x128 pix) to find the new location? => but need an old image => not so interesting
    //

    currImage.image=nMat.clone();

    // could also do some skipping here in order not to show all images.
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
//#ifdef Q_OS_LINUX
        currSource=new AvtSourceSink();
        needTimer=false;
//#else
  //      qDebug()<<"Prosilica backend not supported on windows";
    //    return false;
//#endif
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



