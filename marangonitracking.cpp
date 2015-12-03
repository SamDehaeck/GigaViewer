#include "marangonitracking.h"


using namespace cv;

MarangoniTracking::MarangoniTracking(int thresh,int nrParticles) : threshold(thresh),nrParts(nrParticles),activated(false),shouldTrack(false){

}

Point MarangoniTracking::FindParticle(Mat image) {
    return Point(200,300);
}

void MarangoniTracking::ChangeSettings(QMap<QString,QVariant> settings) {
    bool ok;
    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();
    activated=settings["activated"].toBool();
    shouldTrack=settings["shouldTrack"].toBool();
//    qDebug()<<"threshold="<<threshold;
}

bool MarangoniTracking::processImage(ImagePacket& currIm) {
    if (activated) {

        cv::Mat processed;
        int newT=threshold*255/100.0;  //change here if it needs to work for 16-bit
        cv::threshold ( currIm.image, processed, newT, 255, THRESH_BINARY_INV );

        std::vector<std::vector<cv::Point> > contours;
        cv::Mat hierarchy;
        cv::findContours( processed, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        cv::drawContours(currIm.image,contours,-1,255,2);






//        currIm.image=processed;

        cv::Point newLoc(targetX*currIm.image.cols/100.0,targetY*currIm.image.rows/100.0);
        cv::ellipse(currIm.image,newLoc,cv::Size(5,5),0,0,360,150,-1);
    }
    return true;
}

