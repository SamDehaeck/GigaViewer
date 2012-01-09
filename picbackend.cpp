#include "picbackend.h"

PicBackend::PicBackend(QObject *parent) :
    QObject(parent)
{
}

void PicBackend::LoadNewImage(QString theName)
{
    Image=cv::imread(theName.toStdString(),0);
    emit NewImageReady(Image);
}
