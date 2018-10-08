#include <iostream>
#include <QtCore>
#include <QObject>
//#include <boost/math/special_functions/bessel.hpp>

#ifndef REGULATION_H
#define REGULATION_H

class Regulation
{

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


    //ALWAYS 100
    float figure_x[100];                                            //Variables used for the figure drawing
    float figure_y[100];

    //ALWAYS 100



    int increment;
    float radius;

    float getAlpha (float thing_part_x, float thing_part_y);
    float velocity2distance(float v);

public:
    Regulation();
    void Configure (int type_regulation, int type_target, float r); 	//Must be called when "track" is pressed
    void Configure_FB (int program_ID, float r_Figure, int x_target, int y_target, float kp, float Ti, float Td, float Tt, float Usat, float Tsamp);  //OverCharged Version that accepts more inputs

    void Configure_FF(float x_obstacle, float y_obstacle, float Kff);   //To configure the FeedFoward controller only
    void Regulator2016(float particle_x, float particle_y);             //Must be called when the position of the particule moves...
                                                                        //Dimitri's version
    void Regulator2017(float particle_x, float particle_y);             //Must be called when the position of the particule moves
                                                                        //Adrien's version
    void run_Cont_FB(float x_particle, float y_particle, float x_targ, float y_targ);               //General routine to compute the error and the input of the FeedBack controller
    void run_Cont_FF(float x_particle, float y_particle, float x_targ, float y_targ);              //General routine to compute the error and the input of the FeedForward controller

    void run_Ident(float x_particle, float y_particle, float dist_LasPart, float radius_pattern, float aux_pattern, int direction);
    void run_Traj_Generator(float x_particle, float y_particle, float x_targ, float y_targ);      //Routine to create and ASSIGN the required points of a trajectory

    bool run_Shielding(float x_particle, float y_particle, float x_particle2, float y_particle2, float dlim, float radius);
    bool run_SmartShielding(float x_particle, float y_particle, float x_particle2, float y_particle2, float x_targ, float y_targ, float dlim, float radius);

    void get_Laser_Position(float particle_x, float particle_y, float x_targ, float y_targ);
    void compute_Laser_Pattern(int patterntype, float X_laser_given, float Y_laser_given, float A_laser_given, float R_pattern_given, float A_pattern_given);

    void resetIntegratorVariables();

    int regulation_type, pattern_type;                                            //used to define which type of figure is used
    int target_type, prog_mode;                                                //used to define if a step or a traking reference is used (step = 0, tracking = 1)

    float laser_x, x_las;                                                        //For point control, correspond to the new position of the laser...
    float laser_y, y_las;                                                        //...for the figure control, correspond to the center of the figure

    float pattern_radius, pattern_aux;

    QList<float> x_traj;                                    //Lists used to define the trajectory to the goal
    QList<float> y_traj;

    float u;

    float x_obst;                                                //variables for the feedforward
    float y_obst;

    float x_vector[100];                                             //100 for 180°, 68 for 122.4° and 32 for 57.6°
    float y_vector[100];

    float middleAngle;
    float Factor_pixTOmm;                           //Factor to convert
    int FRAMECOUNTER;                               //Variable to detect when 1 second has passed

    bool objectifReached;

    bool Flag_LinearTracking;                       //Flag to determine if we are tracking a trajectory or not

    //NEW ERROR and INPUT variables for my program Ronald 2018

    double u_0;  double u_minus_1;   double u_minus_2;   // u(k)  u(k-1)*JUST used to keep track nothing else   u(k-2)
    double e_0;     double e_minus_1;   double e_minus_2;       // e(k)   e(k-1)    e(k-2)
    double e_sat_0;     double e_sat_minus_1;   double e_sat_minus_2;  // E_sat(k)   E_sat(k-1)   E_sat(k)     where E_sat = U_sat - u(k)   U_sat is the maximum input we can command to the system


    double e_ang, theta_corr_FB;
    float x_part_INI, y_part_INI;                       //In the new strategy I use the initial Part-Target vector as a reference


    double u_0_FF;
    double theta_corr_FF, theta_corr;    //Error and input for the angle

    double u_Final, theta_corr_Final;   //Final u values to send to the controller


};

#endif // REGULATION_H
