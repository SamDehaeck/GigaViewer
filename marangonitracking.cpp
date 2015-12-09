#include "marangonitracking.h"


using namespace cv;

MarangoniTracking::MarangoniTracking(int thresh,int nrParticles) : threshold(thresh),nrParts(nrParticles),activated(false),shouldTrack(false),mirCtrl(){

}

Point MarangoniTracking::FindParticle(Mat image) {
    return Point(200,300);
}

void MarangoniTracking::ChangeSettings(QMap<QString,QVariant> settings) {
    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();
    if ((!activated)&&(settings["activated"].toBool())) {
        qDebug()<<"Should initialise";

//         mirCtrl.drawCircleAt(targetX,targetX);
    }
    if (activated&&(!settings["activated"].toBool())) {
        qDebug()<<"Should write to disk";
    }

    activated=settings["activated"].toBool();
    shouldTrack=settings["shouldTrack"].toBool();

//    qDebug()<<"threshold="<<threshold;

}

bool MarangoniTracking::processImage(ImagePacket& currIm) {
    if (activated) {


        if (currIm.pixFormat=="RGB8") {
            cv::Mat grayIm;
            cv::cvtColor(currIm.image,grayIm,CV_RGB2GRAY);
            currIm.image=grayIm.clone();
            currIm.pixFormat="MONO8";
        }
        if (currIm.pixFormat=="MONO8") {
            cv::Mat processed;
            int newT=threshold*255/100.0;  //change here if it needs to work for 16-bit
            cv::threshold ( currIm.image, processed, newT, 255, THRESH_BINARY_INV );

            std::vector<std::vector<cv::Point> > contours;
            cv::Mat hierarchy;
            cv::findContours( processed, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

            cv::Mat outImage=currIm.image.clone(); // use a clone to avoid storing on raw image which is stored.
            cv::drawContours(outImage,contours,-1,255,2);

            cv::Point newLoc(targetX*currIm.image.cols/100.0,targetY*currIm.image.rows/100.0);
            cv::ellipse(outImage,newLoc,cv::Size(5,5),0,0,360,150,-1);

            currIm.image=outImage;

            // now make string to save
            //QString outst(""
//            outst=""+
//            currIm.timeStamp+" "+threshold+" "+targetX+""
//            dataToSave.append(outst);


        }
    }
    return true;
}

