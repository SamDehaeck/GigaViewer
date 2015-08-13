#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include "regexsourcesink.h"
#include <QDebug>
#include "avtsourcesink.h"
#include "vimbasourcesink.h"
#include <QInputDialog>


CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(0),currSource(0), recording(false),timerInterval(100),reversePlay(false),needTimer(true),running(false)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(GrabFrame()));
    connect(this,SIGNAL(startTheTimer(int)),this,SLOT(willStartTheTimer(int)));
    connect(this,SIGNAL(stopTheTimer()),this,SLOT(willStopTheTimer()));
    format="MONO8"; //fall back format
}


// in coordinator, first the StartAcquisition method is called and then the Cambackend Thread is started which executes
// this run method. Three use cases here for the moment:
// 1. you need a timer: this is for sources which don't provide one (fmf, regex, opencv). In this case, we start a Qtimer
//    which calls the Grabframe method each time the timer ticks
// 2. you don't need a timer (Prosilica): you can get correct timing from the camera. Therefore, do a continuous loop
//    in which you directly call GrabFrame. This grabframe method blocks for the avtsource until the timing is right
//    (Function waitForQueuedFrame)
// 3. you don't need a timer but the SDK uses callbacks when the frame is ready (VIMBA). Just idle till somebody says to quit
void CamBackend::run()
{
    if (currSource->IsOpened()) {
        if (needTimer) {
            emit startTheTimer(timerInterval); // needs to be started in a slot for interthread communication purposes...
            exec(); //will go beyond this point when quit() is send from within this thread
            emit stopTheTimer();
        } else if (doesCallBack) { // Vimba does this and we should just wait till quit is called
            exec();
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

    // adapt image if not 8 bits
    if (currImage.pixFormat=="") {
        //sink does not support it yet
        currImage.pixFormat="MONO8";
    }
    if (currImage.pixFormat.contains("MONO")) {
        if (currImage.image.depth()==2) { //0: CV_8U - 1: CV_8S - 2: CV_16U - 3: CV_16S
            double max;
            cv::minMaxLoc(currImage.image,NULL,&max);
            if (currImage.pixFormat=="MONO12") {
                if (max<4096) currImage.image=currImage.image*16;  //16 only correct for scaling up 12bit images!!
            } else if (currImage.pixFormat=="MONO14") {
                if (max<16384) currImage.image=currImage.image*4;
            }
            /* Alternative is to scale down to 8bits but this requires making a new Mat..
            cv::Mat newMat;
            currImage.image.convertTo(newMat, CV_8U, 1./16.);
            currImage.image=newMat;*/
        }
    } else if (currImage.pixFormat=="BAYERRG8") { // do colour interpolation but only for showing to screen!
        if (currImage.image.channels()==1) {
            cv::Mat dummy(currImage.image.rows,currImage.image.cols,CV_8UC3);
            cv::cvtColor(currImage.image,dummy,CV_BayerRG2RGB);
            currImage.image=dummy;
        }
    } else {
        qDebug()<<"Format in grab frame not understood: "<<currImage.pixFormat;
    }


    emit NewImageReady(currImage);
}

// make new source
bool CamBackend::StartAcquisition(QString dev)
{
    if (dev.contains(".fmf")) {
        currSource=new FmfSourceSink();
        needTimer=true;
        doesCallBack=false;
    } else if ((dev.contains(".png")) || (dev.contains(".bmp")) || (dev.contains(".jpg")) || (dev.contains(".JPG")) || (dev.contains(".tif")) || (dev.contains(".TIF"))) {
        currSource=new RegexSourceSink();
        needTimer=true;
        doesCallBack=false;
    } else if (dev=="AVT") {
        currSource=new AvtSourceSink();
        needTimer=false;
        doesCallBack=false;
    } else if (dev=="Vimba") {
        currSource=new VimbaSourceSink(this,format); //vimba needs the current object to connect the grabFrame signal
        needTimer=false;
        doesCallBack=true;
    } else {
        currSource=new OpencvSourceSink();
        needTimer=true;
        doesCallBack=false;
    }
    if (currSource->Init()) {
        if (dev=="Vimba") {
            std::vector<std::string> pixF;
            QStringList items;
            pixF=static_cast<VimbaSourceSink*>(currSource)->listPixelFormats();
            for (uint i=0;i<pixF.size();i++) {
                if (pixF[i]=="Mono8") {
                    items<<"MONO8";
                } else if (pixF[i]=="Mono12") {
                    items<<"MONO12";
                } else if (pixF[i]=="Mono14") {
                    items<<"MONO14";
                } else if (pixF[i]=="BayerRG8") {
                    items<<"BAYERRG8";
                } else {
                    if (!QString::fromStdString(pixF[i]).contains("Packed")) {
                        qDebug()<<"This pixel-mode not yet available in Gigaviewer: "<<QString::fromStdString(pixF[i]);
                    }
                }

            }

            bool ok;
            QString item = QInputDialog::getItem(NULL, "Pixel format",
                                                "Selection options:", items, 0, false, &ok);
            if (ok && !item.isEmpty()) {
                format=item;
                static_cast<VimbaSourceSink*>(currSource)->setFormat(format);
                qDebug()<<"Selected "<<format;
            }
        }
        return currSource->StartAcquisition(dev);
    } else {
        return false;
    }
}

// stop source
void CamBackend::StopAcquisition()
{
    running=false;
    if (recording) {
        StartRecording(false);
    }
    currImage.image.release();//=cv::Mat::zeros(1024,1024,CV_8U);

    currSource->StopAcquisition();
    if (needTimer) quit();
    if (doesCallBack) quit();
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

//make new sink
void CamBackend::StartRecording(bool startRec,QString recFold, QString codec)
{
    if (startRec) {
        if (codec.contains("FMF")) {
            currSink=new FmfSourceSink();
            if (format=="MONO8") {
                codec="FMF8";
            } else if (format=="MONO12") {
                codec="FMF12";
            } else if (format=="MONO14") {
                codec="FMF14";
            } else if (format=="BAYERRG8") {
                codec="FMFRGB8";
            }

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

void CamBackend::setRoiRows(int rows) {
    currSource->SetRoiRows(rows);
}

void CamBackend::setRoiCols(int cols) {
    currSource->SetRoiCols(cols);
}



