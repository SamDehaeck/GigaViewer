#include "fmfbufferedsourcesink.h"
#include "opencv2/opencv.hpp"

bool FmfBufferedSourceSink::Init(QString)
{
    return true;
}

bool FmfBufferedSourceSink::StartAcquisition(QString)
{
    qDebug()<<"Should only write into buffered FMF";
    return true;
}

bool FmfBufferedSourceSink::StopAcquisition()
{
    return true;
}

bool FmfBufferedSourceSink::ReleaseCamera()
{
    return true;
}

bool FmfBufferedSourceSink::GrabFrame(ImagePacket &, int)
{
    return true;
}

bool FmfBufferedSourceSink::RecordFrame(ImagePacket &source)
{
    if (recording) {
        if (source.pixFormat!=dataformat) {
            qDebug()<<"Image pixel format different from written fmf-header: "<<source.pixFormat<<" - "<<dataformat;
            return false;
        }

        if (startTime<-0.99) {
            startTime=source.timeStamp;
        }

        timestamps.push_back((source.timeStamp));
        frames.push_back(source.image); //should I use a clone here???
        //qInfo()<<"Currently there are "<<QString::number(frames.size())<<" in memory";
    } else {
        qDebug()<<"Received a frame while not recording!!";
    }

    return true;
}

QString FmfBufferedSourceSink::StartRecording(QString recFold, QString codec, int, int reccols, int recrows)
{
    uint32_t version = 3;
    uint32_t formatlen;
//    uint32_t bitsperpixel;
//    std::string dataformat;
//    qInfo()<<"Will use buffered fmf recording";

    if ((codec=="FMF") || (codec=="FMF8")) {
        formatlen=5;
        bitsperpixel=8;
        dataformat="MONO8";
    } else if (codec=="FMF12") {
        formatlen=6;
        bitsperpixel=16;
        dataformat="MONO12";
    } else if (codec=="FMF14") {
        formatlen=6;
        bitsperpixel=16;
        dataformat="MONO14";
    } else if (codec=="FMFBAYERRG8") {
        formatlen=8;
        bitsperpixel=8;
        dataformat="BAYERRG8";
    } else if (codec=="FMFBAYERGB8") {
        formatlen=8;
        bitsperpixel=8;
        dataformat="BAYERGB8";
    } else if (codec=="FMFBAYERRG12") {
        formatlen=9;
        bitsperpixel=16;
        dataformat="BAYERRG12";
    } else if (codec=="FMFRGB8") {
        formatlen=4;
        bitsperpixel=8;
        dataformat="RGB8";
    } else {
        return QString("");
    }
    rows=recrows;
    cols=reccols;

    timestamps.clear();
    frames.clear();
    startTime=-1;

    QDir basedir(recFold);
    if (basedir.exists()) {
        QDateTime mom = QDateTime::currentDateTime();
        basename=recFold+"/"+mom.toString("yyyyMMdd-hhmmss");
    } else {
        QRegExp rx("(.+)/(.+)$"); //somepath/somename.someextension
        int pos=0;
        pos=rx.indexIn(recFold);
        if (pos>-1) {
            basename=rx.cap(1)+"/"+rx.cap(2);
        } else {
            qDebug()<<"Recording Folder does not exist";
            QDateTime mom = QDateTime::currentDateTime();
            basename=mom.toString("yyyyMMdd-hhmmss");
        }
//        qInfo()<<"recFold"<<dir<<" basename "<<basename;
    }

    QString filenam=basename+".fmf";



//    ListIndex=0;
//    lastPrinted=0;;
#ifdef Q_OS_WIN32
    fmfrec = fopen(filenam.toStdString().c_str(),"wb");
#else
    fmfrec = fopen(filenam.toUtf8().data(),"wb");
#endif
//    fmfrec = fopen(filenam.toUtf8().data(),"wb");

    if(fwrite(&version,sizeof(uint32_t),1,fmfrec) < 1){
        fprintf(stderr,"Error writing version number of input fmf file.\n");
        return(QString(""));
    }

    // format length
    if(fwrite(&formatlen,sizeof(uint32_t),1,fmfrec)<1){
        fprintf(stderr,"Error writing format length to output fmf file.\n");
        return(QString(""));
    }

    // format string
    if(fwrite(dataformat.toStdString().c_str(),sizeof(char),formatlen,fmfrec)<formatlen){
        fprintf(stderr,"Error writing format string to output fmf file.\n");
        return(QString(""));
    }

    // bits per pixel
    if(fwrite(&bitsperpixel,sizeof(uint32_t),1,fmfrec)<1){
        fprintf(stderr,"Error writing bits per pixel to output fmf file.\n");
        return(QString(""));
    }

//    uint32_t rowsRead;
//    uint32_t colsRead;
      // height of the frame
    if(fwrite(&rows,sizeof(uint32_t),1,fmfrec)<1){
        fprintf(stderr,"Error writing frame height to output fmf file.\n");
        return(QString(""));
    }

    // width of the frame
    if(fwrite(&cols,sizeof(uint32_t),1,fmfrec)<1){
        fprintf(stderr,"Error writing frame width to output fmf file.\n");
        return(QString(""));
    }


    if (dataformat=="RGB8") {
        bytesperchunk=static_cast<ulong>(3*bitsperpixel*static_cast<ulong>(round(rows*cols/8))+sizeof(double));
    } else {
        bytesperchunk=static_cast<ulong>(bitsperpixel*static_cast<ulong>(round(rows*cols/8))+sizeof(double));
    }
    // bytes encoding a frame
    if(fwrite(&bytesperchunk,sizeof(uint64_t),1,fmfrec)<1){
        fprintf(stderr,"Error writing bytes per chunk to output fmf file.\n");
        return(QString(""));
    }
//    qInfo()<<"bytesperchunk will be: "<<bytesperchunk;

    // number of frames
    recNframespos=ftell(fmfrec);

    uint64_t nRead=0; //will have to write this when closing the recording
    if(fwrite(&nRead,sizeof(uint64_t),1,fmfrec)<1){
        fprintf(stderr,"Error writing number of frames to output fmf file.\n");
        return(QString(""));
    }
    recheadersize = ftell(fmfrec);


    recording=true;

    return filenam;
}

