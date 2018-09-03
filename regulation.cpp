#include "regulation.h"
#include <QDebug>




Regulation::Regulation() {                                        //Creation of the regulation object
    qDebug()<<"Creation of Regulation object";

    ptsNumb = 100;                                                          //Cutting the circle into ptsNumb portions

    vectorLength = 100;                                                     //100 for 180°, 68 for 122.4° and 32 for 57.6°

    Factor_pixTOmm = 0.064453;                  //Factor to convert pixels to mm = 65/1024
    x_part_minus_1=0; y_part_minus_1 =0;                //Variable to store the previopus particle position to compute the Particle velocity
    x_part_minus_2=0; y_part_minus_2 =0;


    k_p =2.6653;
    T_i =20.2429;                                                             //Creation of the regulator parameters
    T_t =0.00164;
    T_s = 0.0333;
    k_ff= 0.0;      //Feedforward controller gain is set by default to 0

    x_obst = 545;                                     //position of the obstacle
    y_obst = 298;                       //For now, I just replace the default position found by Adrien, but in the future, this should always be inserted in the program


}


void Regulation::Configure (int type_regulation, int type_target, float r, int x_target, int y_target){

    regulation_type = type_regulation; target_type = type_target; radius = r;
    increment = 0;
    counter = 0;

    //k_p =kp;    T_i =Ti;    T_d = Td; T_t =Tt;    T_s = Tsamp;  k_ff = Usat;

    //        k_p =2.6653; T_i =20.2429;  T_t =0.00164; T_s = 0.0666; 15FPS for some reasons   k_ff = 0.7;
            k_p =2.6653;
            T_i =20.2429;                                          //Creation of the regulator parameters
            T_t =0.00164;
            T_s = 0.0666;                                          //IDS limited to 15FPS for some reasons
            k_ff = 0.7;


    //Arc + Circle control strategy DIMITRI
    if( (regulation_type == 1) || (regulation_type == 2) ){
        qDebug() << "Creation of the figure";
        float stepAngle = (float)M_PI*2 / ptsNumb;
        for( int i = 0; i < ptsNumb; i++ ){
            figure_x[i] = radius*cos( i * stepAngle );
            figure_y[i] = radius*sin( i * stepAngle );
        }
    }
    else{
        qDebug()<<"Use of point actuation. No figure creation";
    }

    if( target_type == 0){                                                      //step reference
        target_x = x_target;
        target_y = y_target;
    }
    else{
        flag = 1;
    }

    u_i = 0;
    u = 0;                                                                      //reset of internal states of the regulator and actuation variable
    u_sat = 0;
    objectifReached = false;


    //parameters for Regulator2017 Dot only ADRIEN
    if (regulation_type == 3) {
//        k_p =2.6653;
//        T_i =20.2429;                                          //Creation of the regulator parameters
//        T_t =0.00164;
//        T_s = 0.0666;                                          //IDS limited to 15FPS for some reasons
//        k_ff = 0.7;

        u0 = 0;          // state u(k)
        u1 = 0;          // state u(k-1)
        e0 = 0;          // state e(k)
        e1 = 0;          // state e(k-1)

        x_obst = 545;                                     //position of the obstacle
        y_obst = 298;
    }
}



