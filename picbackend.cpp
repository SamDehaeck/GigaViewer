#include "picbackend.h"

PicBackend::PicBackend(QObject *parent) :
    QObject(parent)
{
}

void PicBackend::LoadNewImage(QString theName)
{
#ifdef Q_OS_WIN32
    Image.image=cv::imread(theName.toStdString().c_str(),0);
#else
    Image.image=cv::imread(theName.toUtf8().data(),0);
#endif
    emit NewImageReady(Image);
}
