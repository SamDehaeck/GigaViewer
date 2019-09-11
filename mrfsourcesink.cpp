#include "mrfsourcesink.h"
#include "opencv2/opencv.hpp"

bool MrfSourceSink::Init(QString)
{
    return true;
}

bool MrfSourceSink::StartAcquisition(QString dev)
{
#ifdef Q_OS_WIN32
    mrf = fopen(dev.toStdString().c_str(),"rb");
#else
    mrf = fopen(dev.toUtf8().data(),"rb");
#endif
    uint firstNums[8];
    if (fread(&firstNums,sizeof(uint32_t),8,mrf) < 1) {
        fprintf(stderr,"Error reading first part of input Mrf file.\n");
        return false;
    }

    nFrames=firstNums[4];
    rows=static_cast<int>(firstNums[6]);
    cols=static_cast<int>(firstNums[5]);
    bitsperpixel=firstNums[7];
    if (bitsperpixel==8) {
        dataformat="MONO8";
        bytesperchunk=static_cast<ulong>(rows*cols);
    } else {
        dataformat="MONO16";
        bytesperchunk=static_cast<ulong>(2*rows*cols);
    }
    uint userdat=firstNums[3]-20;

#ifdef Q_OS_WIN32
    _fseeki64(mrf,userdat,SEEK_CUR);
    headersize=_ftelli64(mrf);
#else
    fseek(mrf,userdat,SEEK_CUR);
    headersize=static_cast<ulong>(ftell(mrf));
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
    if ((currPos+indexIncrement >= static_cast<int>(nFrames)-1)||(currPos+indexIncrement <0)) {
        return true;
    }
    if (indexIncrement!=1) {
#ifdef Q_OS_WIN32
                _fseeki64(mrf,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
#else
                fseek(mrf,(indexIncrement-1)*static_cast<long>(bytesperchunk),SEEK_CUR);
#endif
    }

    cv::Mat temp;
    if (bitsperpixel==8) {
        temp = cv::Mat(rows,cols,CV_8U); // normally this implies that the data of temp is continuous
        size_t amread=fread(temp.data, 1, static_cast<size_t>(rows*cols), mrf);
        if (amread==static_cast<size_t>(rows*cols)) target.image=temp.clone();
    } else {
        temp = cv::Mat(rows,cols,CV_16U); // normally this implies that the data of temp is continuous
        size_t amread=fread(temp.ptr<uint16_t>(0), 2, static_cast<size_t>(rows*cols), mrf);
        if (amread==static_cast<size_t>(rows*cols)) target.image=temp.clone();
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

    if ((currPos+skipping >= static_cast<int>(nFrames)-1)||(currPos+skipping <0)) {
        return true;
    }


#ifdef Q_OS_WIN32
    _fseeki64(mrf,(skipping-1)*bytesperchunk,SEEK_CUR);
#else
    fseek(mrf,(skipping-1)*static_cast<long>(bytesperchunk),SEEK_CUR);
#endif
    currPos+=skipping;
    return true;
}
