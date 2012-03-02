#include "fmfsourcesink.h"
#include "opencv2/opencv.hpp"

bool FmfSourceSink::Init()
{
    return TRUE;
}

bool FmfSourceSink::StartAcquisition(QString dev)
{
    uint32_t formatlen = 5;
    uint32_t version = 3;
    uint32_t bitsperpixel = 8;
    char dataformat[] = "MONO8";
    int sizeofuint32 = 4;
    int sizeofuint64 = 8;

//    ListIndex=0;
//    lastPrinted=0;

    fmf = fopen(dev.toStdString().c_str(),"rb");

    if(fread(&version,sizeofuint32,1,fmf) < 1){
        fprintf(stderr,"Error reading version number of input fmf file.\n");
        exit(1);
    }

    if (version==3) {
          // format length
          if(fread(&formatlen,sizeofuint32,1,fmf)<1){
            fprintf(stderr,"Error writing format length to output fmf file.\n");
            exit(1);
          }

          // format string
          if(fread(&dataformat,sizeof(char),formatlen,fmf)<formatlen){
            fprintf(stderr,"Error writing format string to output fmf file.\n");
            exit(1);
          }

          // bits per pixel
          if(fread(&bitsperpixel,sizeofuint32,1,fmf)<1){
            fprintf(stderr,"Error writing bits per pixel to output fmf file.\n");
            exit(1);
          }

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

    fseek(fmf,0,SEEK_END);
    nFrames=(ftell(fmf)-headersize)/bytesperchunk;
    if (abs(nFrames)!=nRead) qDebug()<<"Wrong number of frames reported"<<nRead<<"versus calculated"<<nFrames;
    fseek(fmf,headersize,SEEK_SET);
    currPos=0;


//		cout<<rows<<" "<<cols<<" "<<n<<endl;
    return TRUE;
}

bool FmfSourceSink::StopAcquisition()
{
    fclose(fmf);
    return TRUE;
}

bool FmfSourceSink::ReleaseCamera()
{
    return TRUE;
}

bool FmfSourceSink::GrabFrame(ImagePacket &target, int indexIncrement)
{
    if ((currPos+indexIncrement >= nFrames-1)||(currPos+indexIncrement <0)) {
        return TRUE;
    }
    if (indexIncrement!=1) {
                fseek(fmf,(indexIncrement-1)*bytesperchunk,SEEK_CUR);
    }
//    if (index!=-1) {
//        fseek(fmf,headersize+index*bytesperchunk,SEEK_SET); //modify this to use currPos to jump around less
//    }

    if (fread(&target.timeStamp,sizeof(double),1,fmf)) {
        cv::Mat temp = cv::Mat(rows,cols,CV_8U); // normally this implies that the data of temp is continuous
        if (fread(temp.data, 1, rows*cols, fmf)) {
            target.image=temp;
            currPos+=indexIncrement;
            target.seqNumber=currPos;
            return TRUE;
        }
    }

    return FALSE;
}

bool FmfSourceSink::RecordFrame(ImagePacket &source)
{
    if (fwrite(&source.timeStamp,sizeof(double),1,fmfrec)==1) {
        if (source.image.channels()==3) {
            cv::Mat dummy;
            cv::cvtColor(source.image,dummy,CV_RGB2GRAY);
            if (fwrite(dummy.data,1,source.image.rows*source.image.cols,fmfrec)==uint(source.image.rows*source.image.cols)) {
                return TRUE;
            }
        } else {
            if (fwrite(source.image.data,1,source.image.rows*source.image.cols,fmfrec)==uint(source.image.rows*source.image.cols)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

bool FmfSourceSink::StartRecording(QString recFold, QString codec, int, int cols, int rows)
{
    if (codec!="FMF") return FALSE;

    QDateTime mom = QDateTime::currentDateTime();
    QString filenam=recFold+"/"+mom.toString("yyyyMMdd-hhmmss")+".fmf";

    uint32_t formatlen = 5;
    uint32_t version = 3;
    uint32_t bitsperpixel = 8;
    char dataformat[] = "MONO8";
    int sizeofuint32 = 4;
    int sizeofuint64 = 8;

//    ListIndex=0;
//    lastPrinted=0;

    fmfrec = fopen(filenam.toStdString().c_str(),"wb");

    if(fwrite(&version,sizeofuint32,1,fmfrec) < 1){
        fprintf(stderr,"Error reading version number of input fmf file.\n");
        exit(1);
    }

    // format length
    if(fwrite(&formatlen,sizeofuint32,1,fmfrec)<1){
        fprintf(stderr,"Error writing format length to output fmf file.\n");
        exit(1);
    }

    // format string
    if(fwrite(&dataformat,sizeof(char),formatlen,fmfrec)<formatlen){
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

    bytesperchunk=bitsperpixel*rows*cols/8+sizeof(double);
    // bytes encoding a frame
    if(fwrite(&bytesperchunk,sizeofuint64,1,fmfrec)<1){
        fprintf(stderr,"Error writing bytes per chunk to output fmf file.\n");
        exit(1);
    }

    // number of frames
    recNframespos=ftell(fmfrec);

    uint64_t nRead=0; //will have to write this when closing the recording
    if(fwrite(&nRead,sizeofuint64,1,fmfrec)<1){
        fprintf(stderr,"Error writing number of frames to output fmf file.\n");
        exit(1);
    }
    recheadersize = ftell(fmfrec);

    return TRUE;
}

bool FmfSourceSink::StopRecording()
{
    int sizeofuint64 = 8;
    uint64_t nWritten=(ftell(fmfrec)-recheadersize)/bytesperchunk;
    fseek(fmfrec,recNframespos,SEEK_SET);
    if (fwrite(&nWritten,sizeofuint64,1,fmfrec)<1) qDebug()<<"Error writing number of frames to fmf file";
    fclose(fmfrec);
    return TRUE;
}

bool FmfSourceSink::IsOpened()
{
    return TRUE;
}

bool FmfSourceSink::SetInterval(int)
{
    return FALSE;
}

bool FmfSourceSink::SetShutter(int)
{
    return FALSE;
}
