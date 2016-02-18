#include "marangonitracking.h"

using namespace cv;

MarangoniTracking::MarangoniTracking(int thresh,int nrParticles) : threshold(thresh),nrParts(nrParticles),activated(false),shouldTrack(false)
#ifdef Q_OS_WIN32
  ,myRegulator()
#endif
{
    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
    center = -0.75;
    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
}

Point MarangoniTracking::FindParticle(Mat image) {
    return Point(200,300);
}

void MarangoniTracking::ChangeSettings(QMap<QString,QVariant> settings) {
    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();

    if ((!activated)&&(settings["activated"].toBool())) {                   //Activation of the tracking
        myRegulator.Initialisation();                                       //Initialisation of the mirror
        if (myRegulator.properConnection == false){                         //Verification that the connection with the MEMs is ok
            qDebug()<<"Problem with the MEM conection - Closing the program";
            //exit(0);                                                        //...if no proper connection, exits the programme
        }
        qDebug()<<"Activation of the tracking";
        myRegulator.Figure(1, targetX, targetY);                           //Creation of the figure. For the future, need a button to choose which type of figure is used
    }
    if (activated&&(!settings["activated"].toBool())) {//Desactivation of the tracking
        qDebug()<<"Desactivation of the traking";
        activated=settings["activated"].toBool();
        Sleep(100);
        myRegulator.closeRegulation();
        qDebug()<<"Should write to disc now";
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

            // now make string to save                                                  //Save the positions in the vector at each new image
            //QString outst(""
//            outst=""+
//            currIm.timeStamp+" "+threshold+" "+targetX+""
//            dataToSave.append(outst);


            /*
                myRegulator.Regulator(x_pos, y_pos);
            }*/

            //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
            float y = 0;
            myRegulator.Regulator(center, y);
            center = center + 0.02;
            //center = center + (rand()%10)/100.;
            if (center>=0.75){center=0.75;}
            //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
        }
    }
    return true;
}

