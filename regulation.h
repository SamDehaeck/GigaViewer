#ifndef REGULATION_H
#define REGULATION_H

class Regulation
{

private:

    int flag;

    int ptsNumb;
    int vectorLength;

    double k_p;
    double k_i;
    double T_i;                                                //Regulator parameters
    double k_t;
    double T_s;

    float start_x;
    float start_y;

    int counter;

    float u_i;                                                //internal states of the regulator
    float u_sat;
    double u0;
    double u1;
    double e0;
    double e1;

    //ALWAYS 100
    float figure_x[100];                                            //Variables used for the figure drawing
    float figure_y[100];
    //ALWAYS 100

    int regulation_type;                                            //used to define which type of figure is used
    int target_type;                                                //used to define if a step or a traking reference is used (step = 0, tracking = 1)
    int increment;
    float radius;

    float getAlpha (float thing_part_x, float thing_part_y);

public:
    Regulation();
    void Configure (int type_regulation, int type_target, float r, int x_target, int y_target); 	//Must be called when "track" is pressed
    void Regulator2016(float particle_x, float particle_y);             //Must be called when the position of the particule moves...
                                                                        //Dimitri's version
    void Regulator2017(float particle_x, float particle_y);             //Must be called when the position of the particule moves
                                                                        //Adrien's version

    float laser_x;                                                        //For point control, correspond to the new position of the laser...
    float laser_y;                                                        //...for the figure control, correspond to the center of the figure

    float u;

    float target_x;                                                  //general variables which represent the desired positions
    float target_y;

    float x_vector[100];                                             //100 for 180°, 68 for 122.4° and 32 for 57.6°
    float y_vector[100];

    float middleAngle;

    bool objectifReached;
};

#endif // REGULATION_H
