#include "marangonitracking.h"

#include <QList>
#include <QDir>

#ifdef Q_OS_WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace cv;

MarangoniTracking::MarangoniTracking() :activated(false),myRegulator()
  #ifdef Q_OS_WIN32
  ,mirCtrl()
  #endif
{
    radius = 50.;
    regulation_type = 3;                           //type 0 = point, type 1 = circle, type 2 = arc circle, type 3 = point with feedforward
    target_type = 0;                               //type 0 = step reference, type 1 = tracking reference, see regulator for the track followed
    myRegulator.x_las=0.0;       myRegulator.y_las=0.0;
    firstScan=1;
}

void MarangoniTracking::ChangeSettings(QMap<QString,QVariant> settings) {

    if (targetX!=settings["targetX"].toInt()||targetY!=settings["targetY"].toInt())
        myRegulator.resetIntegratorVariables();
    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();
    if (nrParts!=settings["NroPart"].toInt())
        firstScan=1;
    nrParts=settings["NroPart"].toInt();

    if (myRegulator.prog_mode != settings["Program_ID"].toInt()/100)
        myRegulator.resetIntegratorVariables();
    myRegulator.prog_mode = settings["Program_ID"].toInt()/100;        // 0 for identification    1 for feedback control   2 Feedback+Feedforward control
    myRegulator.pattern_type = settings["Program_ID"].toInt()%100;     // 0 for point-wise pattern     1 for arc+circle pattern


    //We are in MANUAL MODE, so store the given laser position in the global variables X_las and Y_las manual
    SampleRate=10000;
    if (myRegulator.prog_mode == 3||myRegulator.prog_mode == 6){
        X_las_manual=settings["X_laserPOS"].toFloat();
        Y_las_manual=settings["Y_laserPOS"].toFloat();
        A_las_manual=settings["A_laserPOS"].toFloat();
        R_pat_manual=settings["R_patternPOS"].toFloat()/10.0;
        Aux_pat_manual=settings["A_patternPOS"].toFloat()/100.0;
    }
    if (myRegulator.prog_mode == 0){
        Ident_dir=settings["Ident_LasPos"].toInt();
        Ident_dist=settings["Ident_LasPart_D"].toFloat();
        Ident_radius=settings["Ident_PatternR_D"].toFloat();
        Ident_aux=settings["Ident_PatternA_D"].toFloat();
    }
    if ((myRegulator.prog_mode == 3)||(myRegulator.prog_mode == 0)){
        SampleRate=settings["SampleRate"].toInt();
    }

    SampleSense=1-2*settings["SampleSense"].toInt();

    //Activation of the tracking
    if ((!activated)&&(settings["activated"].toBool())) {
        firstScan=1;
        myRegulator.resetIntegratorVariables();
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
        Sleep(1000);
#else
        usleep(1000*1000);  /* sleep for 300 milliSeconds */                     //... No new postion of the laser is required when closing connection...
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

    timeStamp=currIm.timeStamp;
    if (!activated) {
        if (myRegulator.prog_mode==1||myRegulator.prog_mode==2||myRegulator.prog_mode == 5)
            drawTarget(currIm.image);
        drawLaser(currIm.image);
    }
    else {
        analizeImage(currIm);


        QString currentData;

        if (myRegulator.prog_mode == 0){
            myRegulator.run_Ident(Ppoint[0][0], Ppoint[1][0],Ident_dist,Ident_radius,Ident_aux,Ident_dir);
            myRegulator.compute_Laser_Pattern(myRegulator.pattern_type ,myRegulator.x_las, myRegulator.y_las,myRegulator.middleAngle-M_PI,myRegulator.pattern_radius, SampleSense*myRegulator.pattern_aux);
        }else if (myRegulator.prog_mode == 1){
            //Feedback controller compute
            if (myRegulator.Flag_LinearTracking )
            {
                myRegulator.run_Traj_Generator(Ppoint[0][0], Ppoint[1][0],targetX, targetY);
            }

            myRegulator.run_Cont_FB(Ppoint[0][0], Ppoint[1][0],targetX, targetY);          //Get feedback controller response
            myRegulator.get_Laser_Position(Ppoint[0][0], Ppoint[1][0],targetX, targetY);   //Combine both responses and get the right laser position X_lasD
            myRegulator.compute_Laser_Pattern(myRegulator.pattern_type ,myRegulator.x_las, myRegulator.y_las,myRegulator.middleAngle-M_PI,radius,0.5);    //Define the pattern


        }else if (myRegulator.prog_mode == 2){
            //Feedback + Feedforward control        TO BE PROGRAMMED STILL
            if (myRegulator.Flag_LinearTracking)
            {
                myRegulator.run_Traj_Generator(Ppoint[0][0], Ppoint[1][0],targetX, targetY);
            }
            myRegulator.run_Cont_FB(Ppoint[0][0], Ppoint[1][0],targetX, targetY);          //Get Feedback controller response
            myRegulator.run_Cont_FF(Ppoint[0][0], Ppoint[1][0],targetX, targetY);          //Get Feedfoward controller response
            myRegulator.get_Laser_Position(Ppoint[0][0], Ppoint[1][0],targetX, targetY);   //Combine both responses and get the right laser position X_lasD
            myRegulator.compute_Laser_Pattern(myRegulator.pattern_type ,myRegulator.x_las, myRegulator.y_las,myRegulator.middleAngle-M_PI,radius,0.5);    //Define the pattern

        }else if (myRegulator.prog_mode == 3){
            //MANUAL MODE The laser position is already stored in PIXELS in the global variables X_las_manual, Y_las_manual
            myRegulator.compute_Laser_Pattern(myRegulator.pattern_type ,X_las_manual, Y_las_manual, (A_las_manual*M_PI)/180.0, R_pat_manual*1024.0/65.0, SampleSense*Aux_pat_manual);    //Define the pattern
            //qDebug()<<myRegulator.x_las << myRegulator.y_las;

        }else if (myRegulator.prog_mode == 4){
            currentData=runMirrorCalibration();
        }else if (myRegulator.prog_mode == 5){
            float dlim=160;
            float dlimMin=dlim*0.8;
            float dlimHyst=dlim*1.4;
            bool static flagHyst=1;
            bool static flagMult=1;
            if (!myRegulator.run_SmartShielding(Ppoint[0][0],Ppoint[1][0],Ppoint[0][1],Ppoint[1][1],targetX, targetY,dlimMin, radius)){
                if (flagMult||myRegulator.objectifReached){
                    if (flagHyst)
                        dlim=dlimHyst;
                    flagHyst=myRegulator.run_SmartShielding(Ppoint[0][0],Ppoint[1][0],Ppoint[0][1],Ppoint[1][1],targetX, targetY,dlim, radius);
                    if (!flagHyst){
                        myRegulator.run_Cont_FB(Ppoint[0][0], Ppoint[1][0],targetX, targetY);          //Get feedback controller response
                        myRegulator.get_Laser_Position(Ppoint[0][0], Ppoint[1][0],targetX, targetY);   //Combine both responses and get the right laser position X_lasD
                    }
                }else{
                    myRegulator.run_Cont_FB(Ppoint[0][0], Ppoint[1][0],targetX, targetY);          //Get feedback controller response
                    myRegulator.get_Laser_Position(Ppoint[0][0], Ppoint[1][0],targetX, targetY);   //Combine both responses and get the right laser position X_lasD
                }
            }else{
                flagHyst=1;
            }
            flagMult=!flagMult;
            myRegulator.compute_Laser_Pattern(0 ,myRegulator.x_las, myRegulator.y_las,myRegulator.middleAngle-M_PI,radius,0.5);    //Define the pattern
        }else if (myRegulator.prog_mode == 6){
            bool static flag=1;
            //MANUAL MODE The laser position is already stored in PIXELS in the global variables X_las_manual, Y_las_manual
            if (flag){
                myRegulator.compute_Laser_Pattern(myRegulator.pattern_type ,X_las_manual, Y_las_manual, (A_las_manual*M_PI)/180.0, R_pat_manual*1024.0/65.0, SampleSense*Aux_pat_manual);    //Define the pattern
                flag=0;
            }else{
                float xlas=X_las_manual+R_pat_manual*1024.0/65.0*cos((A_las_manual*M_PI)/180.0);
                float ylas=Y_las_manual+R_pat_manual*1024.0/65.0*sin((A_las_manual*M_PI)/180.0);
                myRegulator.compute_Laser_Pattern(0 ,xlas, ylas, (A_las_manual*M_PI)/180.0, R_pat_manual*1024.0/65.0, SampleSense*Aux_pat_manual);
                flag=1;
            }
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
        if (myRegulator.prog_mode == 3||myRegulator.prog_mode == 0){
            mirCtrl.SetSampleRate(SampleRate);
        }
        if (myRegulator.prog_mode != 4){
                    if (myRegulator.pattern_type == 0){
                        mirCtrl.ResetMirrorStream();
                        mirCtrl.ChangeMirrorPosition(myRegulator.x_las,myRegulator.y_las);  //Point-wise control strategy send point
                    }
                    else{
                        mirCtrl.ResetMirrorVoltage();
                        mirCtrl.ChangeMirrorStream(myRegulator.x_vector,myRegulator.y_vector);  //Pattern type ARC+Circle control strategy send pattern
                    }
        }

#endif

//            //Adding figures on screen
//            //cv::Mat outImage=currIm.image.clone();

        if (myRegulator.prog_mode==1||myRegulator.prog_mode==2||myRegulator.prog_mode==5)
            drawTarget(currIm.image);
        drawParticles(currIm.image);
        drawLaser(currIm.image);
        if (myRegulator.prog_mode==2)
            drawObstacle(currIm.image);

        //Adding all parameters in the 'dataToSave' vector
        if (myRegulator.prog_mode != 4)
            currentData=setdata();
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
    return true;
}


void MarangoniTracking::savingData(){                         //Used to write dataToSave on disc
    QString filename;
    QDateTime mom = QDateTime::currentDateTime();
    if (myRegulator.prog_mode == 4){
        filename = "D:/TEXTFiles/MirrorCalibration_Results_"+mom.toString("yyyyMMdd-hhmmss")+".txt";
    }
    else{
        filename = "D:/TEXTFiles/GigaViewer_Results_"+mom.toString("yyyyMMdd-hhmmss")+".txt";
    }
    QFile file (filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << dataToSave;
    file.close();
    dataToSave.clear();             //Once the data is saved, just clear the String chain
}

bool MarangoniTracking::orderPart(float point[][10], float point_minus_1[][10], float angle[10], float angle_minus_1[10], int Nro)
{
    int jmin[10];
    bool flag_error=0;
    for (int i=0; i<Nro; i++){
        float dx=point[0][i]-point_minus_1[0][0];
        float dy=point[1][i]-point_minus_1[1][0];
        float dmin=dx*dx+dy*dy;
        jmin[i]=0;
        for (int j=1; j<Nro; j++){
            dx=point[0][i]-point_minus_1[0][j];
            dy=point[1][i]-point_minus_1[1][j];
            float dist=dx*dx+dy*dy;
            if (dist<dmin){
                dmin=dist;
                jmin[i]=j;
            }
        }
    }
    for (int i=0; i<Nro; i++){
        for (int j=i+1; j<Nro; j++){
            if (jmin[i]==jmin[j])
                flag_error=1;
        }
    }
    for (int i=0; i<Nro; i++){
        int j=jmin[i];
        point_minus_1[0][j]=point[0][i];
        point_minus_1[1][j]=point[1][i];
        if (abs(angle_minus_1[j]-angle[i])>M_PI*0.5){
             angle[i]+=M_PI;
//                        qDebug()<<"\t\tOrientation switched";
        }
        angle_minus_1[j]=angle[i];
    }
    for (int i=0; i<Nro; i++){
        point[0][i]=point_minus_1[0][i];
        point[1][i]=point_minus_1[1][i];
        angle[i]=angle[i];
    }
    return flag_error;
}

void MarangoniTracking::analizeImage(ImagePacket &currIm)
{
    if (currIm.pixFormat=="RGB8") {     //Gray Image
        cv::Mat grayIm;
        cv::cvtColor(currIm.image,grayIm,CV_RGB2GRAY);
        currIm.image=grayIm.clone();
        currIm.pixFormat="MONO8";
    }
    if (currIm.pixFormat=="MONO8") {

        cv::Mat processed;
        int newT=threshold*255/100.0;  //change here if it needs to work for 16-bit
        if ((myRegulator.prog_mode!=4))
        {
            cv::threshold ( currIm.image, processed, newT, 255, THRESH_BINARY_INV );;
        }
        else{
            cv::threshold ( currIm.image, processed, newT, 255, THRESH_BINARY);
        }


        //This is the openCv closing operation which according to this explanation
        //http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/doc/tutorials/imgproc/opening_closing_hats/opening_closing_hats.html
        //Should closing those white gaps in the obstacle, however is not working using the FitEllipse method
        int morph_size = 4;
        Mat element = getStructuringElement( MORPH_RECT , cv::Size( 2*morph_size + 1 , 2*morph_size +1), cv::Point( morph_size, morph_size ) );
        morphologyEx(processed,processed,  MORPH_CLOSE, element);


        std::vector<std::vector<cv::Point> > contours;
        cv::Mat hierarchy;
        cv::findContours( processed, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        cv::Mat outImage=currIm.image; // use a clone to avoid storing on raw image which is stored.
        //cv::drawContours(outImage,contours,-1,255,2);

        //cv::Point newLoc(targetX*currIm.image.cols/100.0,targetY*currIm.image.rows/100.0);
        //cv::ellipse(outImage,newLoc,cv::Size(5,5),0,0,360,150,-1);

        //std::cout << contours.size() << std::endl;
        //std::cout << contours[0] << std::endl;

        //qDebug()<<contours.size();
        double area;
        //qDebug()<<"Contour 0 area "<<area;
        double area_min;
        double area_max;
        if ((myRegulator.prog_mode!=4))
        {
            area_min=20;
            area_max=5000;
        }
        else{
            area_min=50;
            area_max=10e6;
        }
        int j=0;
        for (int i = 0; i < contours.size(); i++)
        {
            // Area between 20-50 that means is the particle

            area = cv::contourArea(contours[i]);
            if (area > area_min && area < area_max)
            {
                Moments mu = moments( contours[i], false );
                Point2f mc = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
                Ppoint[0][j] = mc.x;
                Ppoint[1][j] = mc.y;

                Pangle[j]=atan2(2*mu.mu11, mu.mu20-mu.mu02)/2.0;
                j++;
    //                    qDebug()<<"Orientation"<<QString::number(Pangle*180.0/M_PI);



    //                    cv::RotatedRect contours_part = cv::fitEllipse(Mat(contours[i]));
    //    //                cv::ellipse(outImage,contours_part,255,2);
    //    //                cv::Point center_contours_part(contours_part.center.x,contours_part.center.y);
    //    //                cv::circle(outImage,center_contours_part,1,255,-1);
    //                    Ppoint[0] = contours_part.center.x;
    //                    Ppoint[1] = contours_part.center.y;

                 //qDebug()<< "Pos" << Ppoint[0] << Ppoint[1] << "Cont size" <<contours[i].size() <<"Contour i area "<<area;

            }
            if (j>=nrParts)
                break;

        }
        if (firstScan){
            for (int i=0; i<nrParts; i++){
                Ppoint_minus_1[0][i]=Ppoint[0][i];
                Ppoint_minus_1[1][i]=Ppoint[1][i];
                Pangle_minus_1[i]=Pangle[i];
            }
            firstScan=0;
        } else{
            if (orderPart(Ppoint, Ppoint_minus_1, Pangle, Pangle_minus_1, nrParts))
                qDebug()<<"Error sorting the particles index";
        }
    }
}

void MarangoniTracking::drawLaser(Mat outImage)
{
    cv::Point figureCenter(myRegulator.x_las, myRegulator.y_las);

    if (myRegulator.pattern_type == 0){
        cv::circle(outImage, figureCenter, radius*0.2, cv::Scalar( 0, 0, 255 ), 2, 8, 0);       //DRAW LASER point on the Screen
    } else if (myRegulator.pattern_type == 1){                                                  //DRAW LASER PATTERN on the Screen TO CHECK
                        if (abs(myRegulator.pattern_aux) < 1.0){
                            cv::Size size(myRegulator.pattern_radius, myRegulator.pattern_radius);                                                        //regulation with a arc circle
                            int middleAngle = myRegulator.middleAngle*180/M_PI;
                            int patternAngle = abs(myRegulator.pattern_aux)*180;
                            cv::ellipse(outImage, figureCenter, size, 0, middleAngle - patternAngle, middleAngle + patternAngle, cv::Scalar( 0, 0, 255 ), 1, 8, 0);
                        }
                        else{
                            cv::circle(outImage, figureCenter, myRegulator.pattern_radius, cv::Scalar( 0, 0, 255 ), 1, 8, 0);       //objective reached. Stabilisation with circle
                        }
    }else if (myRegulator.pattern_type == 2){
        cv::Size size(myRegulator.pattern_radius, myRegulator.pattern_radius*myRegulator.pattern_aux);
        int middleAngle = myRegulator.middleAngle*180/M_PI+180;
        cv::ellipse(outImage, figureCenter, size, middleAngle, 0, 360, cv::Scalar( 0, 0, 255 ), 1, 8, 0);
    }else if (myRegulator.pattern_type == 3){
        cv::Size size(2.0*myRegulator.pattern_radius, 2.0*myRegulator.pattern_radius*myRegulator.pattern_aux);
        int middleAngle = myRegulator.middleAngle*180/M_PI+180;
        RotatedRect rRect = RotatedRect(figureCenter, size, middleAngle);
        Point2f vertices[4];
        rRect.points(vertices);
        for (int i = 0; i < 4; i++)
            line(outImage, vertices[i], vertices[(i+1)%4],cv::Scalar( 0, 0, 255 ), 1, 8, 0);
    }else if (myRegulator.pattern_type == 4){
        cv::Point P2 (myRegulator.x_las-myRegulator.pattern_radius*cos(myRegulator.middleAngle),myRegulator.y_las-myRegulator.pattern_radius*sin(myRegulator.middleAngle));
        cv::line(outImage, figureCenter, P2, cv::Scalar( 0, 0, 255 ), 1, 8, 0);
    }

}

void MarangoniTracking::drawTarget(Mat outImage)
{
    cv::Point targetPosition(targetX, targetY);
    cv::circle(outImage, targetPosition, radius*0.1, cv::Scalar( 96, 96, 96 ), -1, 8, 0);        //target position
}

void MarangoniTracking::drawParticles(Mat outImage)
{
    for (int i=0; i<nrParts; i++){
        cv::Point particlePos(Ppoint[0][i], Ppoint[1][i]);
        cv::Point particleOrientationPos(Ppoint[0][i]+radius*0.5*cos(Pangle[i]), Ppoint[1][i]+radius*0.5*sin(Pangle[i]));
        cv::circle(outImage, particlePos, radius*0.4, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //particle
        cv::line(outImage, particlePos, particleOrientationPos, cv::Scalar( 0, 0, 255 ), 1, 8, 0);        //particle orientation
        const std::string name=std::to_string(i+1);
        if (nrParts>1)
            cv::putText(outImage,name,particlePos, FONT_HERSHEY_SIMPLEX , 2,cv::Scalar( 0, 0, 255 ),1,8,0);
    }
}

void MarangoniTracking::drawObstacle(Mat outImage)
{
    cv::circle(outImage, cv::Point(myRegulator.x_obst, myRegulator.y_obst), radius*0.1, cv::Scalar( 255, 255, 255 ), 5, 8, 0);
}

QString MarangoniTracking::setdata()
{
    QString currentData=(QString::number(Ppoint[0][0])          //x_particle (in px)
    +"\t"
    +QString::number(Ppoint[1][0])         //y_particle
    +"\t"
    +QString::number(Pangle[0])         //angle_particle
    +"\t"
    //+QString::number(myRegulator.laser_x)     //x position of the center of the pattern
    +QString::number(myRegulator.x_las)     //x position of the center of the pattern
    +"\t"
    //+QString::number(myRegulator.laser_y)     //y position of the center of the pattern
    +QString::number(myRegulator.y_las)     //y position of the center of the pattern
    +"\t"
    +QString::number(myRegulator.middleAngle)     //angle of the figure
    +"\t"
    +QString::number(myRegulator.pattern_radius)     //radius of the figure
    +"\t"
    +QString::number(myRegulator.pattern_aux)     //aux variable
    +"\t"
    +QString::number(myRegulator.u_Final)     //u_dot_aux value
    +"\t"
    +QString::number(myRegulator.theta_corr_Final)  //time of the picture
    +"\t"
    //+QString::number(myRegulator.target_x)     //x target
    +QString::number(targetX)     //x target
    +"\t"
    //+QString::number(myRegulator.target_y)     //y target
    +QString::number(targetY)     //y target
    +"\t"
    +QString::number(myRegulator.x_obst)     //x obstacle
    +"\t"
    +QString::number(myRegulator.y_obst)     //y obstacle
    +"\t"
    +QString::number(myRegulator.objectifReached)     //objective reached (1: true, 0: false)
    +"\t"
    +QString::number(timeStamp,'f',0)  //time of the picture
    +"\t"
    +QString::number(myRegulator.prog_mode)  //Current mode 0 identification 1 Feedback control 2 Feedback+Feedforward
    +"\t"
    +QString::number(myRegulator.pattern_type)  //Current pattern type
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

    return currentData;
}

QString MarangoniTracking::runMirrorCalibration()
{
    int n_points_x=21;
    int n_points_y=21;
    double Vx_max=0.6;
    double Vy_max=0.45;
    int wait_time=500;
    static float Vx;
    static float Vy;
    static int i=0;
    static int j=0;
    QString currentData;
    if ((j<n_points_y)||(i<=0)){
        if ((i!=0)||(j!=0)){  //No first iteration
            currentData=(QString::number(i+n_points_x*j-1)
            +"\t"
            +QString::number(Vx)
            +"\t"
            +QString::number(Vy)
            +"\t"
            +QString::number(Ppoint[0][0])         //x_particle (px)
            +"\t"
            +QString::number(Ppoint[1][0])         //y_particle (px)
            +"\r\n");
        }
        if (j<n_points_y){
            if(j%2==0){
                Vx=Vx_max*(((double)(2*i))/((double)(n_points_x-1))-1.0);
            }else{
                Vx=Vx_max*(1.0-((double)(2*i))/((double)(n_points_x-1)));
            }
            Vy=Vy_max*(((double)(2*j))/((double)(n_points_y-1))-1.0);
            mirCtrl.ChangeMirrorVoltage(Vx,Vy);
        }
        i++;
        if (i>=n_points_x){
        i=0;
        j++;
        }
#ifdef Q_OS_WIN32
        Sleep(wait_time);
#else
        usleep(wait_time*1000);  /* sleep for 300 milliSeconds */                     //... No new postion of the laser is required when closing connection...
#endif
    }
    return currentData;
}


#ifdef Q_OS_WIN32
bool MarangoniTracking::initializeMirror(){
    bool properConnection = mirCtrl.Initialisation(SampleRate);                        //initialisation of the mirror
    return properConnection;
}

void MarangoniTracking::closeMirror() {
    qDebug()<<"Closing the mirror";
    mirCtrl.Closing();                                                  //Close the mirror
}
#endif
