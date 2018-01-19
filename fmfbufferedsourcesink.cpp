#include "fmfbufferedsourcesink.h"
#include "opencv2/opencv.hpp"

bool FmfBufferedSourceSink::Init()
{
    return true;
}

bool FmfBufferedSourceSink::StartAcquisition(QString dev)
{
    uint32_t formatlen = 5;
    uint32_t version = 3;
    char dataf[15];
    int sizeofuint32 = 4;
    int sizeofuint64 = 8;

//    ListIndex=0;
//    lastPrinted=0;

//    char* thename=dev.toUtf8().data();
//    const char* thename=dev.toStdString().c_str();
//    qInfo()<<"Wil read: "<<dev<<" => "<<thename<<" or "<<dev.toStdString().c_str();
//    fmf = fopen(thename,"rb");
#ifdef Q_OS_WIN32
    fmf = fopen(dev.toStdString().c_str(),"rb");
#else
    fmf = fopen(dev.toUtf8().data(),"rb");
#endif
    if(fread(&version,sizeofuint32,1,fmf) < 1){
        fprintf(stderr,"Error reading version number of input fmf file.\n");
        exit(1);
    }

    if (version==3) {
          // format length
          if(fread(&formatlen,sizeofuint32,1,fmf)<1){
            fprintf(stderr,"Error reading format length to output fmf file.\n");
            exit(1);
          }

          // format string
          if(fread(dataf,sizeof(char),formatlen,fmf)<formatlen){
            fprintf(stderr,"Error reading format string to output fmf file.\n");
            exit(1);
          }
          dataf[formatlen]='\0';
          dataformat=dataf;
          if (dataformat=="MONO8") {
//              qInfo()<<"Recognised Mono8 FMF-file";
          } else if (dataformat=="MONO12") {
//              qInfo()<<"Recognised Mono12 FMF-file";
          } else if (dataformat=="MONO14") {
//              qInfo()<<"Recognised Mono14 FMF-file";
          } else if (dataformat=="BAYERRG8") {
//              qInfo()<<"Recognised BayerRG8 FMF-file";
          } else if (dataformat=="BAYERRG12") {
              qInfo()<<"Recognised BayerRG12 FMF-file";
          } else if (dataformat=="BAYERGB8") {
//              qInfo()<<"Recognised BayerGB8 FMF-file";
          } else if (dataformat=="RGB8") {
              qInfo()<<"Recognised RGB8 FMF-file";
          } else {
              qInfo()<<"Unrecognised format "<<dataformat;
          }
//          std::string dataf(dataformat);


          // bits per pixel
          if(fread(&bitsperpixel,sizeofuint32,1,fmf)<1){
            fprintf(stderr,"Error reading bits per pixel to output fmf file.\n");
            exit(1);
          }
//          qInfo()<<"Formatlen, dataformat, bitsperpixel"<<formatlen<<", "<<dataformat<<", "<<bitsperpixel;

    }

    uint32_t rowsRead;
    uint32_t colsRead;
      // height of the frame
    if(fread(&rowsRead,sizeofuint32,1,fmf)<1){
        fprintf(stderr,"Error writing frame height to output fmf file.\n");
        exit(1);
    }
    rows=rowsRead;

    // width of the frame
    if(fread(&colsRead,sizeofuint32,1,fmf)<1){
        fprintf(stderr,"Error writing frame width to output fmf file.\n");
        exit(1);
    }
    cols=colsRead;

    // bytes encoding a frame
    if(fread(&bytesperchunk,sizeofuint64,1,fmf)<1){
        fprintf(stderr,"Error writing bytes per chunk to output fmf file.\n");
        exit(1);
    }

    // number of frames
    uint64_t nRead;
    if(fread(&nRead,sizeofuint64,1,fmf)<1){
        fprintf(stderr,"Error writing number of frames to output fmf file.\n");
        exit(1);
    }
    headersize = ftell(fmf);
//    qInfo()<<"Header size of fmf is: "<<headersize;

#ifdef Q_OS_WIN32
    _fseeki64(fmf,0,SEEK_END);
    nFrames=(_ftelli64(fmf)-headersize)/bytesperchunk;
#else
    fseek(fmf,0,SEEK_END);
    nFrames=(ftell(fmf)-headersize)/bytesperchunk;
#endif
    if (abs(nFrames)!=int(nRead)) qInfo()<<"Wrong number of frames reported"<<nRead<<"versus calculated"<<nFrames;
    fseek(fmf,headersize,SEEK_SET);
    currPos=0;


//    qInfo()<<"Found: "<<bytesperchunk<<" - "<<nRead<<" - "<<nFrames<<" - "<<bitsperpixel;
    return true;
}