bool FmfBufferedSourceSink::StopRecording()
{
    recording=false;
    uint64_t nWritten=static_cast<ulong>(frames.size());
    fseek(fmfrec,recNframespos,SEEK_SET);
    if (fwrite(&nWritten,sizeof(uint64_t),1,fmfrec)<1) qDebug()<<"Error writing number of frames to fmf file";
    //fclose(fmfrec);

    // now write out FMF
    for (int i = 0; i < frames.size(); ++i) {
        //qInfo()<<"Writing frame to file";
        if (fwrite(&timestamps[i],sizeof(double),1,fmfrec)==1) {
            //test here what the bitdepth of the source image is
    //        if (source.image.depth()==2)
            cv::Mat temp=frames.at(i);//.clone();
            if (temp.channels()==3) {
                if (fwrite(temp.data,1,static_cast<size_t>(3*temp.rows*temp.cols),fmfrec)!=static_cast<size_t>(3*temp.rows*temp.cols)) {
                    qWarning()<<"Issue with writing of frame";
                    break;
                }
            } else {
                if (dataformat.contains("8")) {
                    //qInfo()<<"Writing matrix";
                    if (fwrite(temp.data,1,static_cast<size_t>(temp.rows*temp.cols),fmfrec)!=static_cast<size_t>(temp.rows*temp.cols)) {
                        qWarning()<<"Issue with writing of frame";
                        break;
                    }
                } else if ((dataformat.contains("12"))||(dataformat.contains("14"))) {
                    if (fwrite(temp.ptr<uint16_t>(0),2,static_cast<size_t>(temp.rows*temp.cols),fmfrec)!=static_cast<size_t>(temp.rows*temp.cols)) {
                        qWarning()<<"Issue with writing of frame";
                        break;
                    } else {
                        qWarning()<<"Writing 12bit or 14bit frame unsuccessfull";
                        break;
                    }
                } else {
                    qDebug()<<"Problem with dataformat: "<<dataformat;
                    break;
                }
            }
        }

    }
    fclose(fmfrec);
    frames.clear();
    //frames.squeeze();

#ifdef Q_OS_WIN32
    QString filename=basename+"_timestamps.txt";
//    QString filename = "/home/sam/times.txt";
    QFile fileout(filename);
    if (fileout.open(QFile::ReadWrite | QFile::Text| QFile::Truncate)){
     QTextStream out(&fileout);
     for (QVector<double>::iterator iter = timestamps.begin(); iter != timestamps.end(); iter++){
         out << *iter<<"\n";
     }
     fileout.close();
    }
#endif
    return true;
}

bool FmfBufferedSourceSink::IsOpened()
{
    return true;
}

bool FmfBufferedSourceSink::SkipFrames(bool forward) {
    int skipping = 0;
    if (forward) {
        skipping=nFrames/10;
    } else {
        skipping=-nFrames/50;
    }
//    qInfo()<<"Will try to skip "<<skipping<<" frames";

    if ((currPos+skipping >= nFrames-1)||(currPos+skipping <0)) {
        return true;
    }

#ifdef Q_OS_WIN32
    _fseeki64(fmf,(skipping-1)*bytesperchunk,SEEK_CUR);
#else
    fseek(fmf,(skipping-1)*static_cast<long>(bytesperchunk),SEEK_CUR);
#endif
    currPos+=skipping;
    return true;
}

QDataStream &operator<<(QDataStream &out, const cv::Mat &matrix) {
    int totSize=(matrix.rows*matrix.cols);
    if (matrix.channels()==3) {
        totSize=totSize*3;
    }
    //uint bitspp=8;
    for (int i=0;i<totSize;i++) {
        out<<matrix.at<uint8_t>(i);
    }
    return out;
}


