#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(TRUE),picBack(parent),camBack(parent)
{
//    connect(theGui,SIGNAL(newPicNeeded(QString)),&picBack,SLOT(LoadNewImage(QString)));
    connect(&picBack,SIGNAL(NewImageReady(cv::Mat)),theGui,SLOT(newPicReceived(cv::Mat)));
    connect(theGui,SIGNAL(newPicNeeded(QString)),&camBack,SLOT(StartAcquisition(QString)));
    connect(&camBack,SIGNAL(NewImageReady(cv::Mat)),theGui,SLOT(newPicReceived(cv::Mat)));
}
