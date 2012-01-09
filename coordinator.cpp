#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(TRUE),picBack(parent),camBack(parent)
{
    if (guiMode) {
        connect(theGui,SIGNAL(newPicNeeded(QString)),&picBack,SLOT(LoadNewImage(QString)));
        connect(&picBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));
//        connect(theGui,SIGNAL(newOpencvFeedNeeded()),&camBack,SLOT(StartAcquisition()));
        connect(theGui,SIGNAL(newOpencvFeedNeeded(bool)),this,SLOT(startOpenCvThread(bool)));
        connect(&camBack,SIGNAL(NewImageReady(ImagePacket)),theGui,SLOT(newImageReceived(ImagePacket)));
    }
}

void Coordinator::startOpenCvThread(bool startNew)
{
    if (startNew) {
        camBack.start(QThread::HighPriority); // max is QThread::TimeCriticalPriority
    } else {
        camBack.quit();
        camBack.wait();
        camBack.StopAcquisition();
    }
}

