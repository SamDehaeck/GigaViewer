#include "marangonitracking.h"

#include <QList>
#include <QDir>

using namespace cv;

MarangoniTracking::MarangoniTracking(int thresh,int nrParticles) : threshold(thresh),nrParts(nrParticles),activated(false),shouldTrack(false)
#ifdef Q_OS_WIN32
  ,myRegulator()
#endif
{
    radius = 50.;
    regulation_type = 2;                                                        //type 0 = point, type 1 = circle, type 2 = arc circle
    target_type = 0;                                                          //type 0 = step reference, type 1 = traking reference
}

void MarangoniTracking::ChangeSettings(QMap<QString,QVariant> settings) {
    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();

    //Activation of the tracking
    if ((!activated)&&(settings["activated"].toBool())) {
        qDebug()<<"Activation of the tracking";
#ifdef Q_OS_WIN32
        if (!myRegulator.Initialisation()){                                         //Initialisation of the mirror and...
            qDebug() << "Problem with the MEM connection";                          //...verification that the connection with the MEMs is ok
        }
        myRegulator.Figure(regulation_type, target_type, radius, targetX, targetY);     //Creation of the figure and initialisation of some parameters
#endif
    }

    //Desactivation of the tracking
    if (activated&&(!settings["activated"].toBool())) {
        qDebug()<<"Desactivation of the traking";
        activated=settings["activated"].toBool();                               //Update and sleep to avoid conflict with the MEM:...
        Sleep(300);                                                             //... No new postion of the laser is required when closing connection...
#ifdef Q_OS_WIN32                                                               //...and stops the data saving
        myRegulator.closeRegulation();
#endif
        qDebug()<<"Writing data to disc...";
        savingData();                                                           //Writting dataToSave which contains every parameters on disc
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

            getPosPartLITTLE(currIm.image, contoursP, hierachyP);

#ifdef Q_OS_WIN32

                myRegulator.Regulator(Ppoint[0], Ppoint[1]);                           //Modification of the laser position

                //Adding figures on screen
                cv::Mat outImage=currIm.image.clone();
                cv::Point particlePos(Ppoint[0], Ppoint[1]);
                cv::circle(outImage, particlePos, radius*0.1, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //particle
                cv::Point targetPosition(myRegulator.target_x, myRegulator.target_y);
                cv::circle(outImage, targetPosition, radius*0.1, cv::Scalar( 96, 96, 96 ), -1, 8, 0);        //target position
                cv::Point figureCenter(myRegulator.x, myRegulator.y);
                if (regulation_type == 0){
                    cv::circle(outImage, figureCenter, radius*0.1, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //regulation with the point
                }
                else if (regulation_type == 1){
                    cv::circle(outImage, figureCenter, radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);            //regulation with the circle
                }
                else {
                    if (myRegulator.objectifReached == false){
                        cv::Size size( radius, radius );                                                        //regulation with a arc circle
                        int middleAngle = myRegulator.middleAngle*180/M_PI;
                        cv::ellipse(outImage, figureCenter, size, 0, middleAngle - 90, middleAngle + 90, cv::Scalar( 0, 0, 255 ), 1, 8, 0);
                    }
                    else{
                        cv::circle(outImage, targetPosition, radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);       //objective reached. Stabilisation with circle
                    }
                }

                currIm.image=outImage;
                //Adding all parameters in the 'dataToSave' vector
                QString currentData(QString::number(Ppoint[0])          //x_particle
                                    +","
                                    +QString::number(Ppoint[1])         //y_particle
                                    +","
                                    +QString::number(myRegulator.x)     //x position of the center of the circle
                                    +","
                                    +QString::number(myRegulator.y)     //y position of the center of the circle
                                    +","
                                    +QString::number(myRegulator.middleAngle)     //angle of the figure
                                    +","
                                    +QString::number(myRegulator.u)     //u_dot_aux value
                                    +","
                                    +QString::number(myRegulator.target_x)     //x target
                                    +","
                                    +QString::number(myRegulator.target_y)     //y target
                                    +","
                                    +QString::number(myRegulator.objectifReached)     //objective reached (1: true, 0: false)
                                    +","
                                    +QString::number(currIm.timeStamp)  //time of the picture
                                    +",");
                dataToSave.append(currentData);

#endif
        }
    }
    return true;
}

void MarangoniTracking::savingData(){                                                       //Used to write dataToSave on disc

    //QString filename = "StepTest_5_2.txt";
    //QString filename = "RampTest_4_1.txt";
    QString filename = "06062016_VideoTest_1.txt";
    //QString filename = "06062016_trajectoryTest_r50_a180_p120_index1";
    QFile file (filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << dataToSave;
    file.close();
}



void MarangoniTracking::getPosPartLITTLE(cv::Mat src, std::vector<std::vector<cv::Point>> contoursf, cv::OutputArray hierarchy)
{
    cv::Mat img2_gray;
    cv::Mat img2_bw(img2_gray.cv::Mat::size(), CV_8U);
    int findcont = 0;
    std::vector<std::vector<cv::Point>> contours;   //vector<vector<Point> > contoursaux;

    img2_gray=src.cv::Mat::clone(); //OR cvtColor( src, img2_gray, CV_BGR2GRAY );
    cv::threshold ( img2_gray, img2_bw, threshold, 255, THRESH_BINARY_INV ); //Equivalent to the function graythresh + imgbw from MATLAB
    cv::GaussianBlur( img2_bw, img2_bw, Size( 5, 5 ), 0, 0 );
    cv::threshold ( img2_bw, img2_bw, 0, 255, THRESH_BINARY | THRESH_OTSU ); //Applying the filter render everything to grey scale so I "binarize it again"

    cv::findContours( img2_bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );



    //IMPROVEMENT: In the case I missclicked on the particle, the algorithm just show the previous value:
    //I expect this not to happen when performing real experiments. It is only when pushing the particle manually
    //The no contour was happend it is probable that I overlapped on the particle so I should only keep the last value
    if (contours.size() >=1)
    {
        //Only if the current area is bigger replace, otherwise keep the value
        for( int i = 1; i < contours.size() ; i++ )
        { if (  contourArea(contours[i]) > contourArea(contours[findcont]) )
            {  //area1 = contourArea(contours[i]); area2 = contourArea(contours[findcont]);
                findcont = i; }
        }

        //contoursf.push_back(contours[findcont]);
        contoursf.insert(contoursf.begin(), contours.begin() + findcont, contours.begin() + findcont+1);

        //contours[findcont], 0);
        // Get the moments
        std::vector<cv::Moments> mu(contoursf.size() );
        for( int i = 0; i < contoursf.size(); i++ )
        { mu[i] = moments( contoursf[i], false ); }

        ///  Get the mass centers:
        std::vector<cv::Point2f> mc( contoursf.size() );
        for( int i = 0; i < contoursf.size(); i++ )
        { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

        Ppoint[0] = mc[0].x;
        Ppoint[1] = mc[0].y;
    }
}