void Regulation::Configure_FB (int program_ID, float r_Figure, int x_target, int y_target, float kp, float Ti, float Td, float Tt, float Usat, float Tsamp){

     //prog_mode = program_ID/100;        // 0 for identification    1 for feedback control   2 Feedback+Feedforward control
     //pattern_type = program_ID%100;     // 0 for point-wise pattern     1 for arc+circle pattern

     target_type=0;                 //NOTE THIS IS JUST TO CHECK WHERE this variable gets its weird value

     radius = r_Figure;
    increment = 0;
    counter = 0;
    k_p =kp;    T_i =Ti;    T_d = Td;  T_s = Tsamp;  //Feedback Controller gains and sampling time
    T_t =Tt;        //Anti-windup gain and Input saturation



    //Arc + Circle control strategy DIMITRI
    if( (regulation_type == 1) || (regulation_type == 2) ){
        qDebug() << "Creation of the figure";
        float stepAngle = (float)M_PI*2 / ptsNumb;
        for( int i = 0; i < ptsNumb; i++ ){
            figure_x[i] = radius*cos( i * stepAngle );
            figure_y[i] = radius*sin( i * stepAngle );
        }
    }
    else{
        qDebug()<<"Use of point actuation. No figure creation";
    }

//    if( target_type == 0){                                                      //step reference
        x_targ = x_target;
        y_targ = y_target;
//    }
//    else{
//        flag = 1;                                               //Trajectory tracking mode TO BE IMPLEMENTED
//    }

    u_i = 0;
    u = 0;
    u_0=0.0;    u_minus_1=0.0;   u_minus_2=0.0;
    u_0_FF = 0.0;
    e_0 = 0.0;  e_minus_1=0.0;  e_minus_2=0.0;
    e_sat_0=0.0;    e_sat_minus_1=0.0;  e_sat_minus_2=0.0;
    e_ang=0.0;      theta_corr_FB = 0.0;    theta_corr_FF = 0.0;    theta_corr=0.0;
    x_part_minus_1=0.0; y_part_minus_1=0.0;
    x_part_minus_2=0.0; y_part_minus_2=0.0;
                            //reset of internal states of the regulator and actuation variable

    u_sat = 0;
    U_sat_global = Usat;                                    //The user assigns the saturation value. By default it should be 5.4
    objectifReached = false;

    FRAMECOUNTER=1;                                         //For every new target position, we must set the framecounter to 0 again
    x_traj.clear();  y_traj.clear();                        //It is also necesary to empty the lists containing the trajectory points


}


void Regulation::Configure_FF(float x_obstacle, float y_obstacle, float Kff)
{
    k_ff = Kff;     //Feedforward gain. In case some value is here, It will just be neglected by the program
    //To avoid problems in the file saving, I say that u_0_FF and theta_corr_FF are 0 on the configuration of the FB controller


    x_obst = 545;                                     //position of the obstacle
    y_obst = 298;                       //For now, I just replace the default position found by Adrien, but in the future, this should always be inserted in the program



}

