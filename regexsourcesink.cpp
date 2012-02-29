#include "regexsourcesink.h"
#include <opencv2/opencv.hpp>

bool RegexSourceSink::Init()
{
    return TRUE;
}

bool RegexSourceSink::StartAcquisition(QString dev)
{
    QRegExp rx("(.+)/(.+)(\\..+)");
    int pos=0;

    pos=rx.indexIn(dev);
    dir=rx.cap(1);
    basename=rx.cap(2);
    extension=rx.cap(3);

    QRegExp rt("(\\d+$)");
    pos=rt.indexIn(basename);
    basename.truncate(pos);

//    qDebug()<<dir<<basename<<rt.cap(1)<<extension;

    QString regexString=basename+"\\d+"+extension;

//    qDebug()<<"Will load files that match"<<regexString;
    QRegExp movieRegex(regexString);
    QDir basedir(dir);
    if (!basedir.exists()) qDebug()<<"Extracted dir does not exist"<<dir;

    QStringList filt;
    filt<<"*"+extension;
    basedir.setNameFilters(filt);
    QStringList files=basedir.entryList();
    goodFiles=new QStringList;
    for (int i=0;i<files.count();i++) {
        if (movieRegex.indexIn(files.at(i))!=-1) {
            goodFiles->append(files.at(i));
        }
    }
    if (goodFiles->count()==0) qDebug()<<"No matching files found";
    index=0;

    return TRUE;
}

bool RegexSourceSink::StopAcquisition()
{
    return TRUE;
}

bool RegexSourceSink::ReleaseCamera()
{
    delete goodFiles;
    return TRUE;
}

bool RegexSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if (index+indexIncrement<goodFiles->count() and index+indexIncrement>0) {
        index+=indexIncrement;
        QFile dum(dir+"/"+goodFiles->at(index));
        if (dum.exists()) {
            target.image=cv::imread(dum.fileName().toStdString().c_str());
            return TRUE;
        } else {
            qDebug()<<"File did not exist"<<dum.fileName();
        }
    } else {
        return TRUE; // it is not a real error after all
    }
    return FALSE;
}

bool RegexSourceSink::RecordFrame(ImagePacket &)
{
    return FALSE;
}

bool RegexSourceSink::StartRecording(QString, QString, int, int, int)
{
    return FALSE;
}

bool RegexSourceSink::StopRecording()
{
    return FALSE;
}

bool RegexSourceSink::IsOpened()
{
    return TRUE;
}

bool RegexSourceSink::SetInterval(int msec)
{
    return FALSE;
}

bool RegexSourceSink::SetShutter(int)
{
    return FALSE;
}


