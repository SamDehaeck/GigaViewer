#ifndef REGULATION_H
#define REGULATION_H

#include "mirrorcontrol.h"

class Regulation
{

private:

    //const float kp;                                               //Regulator parameters
    //const flaot ki;
    //const flaot kd;

    int desired_x;                                                  //general variables which represent the desired positions
    int desired_y;

    int type;                                                      //used to define which type of figure is used

    void Circle ();                                                 //Functions used for the figure drawing
    void SemiCircle ();
    float figure_x[100];                                            //Variables used for the figure drawing
    float figure_y[100];
    float x_vector[100];
    float y_vector[100];

    float x;                                                        //For point control, correspond to the new position of the laser...
    float y;                                                        //...for the figure control, correspond to the center of the figure

    MirrorControl mirCtrl;                                          //Mirror object used to access to the MirrorControl functions

public:
    Regulation ();
    void Initialisation();                                          //Used to initialize the MEM
    bool properConnection;                                          //Used to verify if the connection with the MEMs is ok in MarrandoniTracking.cpp
    void Figure (int type_recieved, int x_desired, int y_desired); 	//Must be called when "track" is pressed
    void Regulator(float particle_x, float particle_y);             //Must be called when the position of the particule moves
    void closeRegulation ();                                        //Must be called when the the experiment is over to close the mirror
};

#endif // REGULATION_H