void Regulation::Regulator2017 (float particle_x, float particle_y){

    //target positions
    if (target_type == 1){                                                       //tracking reference. target_x and y are computed at each step
        if (flag == 1){                                                          //start the tracking at the current particle position
            start_x = particle_x;
            start_y = particle_y;
            flag = 0;
        }
        increment++;

        target_x = start_x + 200*cos(2*3.1416*0.02*increment*T_s);               //circle at f=0.02 Hz
        target_y = start_y + 200*sin(2*3.1416*0.02*increment*T_s);

//        target_x = start_x + 4*15.7538*T_s*increment;                                      //horizontal line 4mm/s)
//        target_y = start_y ;
    }


    //****************************** PID *******************************//


    //update of the (k-1) states
//    e1 = e0;                                                                        //for method without anti-windup
//    u1 = u0;

    //error calculation (pixels to mm)
    float x_error = 0.064453*target_x - 0.064453*particle_x;              //results in mm
    float y_error = 0.064453*target_y - 0.064453*particle_y;

    //conversion cartesian to polar coordinates
    e0 = sqrt((x_error*x_error) + (y_error*y_error));            //radial error
    float alphaPID = getAlpha(x_error,y_error);

    //controller PID
//    u0 = u1 + (k_p + k_i*(T_s/2)) * e0 + (-k_p + k_i*(T_s/2)) * e1;  //in [mm/s]     //for method without anti-windup

    //proportional action
    float u_p = k_p/(2*T_i) * (T_s + 2*T_i) * e0;         //u_p(k) = coeff * e(k)
    u0 = u_i + u_p;                                       //u(k) = up(k) + ui(k)

    //saturation
    double u_sat_PID;
    if (u0 >= 5.4){
        u_sat_PID = 5.4;
    }
    else if (u0 <= -5.4){
        u_sat_PID = -5.4;
    }
    else{
        u_sat_PID = u0;
    }
    std::cout << "PID : " << u0 << std::endl;
    std::cout << "PID sat : " << u_sat_PID << std::endl;

    //integral action with anti-windup
    u_i = u_i + (k_p*T_s/T_i) * e0 - T_t*(u0 - u_sat);   //u_i(k+1) = ui(k) + smth * e(k) - kt*(u(k) - u_sat(k))


    // conversion polar to cartesian coordinates
    // the vector of that speed has to be add to the speed computed by the FF
    double u_PID_x = u_sat_PID * cos(alphaPID);
    double u_PID_y = u_sat_PID * sin(alphaPID);


    //************************** FEED FORWARD **************************//


    //distance between obstacle and particle (pixels to mm)
    float distFF_x = 0.064453*x_obst - 0.064453*particle_x;      //results in mm
    float distFF_y = 0.064453*y_obst - 0.064453*particle_y;

    //conversion cartesian to polar coordinates
    float distFF = sqrt((distFF_x*distFF_x) + (distFF_y*distFF_y));      //radial distance in mm
    float alphaFF = getAlpha(distFF_x,distFF_y);

    //opposite Kralchevsky axial force (from experimental model (fit exponential)) - input : [m], output : [N]
    double force_K =  5.8329 * pow(10,-7) * exp(-168.8263*distFF*0.001)
                      + 1.0887 * pow(10,-5) * exp(-808.8847*distFF*0.001);

    //opposite Kralchevsky axial force (from experimental model (fit Bessel)) - input : [m], output : [N]
//    double a = 9.8888 * pow(10,-6);
//    double b = 800.5627;
//    double bessel = boost::math::cyl_bessel_k(1,b*distFF*0.001);
//    double force_K = a*bessel;

    //opposite Kralchevsky axial force (theoretical model) - input : [m], output : [N]
//    double a = 1.4384 * pow(10,-6);
//    double b = 370.3704;
//    double bessel = boost::math::cyl_bessel_k(1,b*distFF*0.001);
//    double force_K = a*bessel;

    //Stokes to convert force_K to velocity
    double uFF = force_K / (6 * (double)M_PI * 1.0016 * pow(10,-3) * 0.0005);  // [m/s]
    uFF = uFF * 1000; // [mm/s]
    uFF = k_ff * uFF;   // proportional coefficient

    //saturation
    double u_sat_FF;
    if (uFF >= 5.4){
        u_sat_FF = 5.4;
    }
    else if (uFF <= -5.4){
        u_sat_FF = -5.4;
    }
    else{
        u_sat_FF = uFF;
    }


    std::cout << "FF : " << u_sat_FF << std::endl;

    // conversion polar to cartesian coordinates
    // the vector of that speed has to be add to the speed computed by the PID
    double u_FF_x = u_sat_FF * cos(alphaFF);
    double u_FF_y = u_sat_FF * sin(alphaFF);

    //********************** COMBINATION PID-FF ************************//

    //vectors addition
    double u_x = u_PID_x + u_FF_x;
    double u_y = u_PID_y + u_FF_y;

    //conversion cartesian to polar coordinates
    u = sqrt((u_x*u_x) + (u_y*u_y));      //radial distance in mm
    double alpha = getAlpha(u_x,u_y);     //orientation

    //saturation
    if (u >= 5.4){
        u_sat = 5.4;
    }
    else if (u <= -5.4){
        u_sat = -5.4;
    }
    else{
        u_sat = u;
    }
    std::cout << "total : " << u_sat << std::endl;

    //u_steady-state to r_las-part
    double r_las_part;
    if (u_sat >= 0){
        r_las_part = (-1/1.433)*log(u_sat/26.086);
    }
    else{
        r_las_part = (-1/1.433)*log(-u_sat/26.086);
    }
    alpha = alpha + (float)M_PI;

    //convert back in cartesian coordinates and in pixels for the mirror
    //and use the particle as reference
    laser_x = r_las_part * cos(alpha) * 15.515 + particle_x;
    laser_y = r_las_part * sin(alpha) * 15.515 + particle_y;

}




float Regulation::getAlpha (float thing_part_x, float thing_part_y) {
    //take the distance between something and the particle in x and y, and return the angle alpha
    //with respect to the x-axis
    float alpha;
    if (thing_part_x > 0){
        alpha = atan((thing_part_y/thing_part_x));
    }
    else if (thing_part_x < 0){
        alpha = (float)M_PI + atan((thing_part_y/thing_part_x));
    }
    else{
        if (thing_part_y > 0){
            alpha = (float)M_PI/2;
        }
        else if (thing_part_y < 0){
            alpha = - (float)M_PI/2;
        }
        else{
            alpha = 0;
        }
    }
    return alpha;
}








