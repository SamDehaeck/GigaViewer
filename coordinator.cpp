#include "coordinator.h"

Coordinator::Coordinator(QObject *parent) :
    QObject(parent),picBack(parent),camBack(parent)
  ,opencvRunning(false),avtRunning(false)
{
//    backendThread=new QThread();
//    backendThread->start();
//    camBack.moveToThread(backendThread);

    //propagate the signals coming from the backend
    connect(&camBack,SIGNAL(NewImageReady(ImagePacket)),this,SIGNAL(NewImageReady(ImagePacket)));
    connect(&camBack,SIGNAL(shutterChanged(int)),this,SIGNAL(shutterChanged(int)));
    connect(&camBack,SIGNAL(fpsChanged(double)),this,SIGNAL(fpsChanged(double)));


}
/*
void Coordinator::setGui(MainGui *myGui) {

// THIS NOT YET FIXED!!!
#ifdef TRACKING
    connect(theGui,SIGNAL(pluginSettingsChanged(QMap<QString,QVariant>)),&camBack,SLOT(changedPluginSettings(QMap<QString,QVariant>)));
#endif
#ifdef ELLIPSE
    connect(theGui,SIGNAL(pluginSettingsChanged(QMap<QString,QVariant>)),&camBack,SLOT(changedPluginSettings(QMap<QString,QVariant>)));
#endif
}
*/

void Coordinator::controlCameraThread(bool startNew,QString dev)
{
    if (startNew) {
        if (camBack.StartAcquisition(dev)) {
            camBack.start(QThread::HighPriority); // max is QThread::TimeCriticalPriority
            opencvRunning=true;
        } else {
            //reset gui buttons
            // FIX THIS!!!!
            //theGui->returnToStart();
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

void Coordinator::changeFps(double newFps)
{
    if (camBack.isRunning()) {
        camBack.SetInterval(newFps);
    }
}

void Coordinator::setRoiCols(int cols)
{
    if (camBack.isRunning()) {
        camBack.setRoiCols(cols);
    }
}

void Coordinator::setRoiRows(int rows)
{
    if (camBack.isRunning()) {
        camBack.setRoiRows(rows);
    }
}

void Coordinator::skipForwardBackward(bool forward)
{
    if (camBack.isRunning()) {
        camBack.skipForwardBackward(forward);
    }
}

void Coordinator::StartRecording(bool start, QString recFold, QString codec,int skip)
{
    if (camBack.isRunning()) {
        camBack.StartRecording(start,recFold,codec,skip);
    }
}
