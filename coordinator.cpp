#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(true),picBack(parent),camBack(parent)
  ,opencvRunning(false),avtRunning(false)
{
    if (guiMode) {
        connect(theGui,SIGNAL(newPicNeeded(QString)),&picBack,SLOT(LoadNewImage(QString)));
        connect(&picBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));
        connect(theGui,SIGNAL(newMovieNeeded(QString)),this,SLOT(LoadNewMovie(QString)));

        connect(theGui,SIGNAL(newOpencvFeedNeeded(bool)),this,SLOT(controlCameraThread(bool)));
        connect(&camBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));
        connect(theGui,SIGNAL(implementNewFps(int)),this,SLOT(changeFps(int)));
        connect(theGui,SIGNAL(startRecording(bool,QString,QString)),&camBack,SLOT(StartRecording(bool,QString,QString)));
        connect(theGui,SIGNAL(newAvtFeedNeeded(bool)),this,SLOT(StartNewAVT(bool)));
        connect(theGui,SIGNAL(newVimbaFeedNeeded(bool)),this,SLOT(StartNewVimba(bool)));
        connect(theGui,SIGNAL(setShutter(int)),this,SLOT(changeShutter(int)));
        connect(theGui,SIGNAL(setAutoShutter(bool)),this,SLOT(setAutoShutter(bool)));
        connect(&camBack,SIGNAL(shutterChanged(int)),theGui,SLOT(gotNewShutSpeed(int)));
        connect(theGui,SIGNAL(skipFrames(bool)),&camBack,SLOT(skipForwardBackward(bool)));
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
        } else {
//            qDebug()<<"Thread not running";
        }
        camBack.ReleaseCamera(); //this checks first if it is opened. Needs to be called in same thread as camera.open
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


