#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(true),picBack(parent),camBack(parent)
  ,opencvRunning(false),avtRunning(false)
{
    if (guiMode) {
        connect(theGui,SIGNAL(newPicNeeded(QString)),&picBack,SLOT(LoadNewImage(QString)));
        connect(&picBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));
        connect(&camBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));

        connect(theGui,SIGNAL(newMovieNeeded(QString)),this,SLOT(LoadNewMovie(QString)));
        connect(theGui,SIGNAL(newOpencvFeedNeeded(bool)),this,SLOT(controlCameraThread(bool)));
        connect(theGui,SIGNAL(implementNewFps(int)),this,SLOT(changeFps(int)));
        connect(theGui,SIGNAL(startRecording(bool,QString,QString)),&camBack,SLOT(StartRecording(bool,QString,QString)));
        connect(theGui,SIGNAL(newAvtFeedNeeded(bool)),this,SLOT(StartNewAVT(bool)));
        connect(theGui,SIGNAL(newVimbaFeedNeeded(bool)),this,SLOT(StartNewVimba(bool)));
        connect(theGui,SIGNAL(newIdsFeedNeeded(bool)),this,SLOT(StartNewIds(bool)));
        connect(theGui,SIGNAL(setShutter(int)),this,SLOT(changeShutter(int)));
        connect(theGui,SIGNAL(setAutoShutter(bool)),this,SLOT(setAutoShutter(bool)));
        connect(&camBack,SIGNAL(shutterChanged(int)),theGui,SLOT(gotNewShutSpeed(int)));
        connect(&camBack,SIGNAL(fpsChanged(int)),theGui,SLOT(gotNewFpsFromBackend(int)));
        connect(theGui,SIGNAL(skipFrames(bool)),&camBack,SLOT(skipForwardBackward(bool)));
        connect(theGui,SIGNAL(setRoiRows(int)),&camBack,SLOT(setRoiRows(int)));
        connect(theGui,SIGNAL(setRoiCols(int)),&camBack,SLOT(setRoiCols(int)));
#ifdef TRACKING
        connect(theGui,SIGNAL(pluginSettingsChanged(QMap<QString,QVariant>)),&camBack,SLOT(changedPluginSettings(QMap<QString,QVariant>)));
#endif
    }
}

void Coordinator::controlCameraThread(bool startNew,QString dev)
{
    if (startNew) {
        if (camBack.StartAcquisition(dev)) {
            camBack.start(QThread::HighPriority); // max is QThread::TimeCriticalPriority
            opencvRunning=true;
        } else {
            //reset gui buttons
            theGui->returnToStart();
        }
    } else {
        if (camBack.isRunning()) {
            camBack.StopAcquisition();
            if (!camBack.wait(5000)) {
                qDebug()<<"Had to kill the thread";
                camBack.terminate();
            }
            opencvRunning=false;
            camBack.ReleaseCamera(); //this checks first if it is opened. Needs to be called in same thread as camera.open
        } else {
            //qDebug()<<"Thread not running";
        }

    }
}

void Coordinator::changeFps(int newFps)
{
    if (camBack.isRunning()) {
        camBack.SetInterval(newFps);
    }
}

void Coordinator::LoadNewMovie(QString theMovie)
{
     controlCameraThread(true,theMovie);
}

void Coordinator::stopAcquisition()
{
    if (opencvRunning) {
        controlCameraThread(false);
    }
}

void Coordinator::StartNewAVT(bool startNew)
{
    controlCameraThread(startNew,"AVT");
}

void Coordinator::StartNewVimba(bool startNew)
{
    controlCameraThread(startNew,"Vimba");
}

void Coordinator::StartNewIds(bool startNew)
{
    controlCameraThread(startNew,"IDS");
}

void Coordinator::changeShutter(int time)
{
    if (camBack.isRunning()) {
        camBack.SetShutter(time);
    }
}

void Coordinator::setAutoShutter(bool fitRange)
{
    if (camBack.isRunning()) {
        camBack.SetAutoShutter(fitRange);
    }
}


