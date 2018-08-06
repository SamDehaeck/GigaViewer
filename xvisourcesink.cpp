#include "xvisourcesink.h"
#include "opencv2/opencv.hpp"

bool XviSourceSink::Init()
{
    return true;
}

bool XviSourceSink::StartAcquisition(QString dev)
{

#ifdef Q_OS_WIN32
    xvi = fopen(dev.toStdString().c_str(),"rb");
#else
    xvi = fopen(dev.toUtf8().data(),"rb");
#endif
    uint firstNums[10];
    if (fread(&firstNums,sizeof(uint32_t),10,xvi) < 1) {
        fprintf(stderr,"Error reading first part of input xvi file.\n");
        return false;
    }
    rows=static_cast<int>(firstNums[5]);
    cols=static_cast<int>(firstNums[4]);
    headersize=firstNums[7];
    bytesperchunk=firstNums[2]; //including footer
    bitsperpixel=2;
    dataformat="FLOAT";
    footersize=bytesperchunk-firstNums[9];

#ifdef Q_OS_WIN32
    _fseeki64(xvi,0,SEEK_END);
    nFrames=(_ftelli64(xvi)-headersize)/bytesperchunk;
#else
    fseek(xvi,0,SEEK_END);
    nFrames=(static_cast<ulong>(ftell(xvi))-headersize)/bytesperchunk;
#endif


    fseek(xvi,static_cast<long>(headersize),SEEK_SET);
    currPos=0;
//    qDebug()<<"found a lot of frames: "<<nFrames;
    return true;
}

bool XviSourceSink::StopAcquisition()
{
    fclose(xvi);
    return true;
}

bool XviSourceSink::ReleaseCamera()
{
    return true;
}

bool XviSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if ((currPos+indexIncrement >= static_cast<int>(nFrames-1))||(currPos+indexIncrement <0)) {
        return true;
    }
    if (indexIncrement!=1) {
#ifdef Q_OS_WIN32
                _fseeki64(xvi,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
#else
                fseek(xvi,(indexIncrement-1)*static_cast<long>(bytesperchunk),SEEK_CUR);
#endif
    }

    cv::Mat temp = cv::Mat(rows,cols,CV_16U); // normally this implies that the data of temp is continuous
    size_t amread=fread(temp.ptr<uint16_t>(0), 2, static_cast<size_t>(rows*cols), xvi);
    if (amread==static_cast<size_t>(rows*cols)) {
        cv::Mat temp2=cv::Mat(rows,cols,CV_32F);
        temp2=temp.mul(1.0);
        target.image=temp2.clone();

    }
    char dummy[12];
    amread=fread(dummy,1,12,xvi);
    int64 time;
    amread=fread(&time,8,1,xvi);

#ifdef Q_OS_WIN32
                _fseeki64(xvi,footersize-20,SEEK_CUR);
#else
                fseek(xvi,static_cast<long>(footersize)-20,SEEK_CUR);
#endif

    currPos+=indexIncrement;
    target.seqNumber=currPos;
    target.pixFormat=dataformat;
    target.timeStamp=time/1000.0;
    return true;

    return false;
}

bool XviSourceSink::IsOpened()
{
    return true;
}

bool XviSourceSink::SkipFrames(bool forward) {
    int skipping = 0;
    if (forward) {
        skipping=static_cast<int>(round(nFrames/10));
    } else {
        skipping=-static_cast<int>(round(nFrames/50));
    }
//    qDebug()<<"Will try to skip "<<skipping<<" frames";

    if ((currPos+skipping >= static_cast<int>(nFrames)-1)||(currPos+skipping <0)) {
        return true;
    }


#ifdef Q_OS_WIN32
    _fseeki64(xvi,(skipping-1)*bytesperchunk,SEEK_CUR);
#else
    fseek(xvi,(skipping-1)*static_cast<long>(bytesperchunk),SEEK_CUR);
#endif
    currPos+=skipping;
    return true;
}