// Dimitri's version
void Regulation::Regulator2016 (float particle_x, float particle_y){

    //target positions
    if (target_type == 1){                                                       //tracking reference. target_x and y are computed at each step
        if (flag == 1){                                                         //start of the tracking at the current particle position
            start_x = particle_x;
            start_y = particle_y;
            flag = 0;
        }
        increment++;
        target_x = start_x + 200*sin(0.05*increment);                                                  //parameterisation
        target_y = start_y + 200*sin(0.05*increment)*cos(0.05*increment);
    }

    //error calculation
    float x_error = 0.064453*target_x - 0.064453*particle_x;                             //results in mm
    float y_error = 0.064453*target_y - 0.064453*particle_y;
    float error = sqrt((x_error*x_error) + (y_error*y_error));                          //radial error

    //angle orientation and r_particle_target calculation
    float alpha;
    if (x_error > 0){
        alpha = atan((y_error/x_error));
    }
    else if (x_error < 0){
        alpha = (float)M_PI + atan((y_error/x_error));
    }
    else{
        if (y_error > 0){
            alpha = (float)M_PI/2;
        }
        else if (y_error < 0){
            alpha = - (float)M_PI/2;
        }
        else{
            alpha = 0;
        }
    }
    middleAngle = (float)M_PI + alpha;


    //controller
    if (target_type == 1){
        //u_i = u_i + (k_p*T_s/T_i) * error - T_t*(u - u_sat);
    }
    float u_p = k_p/(2*T_i) * (T_s + 2*T_i) * error;
    u = u_i + u_p;
    if (u >= 7){
        u_sat = 7;
    }
    else if (u <= -7){
        u_sat = -7;
    }
    else{
        u_sat = u;
    }

    //u_steady-state to r_las-part
    double r_las_part;
    if (u_sat >= 0){
        r_las_part = (1/1.678)*log(24.93/u_sat)*1;
    }
    else{
        r_las_part = (1/1.678)*log(-u_sat/24.93)*1;
    }

    //objective nearly reached. Use of a circle to stop the particle within the vicinity of the objective
    if (abs(x_error) < (radius*0.064453) && abs(y_error) < (radius*0.064453) && target_type ==0 ){
        objectifReached = true;
        counter++;
    }

    if (regulation_type == 0) {
        laser_x = r_las_part * cos(alpha) * 15.515 + particle_x;
        laser_y = r_las_part * sin(alpha) * 15.515 + particle_y;
    }

    else {

        //r_las-part to r_part_center
        float r_part_center = radius - (r_las_part * 15.515);               //in pix

        //r_part_center to x/y_part_center
        float x_part_center = r_part_center * cos(alpha);
        float y_part_center = r_part_center * sin(alpha);

        //computation of the new center figure
        laser_x = particle_x + x_part_center;
        laser_y = particle_y + y_part_center;

    }


    //Transfer new data stream to the mirror

    if (regulation_type == 0){          //Point actuation

    }

    else if (regulation_type == 1){     //Circle actuation
        for(int j=0; j<ptsNumb; j++){
            x_vector[j] = figure_x[j] + laser_x;
            y_vector[j] = figure_y[j] + laser_y;
        }
    }

    else {                              //Arc circle actuation

        if (objectifReached == true){       //Objecive reached: use of the circle
            float var;
            if (counter < 25){
                var = 2.;
            }
            else {
                var = 1.5;
            }
            for(int j=0; j<ptsNumb; j++){
                x_vector[j] = (float)var*(figure_x[j]) + target_x;
                y_vector[j] = (float)var*(figure_y[j]) + target_y;
            }

        }


        else {                              //Objecive not reached: use of the arc circle actuation

            //Calculation of the index of the middleAngle within x_vector and y_vector
            int middleAngleIndex = middleAngle / ((float)M_PI*2 / ptsNumb) +0.5;

            //Construction of the arc circle
            int index = 0;
            int i = 0;
            int circleIndex;

            for(i; i <= (vectorLength/4); i++){
                circleIndex = middleAngleIndex+index;
                if (circleIndex>=ptsNumb){
                    circleIndex = circleIndex - ptsNumb;        //If circleindex>100 subtract number points (Ex. circleindex=101, circleindex=101-100 = 1
                }
                if (circleIndex < 0){
                    circleIndex = circleIndex + ptsNumb;        //If circleindex < 100, add number points (Ex. Circle index =-2, circleindex=-2+100 = 98
                }
                x_vector[i] = figure_x[circleIndex] + laser_x;
                y_vector[i] = figure_y[circleIndex] + laser_y;
                index++;
            }

            index = index - 2;

            for(i; i <= (vectorLength*0.75); i++){
                circleIndex = middleAngleIndex+index;
                if (circleIndex>=ptsNumb){
                    circleIndex = circleIndex - ptsNumb;
                }
                else if (circleIndex < 0){
                    circleIndex = circleIndex + ptsNumb;
                }
                x_vector[i] = figure_x[circleIndex] + laser_x;
                y_vector[i] = figure_y[circleIndex] + laser_y;
                index--;
            }

            index = index + 2;

            for(i; i < vectorLength; i++){
                circleIndex = middleAngleIndex+index;
                if (circleIndex>=ptsNumb){
                    circleIndex = circleIndex - ptsNumb;
                }
                else if (circleIndex < 0){
                    circleIndex = circleIndex + ptsNumb;
                }
                x_vector[i] = figure_x[circleIndex] + laser_x;
                y_vector[i] = figure_y[circleIndex] + laser_y;
                index++;
            }

        }

    }
}



