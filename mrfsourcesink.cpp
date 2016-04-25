#include "mrfsourcesink.h"
#include "opencv2/opencv.hpp"

bool MrfSourceSink::Init()
{
    return true;
}

bool MrfSourceSink::StartAcquisition(QString dev)
{
//    uint32_t formatlen = 5;
//    uint32_t version = 3;
//    char dataf[15];
    int sizeofuint32 = 4;
//    int sizeofuint64 = 8;

//    ListIndex=0;
//    lastPrinted=0;

//    char* thename=dev.toUtf8().data();
//    const char* thename=dev.toStdString().c_str();
//    qDebug()<<"Wil read: "<<dev<<" => "<<thename<<" or "<<dev.toStdString().c_str();
//    fmf = fopen(thename,"rb");
#ifdef Q_OS_WIN32
    mrf = fopen(dev.toStdString().c_str(),"rb");
#else
    mrf = fopen(dev.toUtf8().data(),"rb");
#endif
    uint firstNums[8];
    if (fread(&firstNums,sizeofuint32,8,mrf) < 1) {
        fprintf(stderr,"Error reading first part of input Mrf file.\n");
        return false;
    }

    nFrames=firstNums[4];
    rows=firstNums[6];
    cols=firstNums[5];
    bitsperpixel=firstNums[7];
    if (bitsperpixel==8) {
        dataformat="MONO8";
        bytesperchunk=rows*cols;
    } else {
        dataformat="MONO16";
        bytesperchunk=2*rows*cols;
    }
    uint userdat=firstNums[3]-20;

#ifdef Q_OS_WIN32
    _fseeki64(mrf,userdat,SEEK_CUR);
    headersize=_ftelli64(mrf);
#else
    fseek(mrf,userdat,SEEK_CUR);
    headersize=ftell(mrf);
#endif



    currPos=0;
//    qDebug()<<"found a lot of frames: "<<nFrames<<" of "<<rows<<"x"<<cols;
    return true;
}

bool MrfSourceSink::StopAcquisition()
{
    fclose(mrf);
    return true;
}

bool MrfSourceSink::ReleaseCamera()
{
    return true;
}

bool MrfSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if ((currPos+indexIncrement >= nFrames-1)||(currPos+indexIncrement <0)) {
        return true;
    }
    if (indexIncrement!=1) {
#ifdef Q_OS_WIN32
                _fseeki64(mrf,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
#else
                fseek(mrf,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
#endif
    }

    cv::Mat temp;
    if (bitsperpixel==8) {
        temp = cv::Mat(rows,cols,CV_8U); // normally this implies that the data of temp is continuous
        uint amread=fread(temp.data, 1, rows*cols, mrf);
        if (amread==(uint)(rows*cols)) target.image=temp.clone();
    } else {
        temp = cv::Mat(rows,cols,CV_16U); // normally this implies that the data of temp is continuous
        uint amread=fread(temp.ptr<uint16_t>(0), 2, rows*cols, mrf);
        if (amread==(uint)(rows*cols)) target.image=temp.clone();
    }



    currPos+=indexIncrement;
    target.seqNumber=currPos;
    target.pixFormat=dataformat;
    target.timeStamp=0;
    return true;

}

bool MrfSourceSink::IsOpened()
{
    return true;
}

bool MrfSourceSink::SkipFrames(bool forward) {
    int skipping = 0;
    if (forward) {
        skipping=nFrames/10;
    } else {
        skipping=-nFrames/50;
    }
//    qDebug()<<"Will try to skip "<<skipping<<" frames";

    if ((currPos+skipping >= nFrames-1)||(currPos+skipping <0)) {
        return true;
    }


#ifdef Q_OS_WIN32
    _fseeki64(mrf,(skipping-1)*bytesperchunk,SEEK_CUR);
#else
    fseek(mrf,(skipping-1)*bytesperchunk,SEEK_CUR);
#endif
    currPos+=skipping;
    return true;
}
