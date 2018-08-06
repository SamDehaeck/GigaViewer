#include "cambackend.h"
#include "opencvsourcesink.h"
#include "fmfsourcesink.h"
#include "fmfbufferedsourcesink.h"
#include "xvisourcesink.h"
#include "mrfsourcesink.h"
#include "regexsourcesink.h"
#include <QDebug>
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

#ifdef TRACKING
#include "marangonitracking.h"
#endif

#ifdef ELLIPSE
#include "ellipsedetection.h"
#endif

CamBackend::CamBackend(QObject *parent) :
    QThread(parent),currSink(nullptr),currSource(nullptr), recording(false),timerInterval(100),reversePlay(false),isPaused(false),needTimer(true),running(false),doPluginProcessing(false),skipImages(0),recSkip(0),stoppingRecording(false)
#ifdef TRACKING
  ,tracker(50,1)
#endif
  #ifdef ELLIPSE
    ,tracker(50)
  #endif

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
        qInfo()<<"Camera is not opened";
    }
}

void CamBackend::GrabFrame()
{
    if ((running)&&(!isPaused))  {
        int incr=1;
        if (reversePlay) incr=-1;

        if (!currSource->GrabFrame(currImage,incr)) {
            qInfo()<<"Some error occured while grabbing the frame";
            return;
        }
        if (currImage.image.rows==0) {
    //        StopAcquisition();
            return;
        }
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

#ifdef TRACKING
        // ADD IMAGE PROCESSING STEP HERE IF NECESSARY, ADJUST pixFormat if necessary to fit with display modifs
        tracker.processImage(currImage);
#endif

#ifdef ELLIPSE
        // ADD IMAGE PROCESSING STEP HERE IF NECESSARY, ADJUST pixFormat if necessary to fit with display modifs
        tracker.processImage(currImage);
#endif


        // ADAPT IMAGE FOR DISPLAY PURPOSES IF NECESSARY
        if (!stoppingRecording) {  // stopping a recording can block the queue and overflow the buffers
            AdaptForDisplay(currImage);

            emit NewImageReady(currImage);
        }
    }

}

// make new source
bool CamBackend::StartAcquisition(QString dev)
{
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
        qInfo()<<"AVT source not compiled";
        return false;
#endif
    } else if (dev=="Vimba") {
#ifdef VIMBA
        currSource=new VimbaSourceSink(this); //vimba needs the current object to connect the grabFrame signal
        needTimer=false;
        doesCallBack=true;
#else
        qInfo()<<"Vimba source not compiled";
        return false;
#endif
    } else if (dev=="IDS") {
#ifdef IDS
        currSource=new IdsSourceSink();
        needTimer=false;
        doesCallBack=false;
#else
        qInfo()<<"IDS source not compiled";
        return false;
#endif
    } else {
        currSource=new OpencvSourceSink();
        needTimer=true;
        doesCallBack=false;
    }

    if (currSource->Init()) {
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

void CamBackend::SetInterval(int newInt) {
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
            timer.setInterval(static_cast<int>(round(std::abs(newNewInt))));
            // no need to emit fpsChanged(newInt) because interface already updated
        } else {  // the source handles the interval by itself
            int newFps=currSource->SetInterval(static_cast<int>(round(std::abs(newNewInt))));
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
            currSink=new OpencvSourceSink();
        }
        int fps=timer.interval()/10;
        bool succ=currSink->StartRecording(recFold,codec,fps,currImage.image.cols,currImage.image.rows);
        if (!succ) {
            qInfo()<<"Start recording failed!";
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
        qInfo()<<"Skipping did not work";
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
                cv::cvtColor(currImage.image,dummy,CV_BayerRG2RGB);
                currImage.image=dummy;
            }
        }
    } else if (currImage.pixFormat=="BAYERGB8") { // do colour interpolation but only for showing to screen!
        if (currImage.image.channels()==1) {
            if (!recording) {
                cv::Mat dummy(currImage.image.rows,currImage.image.cols,CV_8UC3);
                cv::cvtColor(currImage.image,dummy,CV_BayerGB2RGB);
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
                cv::cvtColor(currImage.image,dummy,CV_BayerRG2RGB);
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
        qInfo()<<"Format in grab frame not understood: "<<currImage.pixFormat;
    }
}

bool CamBackend::initProcPlugin() {
    doPluginProcessing=true; // if init successfull, switch on the processing
    return true;
}

bool CamBackend::endProcPlugin() {
    return true;
}

bool CamBackend::DoProcPlugin(ImagePacket& ) {
    return true;
}

bool CamBackend::startRecPlugin(QString ) {
    return true;
}

bool CamBackend::endRecPlugin() {
    return true;
}

bool CamBackend::setSettingsPlugin(ImagePacket& ,QStringList ) {
    return true;
}
#ifdef TRACKING
void CamBackend::changedPluginSettings(QMap<QString,QVariant> settings) {
    if (settings["pluginName"]=="MarangoniTracking") {
//        qInfo()<<"should inform marangoni";
        tracker.ChangeSettings(settings);
    }

}
#endif

#ifdef ELLIPSE
void CamBackend::changedPluginSettings(QMap<QString,QVariant> settings) {
    if (settings["pluginName"]=="EllipseDetection") {
//        qInfo()<<"should inform marangoni";
        tracker.ChangeSettings(settings);
    }

}
#endif