void Regulation::run_Traj_Generator(float x_particle, float y_particle)
{
    //FRAMECOUNTER++;         //Has a value of 1 here already
//    if (Flag_LinearTracking)     //To define the trajectory, I must define where is the particle and where it should go.
//    {
        if (FRAMECOUNTER==1)
        {
            x_traj.append(x_particle);   y_traj.append(y_particle);
            float Des_Velocity = 4.0/Factor_pixTOmm;           //Stablish a given desired velocity in mm/s and convert it to mm/s
            float Des_direction = atan2(y_targ-y_particle,x_targ-x_particle );
            float size_traj= abs((x_targ-x_particle)/(Des_Velocity*T_s*cos(Des_direction)))+1;          //Number of parts on which to split the vector the +1 is to compensate the added initial value
            int ii=1;

            qDebug() << "Desired velocity and direction"<<Des_Velocity  << Des_direction;
            qDebug() << "Particle position "<<x_traj.at(0)  << y_traj.at(0);
            qDebug() << "Target position "<<x_targ  << y_targ;
            qDebug() << "Number of points "<<size_traj  << floor(size_traj);

            for (ii = 1 ; ii < floor(size_traj); ii++)
            {
                x_traj.append(x_traj[ii-1] +Des_Velocity*T_s*cos(Des_direction) );     //I create the trajectory based on finite differences X + deltaX
                y_traj.append(y_traj[ii-1] +Des_Velocity*T_s*sin(Des_direction) );

            }
             x_traj.append(x_targ);     y_traj.append(y_targ);                      //As a last step, append the target value to the array

            for (ii = 0; ii < x_traj.size(); ii++) {

                qDebug() << x_traj.at(ii)  << y_traj.at(ii);

            }

        }

        //I will enter this cycle even with the value of 1, which is that even in the first iteration I should be moving somewhere
        if (FRAMECOUNTER<=x_traj.size())
        {
                    x_targ = x_traj[FRAMECOUNTER];      y_targ = y_traj[FRAMECOUNTER];      //Assign the target position from the list as long as there are elements
                    qDebug() << "Current target" << x_targ  << y_targ;
        } else if (FRAMECOUNTER> x_traj.size())
        {                                                                                   //IN THEORY I SHOULD NEVER REACH THIS STATE
            //x_targ = x_traj.last();         y_targ = y_traj.last();                         //When there are no more elements, just keep the last element
            x_targ = x_particle;            y_targ = y_particle;                           //Alternatively, we can stop the tracking by saying that the particle is at the target location which would force the controller to stop
            qDebug() << "Current target" << x_targ  << y_targ;
        }

    //}

}




