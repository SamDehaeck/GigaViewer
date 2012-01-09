#include "coordinator.h"

Coordinator::Coordinator(MainGui* theGivenGui, QObject *parent) :
    QObject(parent),theGui(theGivenGui),guiMode(TRUE),picBack(parent),camBack(parent)
{
    if (guiMode) {
        connect(theGui,SIGNAL(newPicNeeded(QString)),&picBack,SLOT(LoadNewImage(QString)));
        connect(&picBack,SIGNAL(NewImageReady(cv::Mat)),theGui,SLOT(newImageReceived(cv::Mat)));
        connect(theGui,SIGNAL(newOpencvFeedNeeded()),&camBack,SLOT(StartAcquisition()));
        connect(&camBack,SIGNAL(NewImageReady(cv::Mat)),theGui,SLOT(newImageReceived(cv::Mat)));
    }
}
