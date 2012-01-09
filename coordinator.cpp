#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(TRUE),picBack(parent)
{
    connect(theGui,SIGNAL(newPicNeeded(QString)),&picBack,SLOT(LoadNewImage(QString)));
    connect(&picBack,SIGNAL(NewImageReady(cv::Mat)),theGui,SLOT(newPicReceived(cv::Mat)));
}