//Particle position is received in Pixels
void Regulation::run_Cont_FB (float x_particle, float y_particle)
{

    //FILTERING STAGE
    if (FRAMECOUNTER==1)
    {   //First iteration, fill the previous particle positions with the actual one
        x_part_minus_2 =  x_particle; x_part_minus_1 =  x_particle;
        y_part_minus_2 =  y_particle; y_part_minus_1 =  x_particle;
    }else if (FRAMECOUNTER==2)
    {
        x_particle = x_particle/2.0 + x_part_minus_1/2.0;           //Second iteration, only the previous value is available so I just filter using the two
        y_particle = y_particle/2.0 + y_part_minus_1/2.0;
    } else
    {
        x_particle = x_particle/3.0 + x_part_minus_1/3.0 + x_part_minus_2/3.0;  //Third iteration, finally, I have the proper previous values to start using the entitre filter
        y_particle = y_particle/3.0 + y_part_minus_1/3.0 + y_part_minus_2/3.0;
    }




    //target positions
 //   if (target_type == 1){                                                       //tracking reference. target_x and y are computed at each step
 //   }                                                             //TO PROGRAM IF YOU WANT TO FOLLOW A GIVEN TRAJECTORY
    //error calculation
    float x_err = Factor_pixTOmm*x_targ - Factor_pixTOmm*x_particle;                             //results in mm
    float y_err = Factor_pixTOmm*y_targ - Factor_pixTOmm*y_particle;

    //conversion cartesian to polar coordinates
    e_0 = sqrt((x_err*x_err) + (y_err*y_err));            //radial error which I directly assign to the variable e_0


    if (e_0>0.25)
    {
        //Full equation but I compute first the part not affected by the Antiwindup gain and then I correct the input
        //u_0 = u_minus_2 + k_p*( (1+T_s/2.0/T_i+2*T_d/T_s)*e_0 + (T_s/T_i - 4*T_d/T_s)*e_minus_1 + (T_s/2.0/T_i-1+2*T_d/T_s)*e_minus_2 ) + T_s/2.0/T_t*(e_sat_0+2*e_sat_minus_1+e_sat_minus_2);
        u_0 = u_minus_2 + k_p*( (1+T_s/2.0/T_i+2*T_d/T_s)*e_0 + (T_s/T_i - 4*T_d/T_s)*e_minus_1 + (T_s/2.0/T_i-1+2*T_d/T_s)*e_minus_2 ); //Non saturated input




        // Saturation: Remember that U is r_dot_part in mm/s. U_sat_global by default should be 5.4 mm/s
        if (u_0 >= U_sat_global){
            u_sat = U_sat_global;           //Saturated in the positive sense
        }
        else if (u_0 <= -U_sat_global){
            u_sat = -U_sat_global;          //Saturated in the negative sense
        }
        else{
            u_sat = u_0;                    // Non saturation
        }
        //After I consider the non Linearity, then I can finally use the integral anti-windup
        //My last question, should I only take the proportional part or everything to compute the current error??? According to the scheme I think it should be everything.

        e_sat_0 = u_sat - u_0;                      //where E_sat = U_sat - u(k)   U_sat is the maximum input we can command to the system
                                                    //NOTE: If u_0 was not saturated, the difference will be 0 so this operation will be useless.

        u_0 = u_0 + T_s/2.0/T_t*(e_sat_0+2*e_sat_minus_1+e_sat_minus_2) ; //Saturated input FINAL



        // Saturation: Remember that U is r_dot_part in mm/s. U_sat_global by default should be 5.4 mm/s
        if (u_0 >= U_sat_global){
            u_0 = U_sat_global;           //Saturated in the positive sense
        }
        else if (u_0 <= -U_sat_global){
            u_0 = -U_sat_global;          //Saturated in the negative sense
        }
        else{
            u_0 = u_0;                    // Non saturation
        }




        //After I compute this, I save these values for the next iteration
        e_minus_2 = e_minus_1;               e_minus_1 = e_0;
        u_minus_2 = u_minus_1;             u_minus_1 = u_0;
        e_sat_minus_2 = e_sat_minus_1;      e_sat_minus_1 = e_sat_0;

        //double u_0;     double u_minus_2;   // u(k)   u(k-2)
        //double e_0;     double e_minus_1;   double e_minus_2;       // e(k)   e(k-1)    e(k-2)
        //double e_sat_0;     double e_sat_minus_1;   double e_sat_minus_2;  where E_sat = U_sat - u(k)   U_sat is the maximum input we can command to the system

        //PART TO COMPUTE THE ANGULAR ERROR
        //FRAMECOUNTER++;         //Has a value of 1 here already

        if (FRAMECOUNTER>15)            //Check if one second has passed: Yes run the controller, else set the error to 0
        {
            /*e_ang = atan2(y_targ-y_particle,x_targ-x_particle )- atan2(y_particle-y_part_BEF,x_particle-x_part_BEF );  // equivalent to theta_part-targ - theta_part*/

            //angle = atan2(x1*y2 - y1*x2, x1*x2 + y1*y2)           //Formula to get the angle between two vector limited to -pi to pi
            e_ang = atan2((x_targ-x_particle)*(y_particle-y_part_minus_1) - (y_targ-y_particle)*(x_particle-x_part_minus_1), (x_targ-x_particle)*(x_particle-x_part_minus_1) + (y_targ-y_particle)*(y_particle-y_part_minus_1));
            //e_ang = atan2((x_targ-x_part)*(y_particle-y_part_BEF) - (y_targ-y_part)*(x_particle-x_part_BEF), (x_targ-x_part_INI)*(x_particle-x_part_BEF) + (y_targ-y_part_INI)*(y_particle-y_part_BEF));
            //VERSION to set a desired target direction which was the initial particle-target direction
        }
        else if (FRAMECOUNTER==1)
        {
            //x_part_INI = x_particle; y_part_INI = y_particle;       //The first time I store the initial particle position to use as a reference all the time.
            e_ang =0;
        }
        else
        {e_ang =0;
        }



        //This part corrects the fact that the error has to operate in sort of a mirror effect
        if (e_ang>(float)M_PI/2.0)
        {
            e_ang = (float)M_PI - e_ang;
        } else if (e_ang<-(float)M_PI/2.0)
        {
             e_ang = - (float)M_PI - e_ang;
        }
        //qDebug()<<e_ang<<"Error angle after mirroring effect";


        //e_ang =0;

        theta_corr_FB = e_ang*0.25;      //As it is now, the Velocity Direction controller has a fixed proportional gain of 0.5
    }
    else
    {
        // If I am inside the tolerance region, just reset all the variables all the time so that the integrator does not acumulate
        u_0=0.0;    u_minus_1=0.0;   u_minus_2=0.0;
        e_0 = 0.0;  e_minus_1=0.0;  e_minus_2=0.0;
        e_sat_0=0.0;    e_sat_minus_1=0.0;  e_sat_minus_2=0.0;
        e_ang=0.0;      theta_corr_FB = 0.0;    theta_corr_FF = 0.0;    theta_corr=0.0;
    }


    //x_part_BEF = x_particle;                y_part_BEF = y_particle;            //Store the particle position previous value to compute the velocity direction Theta_part
                                                                                //EVEN if the particle is inside the tolerance region, it is better to store the previous particle position
    x_part_minus_2 = x_part_minus_1;  x_part_minus_1= x_particle;               //Variables to
    y_part_minus_2 = y_part_minus_1;  y_part_minus_1= y_particle;

    FRAMECOUNTER++;
}


