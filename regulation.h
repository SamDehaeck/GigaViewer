#include <iostream>
#include <QtCore>
#include <QObject>
#include "pathfinding.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>

//#include <boost/math/special_functions/bessel.hpp>

#ifndef REGULATION_H
#define REGULATION_H

#define MODE_MANUAL 0
#define MODE_MIRROR 1
#define MODE_IDENT 2
#define MODE_FB 3
#define MODE_FF 4
#define MODE_SHIELDING 5
#define MODE_CLOSEDPATH 6
#define MODE_PATHFIND 7
#define MODE_MULTIPLEX 8

#define PATTERN_POINT 0
#define PATTERN_ARC 1
#define PATTERN_ELLIPSE 2
#define PATTERN_RECTANGLE 3
#define PATTERN_LINE 4

#define IDENT_BOTTOM 0
#define IDENT_RIGHT 1
#define IDENT_TOP 2
#define IDENT_LEFT 3

class Regulation: public QObject
{
    Q_OBJECT
private:

    int flag;

    int ptsNumb;
    int vectorLength;

    double k_p;         //Proportional gain
    double T_d;         //Derivative gain
    double T_i;         //Integral gain

    double T_t;         //Antiwindtup gain
    double T_s;         //Sampling time
    double k_ff;        //Feedforward gain

    double FB_C1;
    double FB_C2;
    double FB_C3;

    float start_x;
    float start_y;

    int counter;

    float u_i;                                                //internal states of the regulator
    double u_sat;
    double U_sat_global;                                        //The gloval unput limit that is given by the user

    double u0;                                          //Varibles used by Dimitri and Adrien
    double u1;
    double e0;
    double e1;




    //float x_part_BEF, y_part_BEF;               //Variables to store the previous particle position as it is required to compute the particle velocity
    float x_part_minus_1, x_part_minus_2;
    float y_part_minus_1, y_part_minus_2;



    int increment;

    float getAlpha (float thing_part_x, float thing_part_y);
    float velocity2distance(float v);

    pathfinding myPathFinding;
    bool firstpath, pathready, pathisvalid;
    QFutureWatcher<bool> Path_watcher;
    QFuture<bool> Path_future;

    float shield_dLim, shield_fHys;    
    bool shield_HysFlag[10];

    int path_searchspace;



public:
    Regulation();
    void Configure_Figure(float r_Figure, float a_Figure);
    void Configure_FB (float kp, float Ti, float Td, float Tt, float Usat, float Tsamp);  //OverCharged Version that accepts more inputs

    void Configure_FF(float x_obstacle, float y_obstacle, float Kff);   //To configure the FeedFoward controller only
//    void Regulator2016(float particle_x, float particle_y);             //Must be called when the position of the particule moves...
                                                                        //Dimitri's version
//    void Regulator2017(float particle_x, float particle_y);             //Must be called when the position of the particule moves
                                                                        //Adrien's version
    void run_Cont_FB(float x_particle, float y_particle, float x_targ, float y_targ);               //General routine to compute the error and the input of the FeedBack controller
    void run_Cont_FB_vel(float x_particle, float y_particle);
    void run_Cont_FF(float x_particle, float y_particle, float x_targ, float y_targ);              //General routine to compute the error and the input of the FeedForward controller

    void run_Ident(float x_particle, float y_particle, float dist_LasPart, float radius_pattern, int direction,float A_pattern_given);
//    void run_Traj_Generator(float x_particle, float y_particle, float x_targ, float y_targ);      //Routine to create and ASSIGN the required points of a trajectory
    void run_ManualMode(float X_laser_given, float Y_laser_given, float A_laser_given, float R_pattern_given, float A_pattern_given);


    bool run_Shielding(float x_particle, float y_particle, float x_particle2, float y_particle2, float x_targ, float y_targ);
    bool run_Shielding_mult(float partX[], float partY[], float x_targ, float y_targ, int NrPart, int ContPart);
    bool run_SmartShielding(float x_particle, float y_particle, float x_particle2, float y_particle2, float x_targ, float y_targ);
    bool run_SmartShielding_mult(float partX[], float partY[], float x_targ, float y_targ, int NrPart, int ContPart);

    void get_Laser_Position(float particle_x, float particle_y);
    void get_Laser_Position_vel(float x_particle, float y_particle);
    void compute_Laser_Pattern(int patterntype, float X_laser_given, float Y_laser_given, float A_laser_given, float R_pattern_given, float A_pattern_given);
    void compute_Laser_Pattern_INT();
    void resetIntegratorVariables();

    void multiplexLAserPosition(int index);
    void demultiplexLAserPosition(int index);

    void computeClosedPath();
    void computeOpenPath();

    void run_ClosedPath_Following(float x_particle, float y_particle);
    void run_OpenPath_Following(float x_particle, float y_particle);

    void FindPath(float partX[], float partY[], float xtarg, float ytarg, int Nrpart, int ContPart);
    bool PathFinding_run();
    void PathFinding_sendPosI(float x, float y);
    void PathFinding_sendTarget(float x, float y);
    void PathFinding_addObs(float x, float y);
    void PathFinding_clearObs();

    int regulation_type, pattern_type;                                            //used to define which type of figure is used
    int target_type, prog_mode;                                                //used to define if a step or a traking reference is used (step = 0, tracking = 1)

    float x_las;                                                        //For point control, correspond to the new position of the laser...
    float y_las;                                                        //...for the figure control, correspond to the center of the figure
    float middleAngle;

    float pattern_radius, pattern_aux;

    float x_las_mult[10];
    float y_las_mult[10];
    float middleAngle_mult[10];
    float pattern_radius_mult[10];
    float pattern_aux_mult[10];
    int nrLaserSpot;

    QList<float> x_traj;                                    //Lists used to define the trajectory to the goal
    QList<float> y_traj;

    float u;

    float x_obst;                                                //variables for the feedforward
    float y_obst;

    float x_vector[100];                                             //100 for 180°, 68 for 122.4° and 32 for 57.6°
    float y_vector[100];


    float Factor_pixTOmm;                           //Factor to convert
    int FRAMECOUNTER;                               //Variable to detect when 1 second has passed

    bool objectifReached;

    bool Flag_LinearTracking;                       //Flag to determine if we are tracking a trajectory or not

    //NEW ERROR and INPUT variables for my program Ronald 2018

    double u_0;  double u_minus_1;   double u_minus_2;   // u(k)  u(k-1)*JUST used to keep track nothing else   u(k-2)
    double u_path;
    double e_0;     double e_minus_1;   double e_minus_2;       // e(k)   e(k-1)    e(k-2)
    double e_sat_0;     double e_sat_minus_1;   double e_sat_minus_2;  // E_sat(k)   E_sat(k-1)   E_sat(k)     where E_sat = U_sat - u(k)   U_sat is the maximum input we can command to the system


    double e_ang, theta_corr_FB;
    float x_part_INI, y_part_INI;                       //In the new strategy I use the initial Part-Target vector as a reference


    double u_0_FF;
    double theta_corr_FF, theta_corr, theta_part_targ;    //Error and input for the angle

    double u_Final, theta_corr_Final;   //Final u values to send to the controller

    float path_x[200];                                            //Variables used for the path following
    float path_y[200];
    int path_N;
    float path_A[200];
    float path_D[200];
    float path_Dac[200];
    float path_K[200];
    float path_cA;
    float path_cL;
    float path_cD;
    float path_totD;
    float path_meanD;
    int path_PrevIndex;

public slots:
    void PathIsReady();


};

#endif // REGULATION_H
