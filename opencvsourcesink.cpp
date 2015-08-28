#include "opencvsourcesink.h"

bool OpencvSourceSink::Init() {
    return true;
}

bool OpencvSourceSink::StartAcquisition(QString dev) {
    if (dev=="0") {
        camera.open(0);
        nFrames=0;
        liveFeed=true;
    } else {
#ifdef Q_OS_WIN32
        camera.open(dev.toStdString().c_str());
#else
        camera.open(dev.toUtf8().data());
#endif
        nFrames=camera.get(CV_CAP_PROP_FRAME_COUNT);
        liveFeed=false;
    }
    return camera.isOpened();
}

bool OpencvSourceSink::StartRecording(QString recFold, QString codec, int fps,int cols,int rows) {
    QDateTime mom = QDateTime::currentDateTime();
    QString filenam=recFold+"/"+mom.toString("yyyyMMdd-hhmmss")+".avi";
    int fourcc=0;
    if (codec=="MSMPEG4V2") {
        fourcc=CV_FOURCC('M','P','4','2'); // for mpeg4 from windows
    } else if (codec=="XVID") {
        fourcc=CV_FOURCC('F','M','P','4'); //for xvid
    } else {
        fourcc=0;// uncompressed raw format
    }
#ifdef Q_OS_WIN32
    recFile=cv::VideoWriter(filenam.toStdString().c_str(),fourcc,fps,cv::Size(cols,rows));
#else
    recFile=cv::VideoWriter(filenam.toUtf8().data(),fourcc,fps,cv::Size(cols,rows));
#endif
    return true;
}

bool OpencvSourceSink::StopRecording() {
    //nothing to do
    return true;
}

bool OpencvSourceSink::StopAcquisition() {
    //nothing specific to do, the thread will be quited in thread class
    return true;
}

bool OpencvSourceSink::ReleaseCamera() {
    if (camera.isOpened()) camera.release();
    return true;
}

bool OpencvSourceSink::RecordFrame(ImagePacket &source) {
    if (source.image.channels()==1) {
        cv::Mat dummy;
        cv::cvtColor(source.image,dummy,CV_GRAY2RGB);
        recFile<<dummy;
    } else {
        recFile<<source.image;
    }

    return true;
}

bool OpencvSourceSink::GrabFrame(ImagePacket &target,int indexIncrement) {
    if (!camera.isOpened()) return false;
    if (indexIncrement==-1) {
        double newpos=camera.get(CV_CAP_PROP_POS_FRAMES)-2;
//        qDebug()<<"calculated:"<<newpos;
        if (newpos>=0) {
            camera.set(CV_CAP_PROP_POS_FRAMES,newpos);
        }
    }
//    qDebug()<<camera.get(CV_CAP_PROP_POS_FRAMES);

    if (!liveFeed) {
        target.seqNumber=camera.get(CV_CAP_PROP_POS_FRAMES);
    } else {
        target.seqNumber=-1;
    }
    camera >> target.image;
    if (target.image.channels()==3) {
        target.pixFormat="RGB8";
    } else {
        target.pixFormat="MONO8";
    }
    target.timeStamp=QDateTime::currentMSecsSinceEpoch();


    return true;
}

bool OpencvSourceSink::IsOpened() {
    return camera.isOpened();
}

bool OpencvSourceSink::SkipFrames(bool forward)
{
//    qDebug()<<"Number of frames"<<nFrames;
    if (!liveFeed) {
        int currPos=camera.get(CV_CAP_PROP_POS_FRAMES);
        int skipping = 0;
        if (forward) {
            skipping=nFrames/10;
        } else {
            skipping=-nFrames/50;
        }
    //    qDebug()<<"Will try to skip "<<skipping<<" frames";
        if (skipping==0) return true; //skipping did not work

        if ((currPos+skipping >= nFrames-1)||(currPos+skipping <0)) {
            return true;
        }


        camera.set(CV_CAP_PROP_POS_FRAMES,currPos+skipping);
        currPos+=skipping;
    }
    return true;


}

