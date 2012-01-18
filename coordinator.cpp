#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(TRUE),picBack(parent),camBack(parent)
  ,opencvRunning(FALSE),avtRunning(FALSE)
{
    if (guiMode) {
        connect(theGui,SIGNAL(newPicNeeded(QString)),&picBack,SLOT(LoadNewImage(QString)));
        connect(&picBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));
        connect(theGui,SIGNAL(newMovieNeeded(QString)),this,SLOT(LoadNewMovie(QString)));

        connect(theGui,SIGNAL(newOpencvFeedNeeded(bool)),this,SLOT(controlOpenCvThread(bool)));
        connect(&camBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));
        connect(this,SIGNAL(stopFeed()),&camBack,SLOT(StopAcquisition()));
        connect(theGui,SIGNAL(implementNewFps(int)),this,SLOT(changeFps(int)));
    }
}

void Coordinator::controlOpenCvThread(bool startNew,QString dev)
{
    if (startNew) {
        if (camBack.Init()) {
            if (camBack.StartAcquisition(dev)) {
                camBack.start(QThread::HighPriority); // max is QThread::TimeCriticalPriority
                opencvRunning=TRUE;
            }
        } else {
//            theGui-> //reset button
        }
    } else {
        if (camBack.isRunning()) {
            emit stopFeed();
            if (!camBack.wait(5000)) {
                qDebug()<<"Had to kill the thread";
                camBack.terminate();
            }
            opencvRunning=FALSE;
        } else {
            qDebug()<<"Thread not running";
        }
        camBack.ReleaseCamera(); //this checks first if it is opened.
    }
}

void Coordinator::changeFps(int newFps)
{
    if (camBack.isRunning()) {
        camBack.timer.setInterval(newFps);
    }
}

void Coordinator::LoadNewMovie(QString theMovie)
{
    controlOpenCvThread(TRUE,theMovie);
}

void Coordinator::stopAcquisition()
{
    if (opencvRunning) {
        controlOpenCvThread(FALSE);
    }
}