bool FmfBufferedSourceSink::StopAcquisition()
{
    fclose(fmf);
    return true;
}

bool FmfBufferedSourceSink::ReleaseCamera()
{
    return true;
}

bool FmfBufferedSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if ((currPos+indexIncrement >= nFrames-1)||(currPos+indexIncrement <0)) {
        return true;
    }
    if (indexIncrement!=1) {
#ifdef Q_OS_WIN32
                _fseeki64(fmf,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
#else
                fseek(fmf,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
#endif
    }
//    if (index!=-1) {
//        fseek(fmf,headersize+index*bytesperchunk,SEEK_SET); //modify this to use currPos to jump around less
//    }

    if (fread(&target.timeStamp,sizeof(double),1,fmf)) {
        // change CV_8U to instance variable datatype
        cv::Mat temp;
        if (dataformat!="RGB8") {
            if (bitsperpixel==8) {
                temp = cv::Mat(rows,cols,CV_8U); // normally this implies that the data of temp is continuous
                uint amread=fread(temp.data, 1, rows*cols, fmf);
                if (amread==(uint)(rows*cols)) target.image=temp.clone();
            } else {
                temp = cv::Mat(rows,cols,CV_16U); // normally this implies that the data of temp is continuous
                uint amread=fread(temp.ptr<uint16_t>(0), 2, rows*cols, fmf);
                if (amread==(uint)(rows*cols)) target.image=temp.clone();
            }
        } else {
            temp = cv::Mat(rows,cols,CV_8UC3); // normally this implies that the data of temp is continuous
            uint amread=fread(temp.data, 1, 3*rows*cols, fmf);
            if (amread==(uint)(3*rows*cols)) target.image=temp.clone();
        }

        currPos+=indexIncrement;
        target.seqNumber=currPos;
        target.pixFormat=dataformat;
        return true;

    }

    return false;
}

bool FmfBufferedSourceSink::RecordFrame(ImagePacket &source)
{
    if (recording) {
        if (source.pixFormat!=dataformat) {
            qInfo()<<"Image pixel format different from written fmf-header: "<<source.pixFormat<<" - "<<dataformat;
            return false;
        }

        if (startTime==-1) {
            startTime=source.timeStamp;
        }

        timestamps.push_back((source.timeStamp-startTime));
        frames.push_back(source.image); //should I use a clone here???
        //qInfo()<<"Currently there are "<<QString::number(frames.size())<<" in memory";
    } else {
        qInfo()<<"Received a frame while not recording!!";
    }

    return true;
}

bool FmfBufferedSourceSink::StartRecording(QString recFold, QString codec, int, int reccols, int recrows)
{
    uint32_t version = 3;
    int sizeofuint32 = 4;
    int sizeofuint64 = 8;
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
        return false;
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
            qInfo()<<"Recording Folder does not exist";
            QDateTime mom = QDateTime::currentDateTime();
            basename=recFold+"/"+mom.toString("yyyyMMdd-hhmmss");
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

    if(fwrite(&version,sizeofuint32,1,fmfrec) < 1){
        fprintf(stderr,"Error writing version number of input fmf file.\n");
        exit(1);
    }

    // format length
    if(fwrite(&formatlen,sizeofuint32,1,fmfrec)<1){
        fprintf(stderr,"Error writing format length to output fmf file.\n");
        exit(1);
    }

    // format string
    if(fwrite(dataformat.toStdString().c_str(),sizeof(char),formatlen,fmfrec)<formatlen){
        fprintf(stderr,"Error writing format string to output fmf file.\n");
        exit(1);
    }

    // bits per pixel
    if(fwrite(&bitsperpixel,sizeofuint32,1,fmfrec)<1){
        fprintf(stderr,"Error writing bits per pixel to output fmf file.\n");
        exit(1);
    }

//    uint32_t rowsRead;
//    uint32_t colsRead;
      // height of the frame
    if(fwrite(&rows,sizeofuint32,1,fmfrec)<1){
        fprintf(stderr,"Error writing frame height to output fmf file.\n");
        exit(1);
    }

    // width of the frame
    if(fwrite(&cols,sizeofuint32,1,fmfrec)<1){
        fprintf(stderr,"Error writing frame width to output fmf file.\n");
        exit(1);
    }


    if (dataformat=="RGB8") {
        bytesperchunk=3*bitsperpixel*rows*cols/8+sizeof(double);
    } else {
        bytesperchunk=bitsperpixel*rows*cols/8+sizeof(double);
    }
    // bytes encoding a frame
    if(fwrite(&bytesperchunk,sizeofuint64,1,fmfrec)<1){
        fprintf(stderr,"Error writing bytes per chunk to output fmf file.\n");
        exit(1);
    }
//    qInfo()<<"bytesperchunk will be: "<<bytesperchunk;

    // number of frames
    recNframespos=ftell(fmfrec);

    uint64_t nRead=0; //will have to write this when closing the recording
    if(fwrite(&nRead,sizeofuint64,1,fmfrec)<1){
        fprintf(stderr,"Error writing number of frames to output fmf file.\n");
        exit(1);
    }
    recheadersize = ftell(fmfrec);


    recording=true;

    return true;
}

bool FmfBufferedSourceSink::StopRecording()
{
    recording=false;
    int sizeofuint64 = 8;
    uint64_t nWritten=frames.size();
    fseek(fmfrec,recNframespos,SEEK_SET);
    if (fwrite(&nWritten,sizeofuint64,1,fmfrec)<1) qInfo()<<"Error writing number of frames to fmf file";
    //fclose(fmfrec);

    // now write out FMF
    for (int i = 0; i < frames.size(); ++i) {
        //qInfo()<<"Writing frame to file";
        if (fwrite(&timestamps[i],sizeof(double),1,fmfrec)==1) {
            //test here what the bitdepth of the source image is
    //        if (source.image.depth()==2)
            cv::Mat temp=frames.at(i);//.clone();
            if (temp.channels()==3) {
                if (fwrite(temp.data,1,3*temp.rows*temp.cols,fmfrec)!=uint(3*temp.rows*temp.cols)) {
                    qWarning()<<"Issue with writing of frame";
                    break;
                }
            } else {
                if (dataformat.contains("8")) {
                    //qInfo()<<"Writing matrix";
                    if (fwrite(temp.data,1,temp.rows*temp.cols,fmfrec)!=uint(temp.rows*temp.cols)) {
                        qWarning()<<"Issue with writing of frame";
                        break;
                    }
                } else if ((dataformat.contains("12"))||(dataformat.contains("14"))) {
                    if (fwrite(temp.ptr<uint16_t>(0),2,temp.rows*temp.cols,fmfrec)!=uint(temp.rows*temp.cols)) {
                        qWarning()<<"Issue with writing of frame";
                        break;
                    } else {
                        qWarning()<<"Writing 12bit or 14bit frame unsuccessfull";
                        break;
                    }
                } else {
                    qInfo()<<"Problem with dataformat: "<<dataformat;
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
    fseek(fmf,(skipping-1)*bytesperchunk,SEEK_CUR);
#endif
    currPos+=skipping;
    return true;
}

QDataStream &operator<<(QDataStream &out, const cv::Mat &matrix) {
    uint totSize=matrix.rows*matrix.cols;
    if (matrix.channels()==3) {
        totSize=totSize*3;
    }
    //uint bitspp=8;
    for (uint i=0;i<totSize;i++) {
        out<<matrix.at<uint8_t>(i);
    }
    return out;
}


