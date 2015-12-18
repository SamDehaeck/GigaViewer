#include "xvisourcesink.h"
#include "opencv2/opencv.hpp"

bool XviSourceSink::Init()
{
    return true;
}

bool XviSourceSink::StartAcquisition(QString dev)
{
//    uint32_t formatlen = 5;
//    uint32_t version = 3;
//    char dataf[15];
    int sizeofuint32 = 4;
    int sizeofuint64 = 8;

//    ListIndex=0;
//    lastPrinted=0;

//    char* thename=dev.toUtf8().data();
//    const char* thename=dev.toStdString().c_str();
//    qDebug()<<"Wil read: "<<dev<<" => "<<thename<<" or "<<dev.toStdString().c_str();
//    fmf = fopen(thename,"rb");
#ifdef Q_OS_WIN32
    xvi = fopen(dev.toStdString().c_str(),"rb");
#else
    xvi = fopen(dev.toUtf8().data(),"rb");
#endif
    uint firstNums[10];
    if (fread(&firstNums,sizeofuint32,10,xvi) < 1) {
        fprintf(stderr,"Error reading first part of input xvi file.\n");
        return false;
    }
    rows=firstNums[5];
    cols=firstNums[4];
    headersize=firstNums[7];
    bytesperchunk=firstNums[2]; //including footer
    bitsperpixel=2;
    dataformat="FLOAT";
    footersize=bytesperchunk-firstNums[9];

    fseek(xvi,0,SEEK_END);
    nFrames=(ftell(xvi)-headersize)/bytesperchunk;
    fseek(xvi,headersize,SEEK_SET);
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
    if ((currPos+indexIncrement >= nFrames-1)||(currPos+indexIncrement <0)) {
        return true;
    }
    if (indexIncrement!=1) {
                fseek(xvi,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
    }

    cv::Mat temp = cv::Mat(rows,cols,CV_16U); // normally this implies that the data of temp is continuous
    uint amread=fread(temp.ptr<uint16_t>(0), 2, rows*cols, xvi);
    if (amread==(uint)(rows*cols)) {
        cv::Mat temp2=cv::Mat(rows,cols,CV_32F);
        temp2=temp.mul(1.0);
        target.image=temp2.clone();

    }
    char dummy[12];
    amread=fread(dummy,1,12,xvi);
    int64 time;
    amread=fread(&time,8,1,xvi);


    fseek(xvi,footersize-20,SEEK_CUR);

    currPos+=indexIncrement;
    target.seqNumber=currPos;
    target.pixFormat=dataformat;
    target.timeStamp=time/1000.0;
    return true;

/*
    if (fread(&target.timeStamp,sizeof(double),1,xvi)) {
        // change CV_8U to instance variable datatype
        cv::Mat temp;
        if (dataformat!="RGB8") {
            if (bitsperpixel==8) {
                temp = cv::Mat(rows,cols,CV_8U); // normally this implies that the data of temp is continuous
                uint amread=fread(temp.data, 1, rows*cols, xvi);
                if (amread==(uint)(rows*cols)) target.image=temp.clone();
            } else {
                temp = cv::Mat(rows,cols,CV_16U); // normally this implies that the data of temp is continuous
                uint amread=fread(temp.ptr<uint16_t>(0), 2, rows*cols, xvi);
                if (amread==(uint)(rows*cols)) target.image=temp.clone();
            }
        } else {
            temp = cv::Mat(rows,cols,CV_8UC3); // normally this implies that the data of temp is continuous
            uint amread=fread(temp.data, 1, 3*rows*cols, xvi);
            if (amread==(uint)(3*rows*cols)) target.image=temp.clone();
        }

        currPos+=indexIncrement;
        target.seqNumber=currPos;
        target.pixFormat=dataformat;
        return true;

    }
*/
//    return false;
}

bool XviSourceSink::IsOpened()
{
    return true;
}

bool XviSourceSink::SkipFrames(bool forward) {
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


    fseek(xvi,(skipping-1)*bytesperchunk,SEEK_CUR);
    currPos+=skipping;
    return true;
}
