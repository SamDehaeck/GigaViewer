#include "marangonitracking.h"

#include <QList>
#include <QStringList>
#include <QDir>

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

    //Activation of the tracking
    if ((!activated)&&(settings["activated"].toBool())) {
        qDebug()<<"Activation of the tracking";
#ifdef Q_OS_WIN32
        myRegulator.Initialisation();                                           //Initialisation of the mirror
        if (myRegulator.properConnection == false){                             //Verification that the connection with the MEMs is ok
            qDebug()<<"Problem with the MEM conection - Closing the program";
            //exit(0);                                                          //...if no proper connection, exits the programme
        }
        myRegulator.Figure(1, targetX, targetY);                                //Creation of the figure. For the future, need a button to choose which type of figure is used
#endif
    }

    //Desactivation of the tracking
    if (activated&&(!settings["activated"].toBool())) {
        qDebug()<<"Desactivation of the traking";
        activated=settings["activated"].toBool();                               //Update and sleep to avoid conflict with the MEM:...
        Sleep(100);                                                             //... No new postion of the laser is required when closing connection
#ifdef Q_OS_WIN32
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

            //Image processing here --> gives x_pos and y_pos: (x,y) position of the particule

#ifdef Q_OS_WIN32
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
#endif

            //Adding all parameters in the 'dataToSave' vector at each new image
            QString currentData(QString::number(center)
                                +","
                                +QString::number(targetX)
                                +",");
            dataToSave.append(currentData);
        }
    }
    return true;
}

void MarangoniTracking::savingData(){                                                       //Used to write dataToSave on disc

    QString filename = "Data.txt";
    QFile file (filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << dataToSave;
    file.close();
}