void Regulation::run_Cont_FF (float x_particle, float y_particle)
{
    //************************** FEED FORWARD **************************//


    //distance between obstacle and particle (pixels to mm)
    float x_ObstPart = Factor_pixTOmm*x_particle - Factor_pixTOmm*x_obst;      //results in mm
    float y_ObstPart = Factor_pixTOmm*y_particle - Factor_pixTOmm*y_obst ;

    float x_PartTarg = Factor_pixTOmm*x_targ - Factor_pixTOmm*x_particle;       //Result in mm.
    float y_PartTarg = Factor_pixTOmm*y_targ - Factor_pixTOmm*y_particle;       //In this case this is only useful to get Theta_corr

    //conversion cartesian to polar coordinates
    float r_ObstPart = sqrt((x_ObstPart*x_ObstPart) + (y_ObstPart*y_ObstPart));      //radial distance in mm



    //opposite Kralchevsky axial force (from experimental model (fit exponential)) - input : [m], output : [N]
    //The coefficients are found from the file D:\PROGRAMAS\ADRIEN THESIS\Data_ForceMeasurements\fitexp.m
    double r_force_Kral =  5.8329 * pow(10,-7) * exp(-168.8263*r_ObstPart*0.001) + 1.0887 * pow(10,-5) * exp(-808.8847*r_ObstPart*0.001);



    //Stokes law to convert force_K to velocity r_dot_part_QSS = F_LatCap/(6*pi*mu*R_part) mu =  1.0016 * pow(10,-3)
    double u_FF = r_force_Kral / (6 * (double)M_PI * 1.0016 * pow(10,-3) * 0.0005);  // [m/s]
    u_FF = u_FF * 1000; // [mm/s]
    u_FF = k_ff * u_FF;   // proportional coefficient

    u_0_FF = u_FF;

    //THIS SECTION WAS wrong, in the proper version we do not saturate the FF until the final sum
//    // Final saturation before assigning the value to the class variable u_0_FF
//    if (u_FF >= 5.4){
//        u_0_FF = 5.4;
//    }
//    else if (u_FF <= -5.4){
//        u_0_FF = -5.4;
//    }
//    else{
//        u_0_FF = u_FF;
//    }

    //The theta_corr_FF is computed as Theta_part-targ - Theta_Obst-part eq 5.11 Ronald Thesis




    //angle = atan2(x1*y2 - y1*x2, x1*x2 + y1*y2)           //Formula to get the angle between two vector limited to -pi to pi
    double e_ang_FF = atan2(x_PartTarg*y_ObstPart - y_PartTarg*x_ObstPart, x_PartTarg*x_ObstPart + y_PartTarg*y_ObstPart);

    //This part corrects the fact that the error has to operate in sort of a mirror effect Which also applies to the obstacle
    if (e_ang_FF>(float)M_PI/2.0)
    {
        e_ang_FF = (float)M_PI - e_ang_FF;
    } else if (e_ang_FF<-(float)M_PI/2.0)
    {
         e_ang_FF = - (float)M_PI - e_ang_FF;
    }

    //In the future you can also add a gain in the same way as for the Feedback controller
    theta_corr_FF=e_ang_FF;


}


