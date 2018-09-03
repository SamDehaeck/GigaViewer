#include "marangonitracking.h"

#include <QList>
#include <QDir>

#ifdef Q_OS_WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace cv;

MarangoniTracking::MarangoniTracking(int thresh,int nrParticles) : threshold(thresh),nrParts(nrParticles),activated(false),myRegulator()
  #ifdef Q_OS_WIN32
  ,mirCtrl()
  #endif
{
    radius = 50.;
    regulation_type = 3;                           //type 0 = point, type 1 = circle, type 2 = arc circle, type 3 = point with feedforward
    target_type = 0;                               //type 0 = step reference, type 1 = tracking reference, see regulator for the track followed
    myRegulator.x_las=0.0;       myRegulator.y_las=0.0;

}

void MarangoniTracking::ChangeSettings(QMap<QString,QVariant> settings) {

    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();

    myRegulator.prog_mode = settings["Program_ID"].toInt()/100;        // 0 for identification    1 for feedback control   2 Feedback+Feedforward control
    myRegulator.pattern_type = settings["Program_ID"].toInt()%100;     // 0 for point-wise pattern     1 for arc+circle pattern


    //We are in MANUAL MODE, so store the given laser position in the global variables X_las and Y_las manual
    if (myRegulator.prog_mode == 3){
        X_las_manual=settings["X_laserPOS"].toFloat();
        Y_las_manual=settings["Y_laserPOS"].toFloat();

    }


    //Activation of the tracking
    if ((!activated)&&(settings["activated"].toBool())) {
        qDebug()<<"Activation of the tracking";

#ifdef Q_OS_WIN32
        if (!initializeMirror()){                                         //Initialisation of the mirror and...
            qDebug() << "Problem with the MEM connection";                //...verification that the connection with the MEMs is ok
        }
#endif

        //myRegulator.Configure(regulation_type, target_type, radius, targetX, targetY);     //Creation of the figure and initialisation of some parameters
        myRegulator.Configure_FB (settings["Program_ID"].toInt(), radius, targetX, targetY,settings["kp"].toFloat(), settings["Ti"].toFloat(),  settings["Td"].toFloat(),settings["Tt"].toFloat(), settings["Usat"].toFloat(), settings["Tsamp"].toFloat());
        myRegulator.Configure_FF(0.0 , 0.0,settings["kff"].toFloat());      //For now I just give whatever value which is then replaced in the method. This is to be improved in the future



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
    myRegulator.Flag_LinearTracking=settings["TrackingON"].toBool();                                //2018 New variable that allows to Determine if we are working in line tracking mode or not.
    settingsBKUP = settings;    settingsBKUP.detach();                          //To stop the program I recall the change setting function with this modified copy.
    //qDebug()<<"Changesettings Seeting BKUP " << settingsBKUP["activated"].toBool();

}

bool MarangoniTracking::processImage(ImagePacket& currIm) {

    if (!activated) {
        cv::Mat outImage=currIm.image.clone();
        cv::Point targetPosition(targetX, targetY);
        cv::circle(outImage, targetPosition, radius*0.1, cv::Scalar( 96, 96, 96 ), -1, 8, 0);        //target position
        currIm.image=outImage;

        cv::Point figureCenter(myRegulator.x_las, myRegulator.y_las);
        if (myRegulator.pattern_type == 0){
            cv::circle(outImage, figureCenter, radius*0.2, cv::Scalar( 0, 0, 255 ), 2, 8, 0);       //DRAW LASER point on the Screen
        } else if (myRegulator.pattern_type == 1){                                                  //DRAW LASER PATTERN on the Screen TO CHECK

                        }
        if (myRegulator.prog_mode==2)
        {
            cv::circle(outImage, cv::Point(myRegulator.x_obst, myRegulator.y_obst), radius*0.1, cv::Scalar( 255, 255, 255 ), 5, 8, 0);
        }


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

            //This is the openCv closing operation which according to this explanation
            //http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/doc/tutorials/imgproc/opening_closing_hats/opening_closing_hats.html
            //Should closing those white gaps in the obstacle, however is not working using the FitEllipse method
            int morph_size = 4;
            Mat element = getStructuringElement( MORPH_RECT , cv::Size( 2*morph_size + 1 , 2*morph_size +1), cv::Point( morph_size, morph_size ) );
            morphologyEx(processed,processed,  MORPH_CLOSE, element);


            std::vector<std::vector<cv::Point> > contours;
            cv::Mat hierarchy;
            cv::findContours( processed, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

            cv::Mat outImage=currIm.image.clone(); // use a clone to avoid storing on raw image which is stored.
            //cv::drawContours(outImage,contours,-1,255,2);

            //cv::Point newLoc(targetX*currIm.image.cols/100.0,targetY*currIm.image.rows/100.0);
            //cv::ellipse(outImage,newLoc,cv::Size(5,5),0,0,360,150,-1);

            //std::cout << contours.size() << std::endl;
            //std::cout << contours[0] << std::endl;

            qDebug()<<contours.size();
            double area = cv::contourArea(contours[0]);
            //qDebug()<<"Contour 0 area "<<area;

            for (int i = 0; i < contours.size(); i++)
            {
                // Area between 10-50 that means is the particle

                area = cv::contourArea(contours[i]);
                if (area > 20 && area < 50)
                {
                    Moments mu = moments( contours[i], false );
                    Point2f mc = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
                    Ppoint[0] = mc.x;
                    Ppoint[1] = mc.y;



//                    cv::RotatedRect contours_part = cv::fitEllipse(Mat(contours[i]));
//    //                cv::ellipse(outImage,contours_part,255,2);
//    //                cv::Point center_contours_part(contours_part.center.x,contours_part.center.y);
//    //                cv::circle(outImage,center_contours_part,1,255,-1);
//                    Ppoint[0] = contours_part.center.x;
//                    Ppoint[1] = contours_part.center.y;

                     qDebug()<< "Pos" << Ppoint[0] << Ppoint[1] << "Cont size" <<contours[i].size() <<"Contour i area "<<area;

                }

            }



            currIm.image=outImage;

//            if ((regulation_type == 0) || (regulation_type == 1) || (regulation_type == 2)){
//                myRegulator.Regulator2016(Ppoint[0], Ppoint[1]);                           //Modification of the laser position
//            }
//            else {
//                myRegulator.Regulator2017(Ppoint[0], Ppoint[1]);                           //Modification of the laser position

//            }

            if (myRegulator.prog_mode == 0){
                //Program the IDENTIFICATION ROUTINE

            }else if (myRegulator.prog_mode == 1){
                //Feedback controller compute
                if (myRegulator.Flag_LinearTracking )
                {
                    myRegulator.run_Traj_Generator(Ppoint[0], Ppoint[1]);
                }

                myRegulator.run_Cont_FB(Ppoint[0], Ppoint[1]);          //Get feedback controller response
                myRegulator.get_Laser_Position(Ppoint[0], Ppoint[1]);   //Combine both responses and get the right laser position X_lasD
                myRegulator.compute_Laser_Pattern(myRegulator.x_las, myRegulator.y_las);    //Define the pattern


            }else if (myRegulator.prog_mode == 2){
                //Feedback + Feedforward control        TO BE PROGRAMMED STILL
                if (myRegulator.Flag_LinearTracking)
                {
                    myRegulator.run_Traj_Generator(Ppoint[0], Ppoint[1]);
                }
                myRegulator.run_Cont_FB(Ppoint[0], Ppoint[1]);          //Get Feedback controller response
                myRegulator.run_Cont_FF(Ppoint[0], Ppoint[1]);          //Get Feedfoward controller response
                myRegulator.get_Laser_Position(Ppoint[0], Ppoint[1]);   //Combine both responses and get the right laser position X_lasD
                myRegulator.compute_Laser_Pattern(myRegulator.x_las, myRegulator.y_las);    //Define the pattern

            }else if (myRegulator.prog_mode == 3){
                //MANUAL MODE The laser position is already stored in PIXELS in the global variables X_las_manual, Y_las_manual
                myRegulator.compute_Laser_Pattern(X_las_manual, Y_las_manual);    //Define the pattern
                 qDebug()<<myRegulator.x_las << myRegulator.y_las;

            }

#ifdef Q_OS_WIN32
//            //Transfer new data stream to the mirror
//            if ((regulation_type == 1) || (regulation_type == 2)) {
//                mirCtrl.ChangeMirrorStream(myRegulator.x_vector,myRegulator.y_vector);
//            }
//            else {
//                mirCtrl.ChangeMirrorPosition(myRegulator.laser_x,myRegulator.laser_y);
//            }
            //Transfer new data stream to the mirror        USING RONALD VARIABLES
                        if (myRegulator.pattern_type == 0){
                            mirCtrl.ChangeMirrorPosition(myRegulator.x_las,myRegulator.y_las);  //Point-wise control strategy send point
                        }
                        else if (myRegulator.pattern_type == 1){
                            mirCtrl.ChangeMirrorStream(myRegulator.x_vector,myRegulator.y_vector);  //Pattern type ARC+Circle control strategy send pattern
                        }

#endif

//            //Adding figures on screen
//            //cv::Mat outImage=currIm.image.clone();
            cv::Point particlePos(Ppoint[0], Ppoint[1]);
            cv::circle(outImage, particlePos, radius*0.4, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //particle
            //cv::Point targetPosition(myRegulator.target_x, myRegulator.target_y);
            cv::Point targetPosition(targetX, targetY);
            cv::circle(outImage, targetPosition, radius*0.1, cv::Scalar( 96, 96, 96 ), -1, 8, 0);        //target position
            //cv::Point figureCenter(myRegulator.laser_x, myRegulator.laser_y);
            cv::Point figureCenter(myRegulator.x_las, myRegulator.y_las);



//            if (regulation_type == 0){
//                cv::circle(outImage, figureCenter, radius*0.1, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //regulation with the point
//            }
//            else if (regulation_type == 1){
//                cv::circle(outImage, figureCenter, radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);            //regulation with the circle
//            }
//            else if (regulation_type == 2){
//                if (myRegulator.objectifReached == false){
//                    cv::Size size( radius, radius );                                                        //regulation with a arc circle
//                    int middleAngle = myRegulator.middleAngle*180/M_PI;
//                    cv::ellipse(outImage, figureCenter, size, 0, middleAngle - 90, middleAngle + 90, cv::Scalar( 0, 0, 255 ), 1, 8, 0);
//                }
//                else{
//                    cv::circle(outImage, targetPosition, radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);       //objective reached. Stabilisation with circle
//                }
//            }
//            else if (regulation_type == 3){
//                cv::circle(outImage, figureCenter, radius*0.1, cv::Scalar( 200, 200, 200 ), 3, 8, 0);    //regulation with the point and feedforward to reject perturbation
//                cv::Point obstaclePosition(myRegulator.x_obst, myRegulator.y_obst);
//                cv::circle(outImage, obstaclePosition, radius*0.1, cv::Scalar( 255, 255, 255 ), 5, 8, 0);
//            }


            if (myRegulator.pattern_type == 0){
                cv::circle(outImage, figureCenter, radius*0.2, cv::Scalar( 0, 0, 255 ), 2, 8, 0);       //DRAW LASER point on the Screen
            } else if (myRegulator.pattern_type == 1){                                                  //DRAW LASER PATTERN on the Screen TO CHECK
                                if (myRegulator.objectifReached == false){
                                    cv::Size size( radius, radius );                                                        //regulation with a arc circle
                                    int middleAngle = myRegulator.middleAngle*180/M_PI;
                                    cv::ellipse(outImage, figureCenter, size, 0, middleAngle - 90, middleAngle + 90, cv::Scalar( 0, 0, 255 ), 1, 8, 0);
                                }
                                else{
                                    cv::circle(outImage, targetPosition, radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);       //objective reached. Stabilisation with circle
                                }
                            }


            cv::circle(outImage, cv::Point(myRegulator.x_obst, myRegulator.y_obst), radius*0.1, cv::Scalar( 255, 255, 255 ), 5, 8, 0);


            currIm.image=outImage;
            //Adding all parameters in the 'dataToSave' vector
            QString currentData(QString::number(Ppoint[0])          //x_particle (in px)
                    +"\t"
                    +QString::number(Ppoint[1])         //y_particle
                    +"\t"
                    //+QString::number(myRegulator.laser_x)     //x position of the center of the pattern
                    +QString::number(myRegulator.x_las)     //x position of the center of the pattern
                    +"\t"
                    //+QString::number(myRegulator.laser_y)     //y position of the center of the pattern
                    +QString::number(myRegulator.y_las)     //y position of the center of the pattern
                    +"\t"
                    +QString::number(myRegulator.middleAngle)     //angle of the figure
                    +"\t"
                    +QString::number(myRegulator.u_Final)     //u_dot_aux value
                    +"\t"
                    +QString::number(myRegulator.theta_corr_Final)  //time of the picture
                    +"\t"
                    //+QString::number(myRegulator.target_x)     //x target
                    +QString::number(myRegulator.x_targ)     //x target
                    +"\t"
                    //+QString::number(myRegulator.target_y)     //y target
                    +QString::number(myRegulator.y_targ)     //x target
                    +"\t"
                    +QString::number(myRegulator.x_obst)     //x obstacle
                    +"\t"
                    +QString::number(myRegulator.y_obst)     //y obstacle
                    +"\t"
                    +QString::number(myRegulator.objectifReached)     //objective reached (1: true, 0: false)
                    +"\t"
                    +QString::number(currIm.timeStamp,'f',0)  //time of the picture
                    +"\t"
                    +QString::number(myRegulator.prog_mode)  //Current mode 0 identification 1 Feedback control 2 Feedback+Feedforward
                    +"\t"
                    +QString::number(myRegulator.Flag_LinearTracking)  //Flag to know If I tracaking or not
                    +"\t"
                    +QString::number(myRegulator.u_0)  //Output computed by the Particle Velocity Magnitude cont FB
                    +"\t"
                    +QString::number(myRegulator.theta_corr_FB)  //Output computed by the Particle Velocity direction cont FB
                    +"\t"
                    +QString::number(myRegulator.e_0)  //Distance Error of the FB controller
                    +"\t"
                    +QString::number(myRegulator.e_ang)  //Angular Error of the FB controller
                    +"\t"
                    +QString::number(myRegulator.u_0_FF)  //Output computed by the FF controller (velocity component)
                    +"\t"
                    +QString::number(myRegulator.theta_corr_FF)  //Output computed by the FF controller (Angular component)

                    +"\r\n");

            dataToSave.append(currentData);
            //qDebug()<<currentData;  //To ch
        }

        //Process to stop the controller once all the points of the trajectory have been given
        if ( (myRegulator.prog_mode == 1||myRegulator.prog_mode == 2)&& myRegulator.Flag_LinearTracking )
        {
            //myRegulator.run_Traj_Generator(Ppoint[0], Ppoint[1]);
            if (myRegulator.FRAMECOUNTER==myRegulator.x_traj.size())
            {
                settingsBKUP["activated"]= false;       //This emulates me unticking the Activate Tracking button
                ChangeSettings(settingsBKUP);           //With this artifitial modified settings values, I call the ChangeSettings, which will change the activated variables correctly, close the mirror and store the results
            }
        }
    }

    return true;
}

void MarangoniTracking::savingData(){                         //Used to write dataToSave on disc
    QString filename = "D:/TEXTFiles/GigaViewer_Results.txt";
    QFile file (filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << dataToSave;
    file.close();
    dataToSave.clear();             //Once the data is saved, just clear the String chain
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
