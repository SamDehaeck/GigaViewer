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
    currPos=0;
    nFrames=nRead;

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
//    if ((currPos+indexIncrement > nFrames)||(currPos+indexIncrement <0)) {
    if (currPos+indexIncrement <0) {
        return FALSE;
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
    return FALSE;
}

bool FmfSourceSink::StartRecording(QString recFold, QString codec, int fps, int cols, int rows)
{

    return FALSE;
}

bool FmfSourceSink::StopRecording()
{
    return TRUE;
}

bool FmfSourceSink::IsOpened()
{
    return TRUE;
}