void Regulation::get_Laser_Position(float x_particle, float y_particle )
{

    if (e_0>0.25)               //Error is in [mm]
    {
        //float u_Final, theta_corr_Final;

        //To program the long formula to combine both answers

        if (prog_mode==1)
        {
            u_Final = u_0;                  //NOTE this should be u_feedforward + u_feedback TO CODE LATER
            theta_corr_Final = theta_corr_FB;   //NOTE this should be theta_corr_FB + theta_corr_FF TO CODE LATER
        }else if (prog_mode==2)
        {
             qDebug()<<"FB "<< u_0 <<"  FF  "<<u_0_FF;
            //Use equations 5.12 a and b to compose the final U and Theta_corr to send to the controller
            u_Final = sqrt(u_0*u_0 + u_0_FF*u_0_FF + 2*u_0*u_0_FF*cos(theta_corr_FF-theta_corr_FB));
            theta_corr_Final = theta_corr_FB + atan2(u_0_FF*sin(theta_corr_FF-theta_corr_FB), u_0+u_0_FF*cos(theta_corr_FF-theta_corr_FB));

        }

        if (u_Final >= U_sat_global){
            u_Final = U_sat_global;           //Saturated in the positive sense
        }
        else if (u_Final <= -U_sat_global){
            u_Final = -U_sat_global;          //Saturated in the negative sense
        }
        else{
            u_Final = u_Final;                    // Non saturation
        }


        //u_steady-state [mm/s] to r_las-part [mm]
        double r_las_part, theta_las_part;
        if (u_Final >= 0){
            r_las_part = (-1/1.433)*log(u_0/26.086);
        }
        else{
            r_las_part = (-1/1.433)*log(-u_0/26.086);
        }



        theta_las_part = atan2(y_targ-y_particle,x_targ-x_particle ) - theta_corr_Final;        //Eq 5.2 thesis Theta_las-part= Theta_part-targ - theta_corr

        x_las = x_particle - r_las_part * cos(theta_las_part) /Factor_pixTOmm  ;               //Compute the laser position using the formula and converting to Pixels
        y_las = y_particle - r_las_part * sin(theta_las_part) /Factor_pixTOmm ;                 //NOTE x_particle is in [pixel] and r_las_part [mm]. X_las must be in PIXELS

    }
    else
    {
        if (pattern_type==0)
        {
        x_las = 0.0;     y_las = 0.0;       //Put the laser as far away as possible from the target if the particle is inside the tolerance region OR draw the circular pattern
        } else if (pattern_type==1)
        {
            //DRAW the circular pattern
        }
    }


}

void Regulation::compute_Laser_Pattern(float X_laser_given, float Y_laser_given)
{
    //If we are in Manual mode, assign the given laser position to the regulator object
    if (prog_mode==3)
    {
        x_las = X_laser_given; y_las = Y_laser_given;
    }

    //If Pattern type is arc+Circle then define the vector of components required to draw it
    if (pattern_type == 1){
        //TO PROGRAM Define the laser Pattern VECTOR like Dimitri

    }else if(pattern_type == 0)
    {
        //If point-wise do not do anything, just command the given laser position
    }


}