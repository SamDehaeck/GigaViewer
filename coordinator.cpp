#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(TRUE),picBack(parent),camBack(parent)
  ,opencvRunning(FALSE),avtRunning(FALSE)
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
    }
}

void Coordinator::controlCameraThread(bool startNew,QString dev)
{
    if (startNew) {
        if (camBack.StartAcquisition(dev)) {
            camBack.start(QThread::HighPriority); // max is QThread::TimeCriticalPriority
            opencvRunning=TRUE;
        } else {
            //reset gui buttons
        }
    } else {
        if (camBack.isRunning()) {
            camBack.StopAcquisition();
            if (!camBack.wait(5000)) {
                qDebug()<<"Had to kill the thread";
                camBack.terminate();
            }
            opencvRunning=FALSE;
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
     controlCameraThread(TRUE,theMovie);
}

void Coordinator::stopAcquisition()
{
    if (opencvRunning) {
        controlCameraThread(FALSE);
    }
}

void Coordinator::StartNewAVT(bool startNew)
{
    controlCameraThread(startNew,"AVT");
}


