#ifndef REGULATION_H
#define REGULATION_H

#include "mirrorcontrol.h"

//CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
#include <QtCore>
//CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//

class Regulation
{

private:

    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//
    QString dataToSave;
    //CODE TO DELETE - USED TO VERIFY THE FUNCTIONING//

    int ptsNumb;
    int vectorLength;

    //const float kp;                                               //Regulator parameters
    //const flaot ki;
    //const flaot kd;

    float figure_x[100];                                            //Variables used for the figure drawing
    float figure_y[100];
    float x_vector[100];
    float y_vector[100];

    float d_pl;                                                     //Distance between the particle and the laser
    float d_pc;                                                     //Distance between the center of the circle and the particle
    int type;                                                       //used to define which type of figure is used
    float radius;
    int flag;

    MirrorControl mirCtrl;                                          //Mirror object used to access to the MirrorControl functions

public:
    Regulation ();
    bool Initialisation();                                          //Used to initialize the MEM
    void Figure (int type_recieved, float r, int x_desired, int y_desired); 	//Must be called when "track" is pressed
    void Regulator(float particle_x, float particle_y);             //Must be called when the position of the particule moves
    void closeRegulation ();                                        //Must be called when the the experiment is over to close the mirror

    float x;                                                        //For point control, correspond to the new position of the laser...
    float y;                                                        //...for the figure control, correspond to the center of the figure

    int desired_x;                                                  //general variables which represent the desired positions
    int desired_y;

    float middleAngle;
};

#endif // REGULATION_H
