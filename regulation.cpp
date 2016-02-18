#include "regulation.h"
#include <QDebug>

Regulation::Regulation(): mirCtrl(){                                    //Creation of the regulation object
    qDebug()<<"Creation of Regulation object";

    //const float kp =0;                                                //Creation of the regulator parameters
    //const flaot ki =0;
    //const flaot kd =0;
}

void Regulation::Initialisation(){
    properConnection = mirCtrl.Initialisation();                        //initialisation of the mirror
}


void Regulation::Figure (int type_recieved, int x_desired, int y_desired){

    desired_x = x_desired; desired_y = y_desired; type = type_recieved;
    if (type == 1){
        Circle ();
        qDebug()<<"Creation of the circle";
    }
    else if (type == 2){
        SemiCircle ();
        qDebug()<<"Creation of the semi-circle";
    }
    else{
        qDebug()<<"No figure creation";
    }
}


void Regulation::Circle (){

    const int ptsNumb = 100;
    float stepAngle = (float)M_PI*2 / ptsNumb;
    int i;
    for( i = 0; i < ptsNumb; i++ ){
        figure_x[i] = 0.15*sin( i * stepAngle );
        figure_y[i] = 0.15*cos( i * stepAngle );
        }
}


void Regulation::SemiCircle (){

    const int ptsNumb = 100;
    float stepAngle = (float)M_PI*2 / ptsNumb;
    int i;
    for( i = 0; i < ptsNumb; i++ ){
        figure_x[i] = 0.15*sin( i * stepAngle );
        figure_y[i] = fabs(0.15*cos( i * stepAngle ));
        }
}


void Regulation::Regulator (float particle_x, float particle_y){

    if (type == 1 || type == 2){                                        //SendDataStream is used - Use of circle or half-circle control

        //REGULATION    //use of particule_x, particule_y, desired_x and desired_y
        //REGULATION    //gives the new center of the figure ==> float x and float y
        //REGULATION

        //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
        x=particle_x;
        y=particle_y;
        //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//

        for(int j=0; j<100; j++){
            x_vector[j] = figure_x[j] + x;
            y_vector[j] = figure_y[j] + y;
        }
        mirCtrl.ChangeMirrorStream (x_vector, y_vector);
        }

    else if (type == 0){                                                //GoToDevicePosition is used - Use of a single point control

        //REGULATION
        //REGULATION    gives the new position x and y of the laser ==> float x and float y
        //REGULATION

        //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
        x=particle_x;
        y=particle_y;
        //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//

        mirCtrl.ChangeMirrorPosition(x, y);
    }
}


void Regulation::closeRegulation (){
    qDebug()<<"Closing the mirror";
    mirCtrl.Closing();                                                  //Close the mirror
}
