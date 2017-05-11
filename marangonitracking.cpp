#include "marangonitracking.h"

#include <QList>
#include <QDir>

#ifdef Q_OS_WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace cv;

MarangoniTracking::MarangoniTracking(int thresh,int nrParticles) : threshold(thresh),nrParts(nrParticles),activated(false),shouldTrack(false),myRegulator()
  #ifdef Q_OS_WIN32
  ,mirCtrl()
  #endif
{
    radius = 50.;
    regulation_type = 3;                           //type 0 = point, type 1 = circle, type 2 = arc circle, type 3 = point with feedforward
    target_type = 0;                               //type 0 = step reference, type 1 = traking reference
}

void MarangoniTracking::ChangeSettings(QMap<QString,QVariant> settings) {
    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();

    //Activation of the tracking
    if ((!activated)&&(settings["activated"].toBool())) {
        qDebug()<<"Activation of the tracking";
#ifdef Q_OS_WIN32
        if (!initializeMirror()){                                         //Initialisation of the mirror and...
            qDebug() << "Problem with the MEM connection";                          //...verification that the connection with the MEMs is ok
        }
#endif
        myRegulator.Configure(regulation_type, target_type, radius, targetX, targetY);     //Creation of the figure and initialisation of some parameters
    }

    //Desactivation of the tracking
    if (activated&&(!settings["activated"].toBool())) {
        qDebug()<<"Desactivation of the tracking";
        activated=settings["activated"].toBool();                               //Update and sleep to avoid conflict with the MEM:...
#ifdef Q_OS_WIN32
        Sleep(300);
#else
        usleep(300*1000);  /* sleep for 300 milliSeconds */                     //... No new postion of the laser is required when closing connection...
#endif

#ifdef Q_OS_WIN32                                                               //...and stops the data saving
        closeMirror();
#endif
        qDebug()<<"Writing data to disc...";
        savingData();                                                           //Writting which contains every parameters on disc
        qDebug()<<"Data saved";
    }

    activated=settings["activated"].toBool();
    shouldTrack=settings["shouldTrack"].toBool();
}

bool MarangoniTracking::processImage(ImagePacket& currIm) {

    if (!activated) {
        cv::Mat outImage=currIm.image.clone();
        cv::Point targetPosition(targetX, targetY);
        cv::circle(outImage, targetPosition, radius*0.1, cv::Scalar( 96, 96, 96 ), -1, 8, 0);        //target position
        currIm.image=outImage;

    }
    else {

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
            //            cv::drawContours(outImage,contours,-1,255,2);

            cv::Point newLoc(targetX*currIm.image.cols/100.0,targetY*currIm.image.rows/100.0);
            cv::ellipse(outImage,newLoc,cv::Size(5,5),0,0,360,255,-1);

            //            std::cout << contours.size() << std::endl;
            //            std::cout << contours[0] << std::endl;

            if (contours[0].size() > 5) { //otherwise fitEllipse does not work
                cv::RotatedRect contours_part = cv::fitEllipse(Mat(contours[0]));
                cv::ellipse(outImage,contours_part,255,2);
                cv::Point center_contours_part(contours_part.center.x,contours_part.center.y);
                cv::circle(outImage,center_contours_part,2,255,-1);
                Ppoint[0] = contours_part.center.x;
                Ppoint[1] = contours_part.center.y;
            }

            currIm.image=outImage;

            if ((regulation_type == 0) || (regulation_type == 1) || (regulation_type == 2)){
                myRegulator.Regulator2016(Ppoint[0], Ppoint[1]);                           //Modification of the laser position
            }
            else {
                myRegulator.Regulator2017(Ppoint[0], Ppoint[1]);                           //Modification of the laser position
            }

#ifdef Q_OS_WIN32
            if (regulation_type == 0) || (regulation_type == 3){                   //Transfer new data stream to the mirror
                mirCtrl.ChangeMirrorPosition(myRegulator.laser_x,myRegulator.laser_y);
            }
            else {
                mirCtrl.ChangeMirrorStream(myRegulator.x_vector,myRegulator.y_vector);
            }
#endif

            //Adding figures on screen
            //cv::Mat outImage=currIm.image.clone();
            cv::Point particlePos(Ppoint[0], Ppoint[1]);
            cv::circle(outImage, particlePos, radius*0.1, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //particle
            cv::Point targetPosition(myRegulator.target_x, myRegulator.target_y);
            cv::circle(outImage, targetPosition, radius*0.1, cv::Scalar( 96, 96, 96 ), -1, 8, 0);        //target position
            cv::Point figureCenter(myRegulator.laser_x, myRegulator.laser_y);
            if (regulation_type == 0){
                cv::circle(outImage, figureCenter, radius*0.1, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //regulation with the point
            }
            else if (regulation_type == 1){
                cv::circle(outImage, figureCenter, radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);            //regulation with the circle
            }
            else if (regulation_type == 2){
                if (myRegulator.objectifReached == false){
                    cv::Size size( radius, radius );                                                        //regulation with a arc circle
                    int middleAngle = myRegulator.middleAngle*180/M_PI;
                    cv::ellipse(outImage, figureCenter, size, 0, middleAngle - 90, middleAngle + 90, cv::Scalar( 0, 0, 255 ), 1, 8, 0);
                }
                else{
                    cv::circle(outImage, targetPosition, radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);       //objective reached. Stabilisation with circle
                }
            }
            else if (regulation_type == 3){
                cv::circle(outImage, figureCenter, radius*0.1, cv::Scalar( 200, 200, 200 ), 3, 8, 0);        //regulation with the point
            }

            currIm.image=outImage;
            //Adding all parameters in the 'dataToSave' vector
            QString currentData(QString::number(Ppoint[0])          //x_particle
                    +" "
                    +QString::number(Ppoint[1])         //y_particle
                    +" "
                    +QString::number(myRegulator.laser_x)     //x position of the center of the pattern
                    +" "
                    +QString::number(myRegulator.laser_y)     //y position of the center of the pattern
                    +" "
                    +QString::number(myRegulator.middleAngle)     //angle of the figure
                    +" "
                    +QString::number(myRegulator.u)     //u_dot_aux value
                    +" "
                    +QString::number(myRegulator.target_x)     //x target
                    +" "
                    +QString::number(myRegulator.target_y)     //y target
                    +" "
                    +QString::number(myRegulator.objectifReached)     //objective reached (1: true, 0: false)
                    +" "
                    +QString::number(currIm.timeStamp,'f',0)  //time of the picture
                    +"\n");

            dataToSave.append(currentData);
        }
    }
    return true;
}

void MarangoniTracking::savingData(){                                                       //Used to write dataToSave on disc
    QString filename = "../Data/test.txt";
    QFile file (filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << dataToSave;
    file.close();
}

#ifdef Q_OS_WIN32
bool MarangoniTracking::initializeMirror(){
    bool properConnection = mirCtrl.Initialisation();                        //initialisation of the mirror
    return properConnection;
}

void MarangoniTracking::closeMirror() {
    qDebug()<<"Closing the mirror";
    mirCtrl.Closing();                                                  //Close the mirror
}
#endif
