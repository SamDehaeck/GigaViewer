#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include "fmfbufferedsourcesink.h"
#include "xvisourcesink.h"
#include "mrfsourcesink.h"
#include "regexsourcesink.h"
#include <QtGlobal>
#include <QtDebug>
#include <cmath>

#ifdef PVAPI
#include "avtsourcesink.h"
#endif

#ifdef VIMBA
#include "vimbasourcesink.h"
#endif

#ifdef IDS
#include "idssourcesink.h"
#endif

#ifdef ENABLE_HDF5
#include "hdf5sourcesink.h"
#endif

#ifdef ELLIPSE
#include "ellipsedetection.h"
#endif

CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(nullptr),currSource(nullptr), recording(false),timerInterval(100),reversePlay(false),isPaused(false),needTimer(true),running(false),doPluginProcessing(false),skipImages(0),recSkip(0),stoppingRecording(false)
  #ifdef ELLIPSE
    ,ellipse(50)
  #endif
{
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(GrabFrame()));
    connect(this,SIGNAL(startTheTimer(int)),this,SLOT(willStartTheTimer(int)));
    connect(this,SIGNAL(stopTheTimer()),this,SLOT(willStopTheTimer()));
    format="MONO8"; //fall back format
}


// in coordinator, first the StartAcquisition method is called and then the Cambackend Thread is started which executes
// this run method. Three use cases here for the moment:
// 1. you need a timer: this is for sources which don't provide one (fmf, regex, opencv). In this case, we start a Qtimer
//    which calls the Grabframe method each time the timer ticks
// 2. you don't need a timer (Prosilica & IDS): you can get correct timing from the camera. Therefore, do a continuous loop
//    in which you directly call GrabFrame. This grabframe method blocks for the avtsource until the timing is right
//    (Function waitForQueuedFrame)
// 3. you don't need a timer but the SDK uses callbacks when the frame is ready (VIMBA). Just idle till somebody says to quit
void CamBackend::run()
{
    if (currSource->IsOpened()) {
        if (needTimer) {
            emit startTheTimer(static_cast<int>(round(timerInterval))); // needs to be started in a slot for interthread communication purposes...
            exec(); //will go beyond this point when quit() is send from within this thread
            emit stopTheTimer();
        } else if (doesCallBack) { // Vimba does this and we should just wait till quit is called
            exec();
        } else {  // the AVT and IDS backend will block itself when waiting for the next frame. No need for an extra timer
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
    if ((running)&&(!isPaused))  {
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
        //currImage.fileName=recFileName.toStdString();
        //qInfo(recFileName.toLocal8Bit());
        //qInfo("Received cambackend: %s",currImage.fileName.data());
        //qInfo("Want to insert: %s",recFileName.toStdString().data());
        if (format!=currImage.pixFormat) {
    //        qInfo()<<"Switching data format";
            format=currImage.pixFormat;
        }

        // RECORD FRAME TO DISC
        if (skipImages>0) {
            if (currImage.seqNumber%skipImages!=0) {
                return; // skip recording and showing the image
            }
        }

        if (recSkip>0) {
            int newSkip=recSkip;
            if (skipImages>0) newSkip=newSkip*skipImages;
            if (currImage.seqNumber%newSkip==0) {
                if (recording && currSink) currSink->RecordFrame(currImage);
            }
        } else {
            if (recording && currSink) currSink->RecordFrame(currImage);
        }

        // now set common properties for the image message
        if (!currImage.message.contains("origin")) currImage.message.insert("origin",origin);
        currImage.message.insert("@timestamp",currImage.timeStamp);
        currImage.message.insert("seqnumber",currImage.seqNumber);
        currImage.message.insert("pixformat",currImage.pixFormat);
        if (recording && currSink) {
            currImage.message.insert("recording",1);
            currImage.message.insert("recfilename",recFileName);
            currImage.message.insert("recskip",recSkip);
        } else {
            currImage.message.insert("recording",0);
            currImage.message.insert("recfilename","-");
            currImage.message.insert("recskip",0);
        }
        if (needTimer) currImage.message.insert("interval",timer->interval());  //not really usefull in this case..

        doPlugin(currImage);  // will perform post-processing if necessary => on all images.

        // ADAPT IMAGE FOR DISPLAY PURPOSES IF NECESSARY
        if (!stoppingRecording) {  // stopping a recording can block the queue and overflow the buffers
            if (timerInterval<10) {
                int skipShows=static_cast<int>(round(15.0/timerInterval));
                if (currImage.seqNumber%skipShows==0) {
                    AdaptForDisplay(currImage);
                    emit NewImageReady(currImage);
                }
            } else {
                AdaptForDisplay(currImage);
                emit NewImageReady(currImage);
            }
        }
    }

}

// make new source
bool CamBackend::StartAcquisition(QString dev)
{
    origin=dev;
    if (dev.contains(".h5")) {
#ifdef ENABLE_HDF5
        currSource=new Hdf5SourceSink();
        needTimer=true;
        doesCallBack=false;
#else
        qInfo()<<"HDF5 source/sink not compiled";
        return false;
#endif
    } else if (dev.contains(".fmf")) {
        currSource=new FmfSourceSink();
        needTimer=true;
        doesCallBack=false;
    } else if (dev.contains(".xvi")) {
        currSource=new XviSourceSink();
        needTimer=true;
        doesCallBack=false;
    } else if (dev.contains(".mrf")) {
        currSource=new MrfSourceSink();
        needTimer=true;
        doesCallBack=false;
    } else if ((dev.contains(".png")) || (dev.contains(".bmp")) || (dev.contains(".jpg")) || (dev.contains(".JPG")) || (dev.contains(".tif")) || (dev.contains(".TIF"))) {
        currSource=new RegexSourceSink();
        needTimer=true;
        doesCallBack=false;
    } else if (dev=="AVT") {
#ifdef PVAPI
        currSource=new AvtSourceSink();
        needTimer=false;
        doesCallBack=false;
#else
        qDebug()<<"AVT source not compiled";
        return false;
#endif
    } else if (dev.contains("Vimba")) {
#ifdef VIMBA
        currSource=new VimbaSourceSink(this); //vimba needs the current object to connect the grabFrame signal
        needTimer=false;
        doesCallBack=true;
#else
        qDebug()<<"Vimba source not compiled";
        return false;
#endif
    } else if (dev.contains("IDS")) {
#ifdef IDS
        qDebug()<<"Will use IDS";
        currSource=new IdsSourceSink();
        needTimer=false;
        doesCallBack=false;
#else
        qDebug()<<"IDS source not compiled";
        return false;
#endif
    } else {
        if (dev=="0") origin="opencv-camera";
        currSource=new OpencvSourceSink();
        needTimer=true;
        doesCallBack=false;
    }

    if (currSource->Init(dev)) {
        if (currSource->StartAcquisition(dev)) {
            running=true;
            return true;
        } else {
            running=false;
            return false;
        }
    } else {
        running=false;
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

    currSource->StopAcquisition();
    if (needTimer) quit();
    if (doesCallBack) quit();
    //currImage.image.release();//=cv::Mat::zeros(1024,1024,CV_8U);
}

void CamBackend::ReleaseCamera()
{
    currSource->ReleaseCamera();
    delete currSource;
    currSource=nullptr;
}

void CamBackend::SetInterval(double newInt) {
    reversePlay=newInt<0;
    isPaused=newInt>3000000;
    if (!isPaused) {
//        qInfo()<<"newInt:   "<<newInt;
        // if interval is too big, possible that timer of cameras not working => smaller timer and skip some images.
        double maxInterval=1000.0; // ask from camera/ current source
        double newNewInt(newInt);
        if (newInt>maxInterval) {
                double ratio=newInt/maxInterval;
                skipImages=int(ceil(ratio));
                newNewInt=1.0*newInt/skipImages;
//                qInfo()<<"Will skip: "<<skipImages-1<<" images with interval of: "<<newNewInt;
        } else {
            skipImages=0;
        }

        if (needTimer) {
            timer->setInterval(static_cast<int>(round(std::abs(newNewInt))));  // cannot do better here than integer accuracy
            // no need to emit fpsChanged(newInt) because interface already updated
        } else {  // the source handles the interval by itself
            double newFps=currSource->SetInterval(std::abs(newNewInt));
            if (abs(newFps-newNewInt)<1e-5) {
                emit fpsChanged(newFps);
            }
        }
    }
}

//make new sink
void CamBackend::StartRecording(bool startRec, QString recFold, QString codec, int skip)
{
    if (startRec) {
        recSkip=skip+1;
        if (codec.contains("FMF")) {
            if (codec.contains("BUF-FMF")) {
                currSink=new FmfBufferedSourceSink();
//                qInfo()<<"Doing buffered version";
            } else {
                currSink=new FmfSourceSink();
            }
            if (format=="MONO8") {
                codec="FMF8";
            } else if (format=="MONO12") {
                codec="FMF12";
            } else if (format=="MONO14") {
                codec="FMF14";
            } else if (format=="BAYERRG8") {
                codec="FMFBAYERRG8";
            } else if (format=="BAYERGB8") {
                codec="FMFBAYERGB8";
            } else if (format=="BAYERRG12") {
                codec="FMFBAYERRG12";
            } else if (format=="RGB8") {
                codec="FMFRGB8";
            } else if (format=="RGB8Packed") {
                codec="FMFRGB8";
            }

        } else if (codec=="BMP" || codec=="PNG" || codec=="JPG") {
            currSink=new RegexSourceSink();
        } else if (codec=="HDF5") {
#ifdef ENABLE_HDF5
            currSink=new Hdf5SourceSink();
            if (format=="MONO8") {
                codec="HDF8";
            } else if (format=="MONO12") {
                codec="HDF12";
            } else if (format=="MONO14") {
                codec="HDF14";
            } else if (format=="BAYERRG8") {
                codec="HDFBAYERRG8";
            } else if (format=="BAYERRG12") {
                codec="HDFBAYERRG12";
            } else if (format=="BAYERGB8") {
                codec="HDFBAYERGB8";
            } else if (format=="RGB8") {
                codec="HDFRGB8";
            } else if (format=="RGB8Packed") {
                codec="HDFRGB8";
            }
#else
            qInfo()<<"Hdf5 source/sink not compiled in";
#endif
        } else {
            qInfo()<<"Am here, not sure what to do...";
            currSink=new OpencvSourceSink();
        }
        int fps=timer->interval()/10;
        recFileName=currSink->StartRecording(recFold,codec,fps,currImage.image.cols,currImage.image.rows);
        //qInfo("This is the recFile: %s",recFileName.toLocal8Bit().data());
        if (recFileName=="") {
            qDebug()<<"Starting recording failed!";
            delete currSink;
            currSink=nullptr;
            recording=false;
            return;
        }
        stoppingRecording=false;
    } else { // stopping recording
        if (currSink!=nullptr) {
            recording=false;
            stoppingRecording=true;  // to prevent the display queue to overflow!
            currSink->StopRecording();
            delete currSink;
            currSink=nullptr;
            stoppingRecording=false; // show images again on screen
        }
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
    timer->setInterval(interval);
    timer->start();
}

void CamBackend::willStopTheTimer()
{
    timer->stop();
}

void CamBackend::SetShutter(int shut)
{
    if (currSource!=nullptr) {
        if (currSource->SetShutter(shut)) {
            emit shutterChanged(shut);
        }
    }
}

void CamBackend::SetAutoShutter(bool fitRange)
{
    if (currSource!=nullptr) {
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

// only 8-bit/16-bit gray or 8-bit RGB can be displayed to screen => adjust if a different format
void CamBackend::AdaptForDisplay(ImagePacket& currImage) {

    if (currImage.pixFormat=="") {
        //sink does not support it yet
        currImage.pixFormat="MONO8";
    }
    if (currImage.pixFormat.contains("MONO")) {
        if (currImage.image.depth()==2) { //0: CV_8U - 1: CV_8S - 2: CV_16U - 3: CV_16S
            double max;
            cv::minMaxLoc(currImage.image,nullptr,&max);
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
            if (!recording) {  // when recording, don't bother doing interpolation only for display
                cv::Mat dummy(currImage.image.rows,currImage.image.cols,CV_8UC3);
                cv::cvtColor(currImage.image,dummy,cv::COLOR_BayerRG2RGB);
                currImage.image=dummy;
            }
        }
    } else if (currImage.pixFormat=="BAYERGB8") { // do colour interpolation but only for showing to screen!
        if (currImage.image.channels()==1) {
            if (!recording) {
                cv::Mat dummy(currImage.image.rows,currImage.image.cols,CV_8UC3);
                cv::cvtColor(currImage.image,dummy,cv::COLOR_BayerGB2RGB);
                currImage.image=dummy;
            }
        }
    } else if (currImage.pixFormat=="BAYERRG12") {
        double max;
        cv::minMaxLoc(currImage.image,nullptr,&max);
        if (max<4096) currImage.image=currImage.image*16;  //16 only correct for scaling up 12bit images!!

        if (currImage.image.channels()==1) {
            if (!recording) {  // when recording, don't bother doing interpolation only for display
                cv::Mat dummy(currImage.image.rows,currImage.image.cols,CV_16UC3);
                cv::cvtColor(currImage.image,dummy,cv::COLOR_BayerRG2RGB);
                currImage.image=dummy;
            }
        }


    } else if ((currImage.pixFormat=="RGB8")||(currImage.pixFormat=="RGB8Packed")) {
//        cv::Mat dummy(currImage.image.rows,currImage.image.cols,CV_8UC3);
//        cv::cvtColor(currImage.image,dummy,CV_RGB2BGR);
//        currImage.image=dummy;
//        qInfo()<<"Got a RGB8 frame"; //Nothing to do
    } else if (currImage.pixFormat=="FLOAT") {
        double min,max;
        cv::minMaxLoc(currImage.image,&min,&max);
        double stretch=255.0/(max-min);
        double shift=-min*stretch;
        cv::Mat temp;
        currImage.image.convertTo(temp,CV_8U,stretch,shift);
        currImage.image=temp.clone();
        //qInfo()<<"Got a Float frame";
    } else if (currImage.pixFormat=="BOOL") {
        cv::Mat temp;
        currImage.image.convertTo(temp,CV_8U,255,0);
        currImage.image=temp.clone();
    } else if (currImage.pixFormat=="BGR8") {
        //nothing to do
    } else {
        qDebug()<<"Format in grab frame not understood: "<<currImage.pixFormat;
    }
}

void CamBackend::changedPluginSettings(QMap<QString,QVariant> settings) {
#ifdef ELLIPSE
    if (settings["pluginName"]=="EllipseDetection") {
        ellipse.ChangeSettings(settings);
    }
#endif
}

void CamBackend::doPlugin(ImagePacket& currIm) {
#ifdef ELLIPSE
    ellipse.processImage(currIm);
#endif
}

