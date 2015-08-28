#include "picbackend.h"

PicBackend::PicBackend(QObject *parent) :
    QObject(parent)
{
}

void PicBackend::LoadNewImage(QString theName)
{
    Image.image=cv::imread(theName.toUtf8().data(),0);
    emit NewImageReady(Image);
}
