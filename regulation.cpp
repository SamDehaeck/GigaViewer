#include "regulation.h"
#include <QDebug>




float Regulation::velocity2distance(float v)
{
    float d = (-1/1.433)*log(v/26.086);
    if (pattern_type==1)
        d-=pattern_radius*Factor_pixTOmm;
    return d;
}

void Regulation::PathIsReady()
{
//    qDebug()<<"NewPathReady";
    pathready=true;
    pathisvalid=Path_watcher.result();
}

Regulation::Regulation():
    myPathFinding()
{                                        //Creation of the regulation object
    qDebug()<<"Creation of Regulation object";

    ptsNumb = 100;                                                          //Cutting the circle into ptsNumb portions

    vectorLength = 100;                                              //100 for 180°, 68 for 122.4° and 32 for 57.6°

    path_N=200;
    path_PrevIndex=-1;
    path_cA=0;
    path_cL=0;

    U_sat_global=5.4;

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

    shield_dLim=160;
    shield_fHys=1.3;

    firstpath=true;
    pathready=false;

    QObject::connect(&Path_watcher, SIGNAL(finished()), this, SLOT(PathIsReady()));
}

void Regulation::Configure_Figure(float r_Figure, float a_Figure)
{
    pattern_radius=r_Figure;
    pattern_aux=a_Figure;
}

void Regulation::Configure_FB (float kp, float Ti, float Td, float Tt, float Usat, float Tsamp){

     //prog_mode = program_ID/100;        // 0 for identification    1 for feedback control   2 Feedback+Feedforward control
     //pattern_type = program_ID%100;     // 0 for point-wise pattern     1 for arc+circle pattern

     target_type=0;                 //NOTE THIS IS JUST TO CHECK WHERE this variable gets its weird value


    increment = 0;
    counter = 0;
    k_p =kp;    T_i =Ti;    T_d = Td;  T_s = Tsamp;  //Feedback Controller gains and sampling time
    T_t =Tt;        //Anti-windup gain and Input saturation
    FB_C1=k_p*(1+T_s/T_i+T_d/T_s);
    FB_C2=k_p*(-1+2*T_d/T_s);
    FB_C3=k_p*(T_d/T_s);



//    //Arc + Circle control strategy DIMITRI
//    if( (regulation_type == 1) || (regulation_type == 2) ){
//        qDebug() << "Creation of the figure";
//        float stepAngle = (float)M_PI*2 / ptsNumb;
//        for( int i = 0; i < ptsNumb; i++ ){
//            figure_x[i] = radius*cos( i * stepAngle );
//            figure_y[i] = radius*sin( i * stepAngle );
//        }
//    }
//    else{
//        qDebug()<<"Use of point actuation. No figure creation";
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

//void Regulation::Regulator2017 (float particle_x, float particle_y){

//    //target positions
//    if (target_type == 1){                                                       //tracking reference. target_x and y are computed at each step
//        if (flag == 1){                                                          //start the tracking at the current particle position
//            start_x = particle_x;
//            start_y = particle_y;
//            flag = 0;
//        }
//        increment++;

//        target_x = start_x + 200*cos(2*3.1416*0.02*increment*T_s);               //circle at f=0.02 Hz
//        target_y = start_y + 200*sin(2*3.1416*0.02*increment*T_s);

////        target_x = start_x + 4*15.7538*T_s*increment;                                      //horizontal line 4mm/s)
////        target_y = start_y ;
//    }


//    //****************************** PID *******************************//


//    //update of the (k-1) states
////    e1 = e0;                                                                        //for method without anti-windup
////    u1 = u0;

//    //error calculation (pixels to mm)
//    float x_error = 0.064453*target_x - 0.064453*particle_x;              //results in mm
//    float y_error = 0.064453*target_y - 0.064453*particle_y;

//    //conversion cartesian to polar coordinates
//    e0 = sqrt((x_error*x_error) + (y_error*y_error));            //radial error
//    float alphaPID = getAlpha(x_error,y_error);

//    //controller PID
////    u0 = u1 + (k_p + k_i*(T_s/2)) * e0 + (-k_p + k_i*(T_s/2)) * e1;  //in [mm/s]     //for method without anti-windup

//    //proportional action
//    float u_p = k_p/(2*T_i) * (T_s + 2*T_i) * e0;         //u_p(k) = coeff * e(k)
//    u0 = u_i + u_p;                                       //u(k) = up(k) + ui(k)

//    //saturation
//    double u_sat_PID;
//    if (u0 >= 5.4){
//        u_sat_PID = 5.4;
//    }
//    else if (u0 <= -5.4){
//        u_sat_PID = -5.4;
//    }
//    else{
//        u_sat_PID = u0;
//    }
//    std::cout << "PID : " << u0 << std::endl;
//    std::cout << "PID sat : " << u_sat_PID << std::endl;

//    //integral action with anti-windup
//    u_i = u_i + (k_p*T_s/T_i) * e0 - T_t*(u0 - u_sat);   //u_i(k+1) = ui(k) + smth * e(k) - kt*(u(k) - u_sat(k))


//    // conversion polar to cartesian coordinates
//    // the vector of that speed has to be add to the speed computed by the FF
//    double u_PID_x = u_sat_PID * cos(alphaPID);
//    double u_PID_y = u_sat_PID * sin(alphaPID);


//    //************************** FEED FORWARD **************************//


//    //distance between obstacle and particle (pixels to mm)
//    float distFF_x = 0.064453*x_obst - 0.064453*particle_x;      //results in mm
//    float distFF_y = 0.064453*y_obst - 0.064453*particle_y;

//    //conversion cartesian to polar coordinates
//    float distFF = sqrt((distFF_x*distFF_x) + (distFF_y*distFF_y));      //radial distance in mm
//    float alphaFF = getAlpha(distFF_x,distFF_y);

//    //opposite Kralchevsky axial force (from experimental model (fit exponential)) - input : [m], output : [N]
//    double force_K =  5.8329 * pow(10,-7) * exp(-168.8263*distFF*0.001)
//                      + 1.0887 * pow(10,-5) * exp(-808.8847*distFF*0.001);

//    //opposite Kralchevsky axial force (from experimental model (fit Bessel)) - input : [m], output : [N]
////    double a = 9.8888 * pow(10,-6);
////    double b = 800.5627;
////    double bessel = boost::math::cyl_bessel_k(1,b*distFF*0.001);
////    double force_K = a*bessel;

//    //opposite Kralchevsky axial force (theoretical model) - input : [m], output : [N]
////    double a = 1.4384 * pow(10,-6);
////    double b = 370.3704;
////    double bessel = boost::math::cyl_bessel_k(1,b*distFF*0.001);
////    double force_K = a*bessel;

//    //Stokes to convert force_K to velocity
//    double uFF = force_K / (6 * (double)M_PI * 1.0016 * pow(10,-3) * 0.0005);  // [m/s]
//    uFF = uFF * 1000; // [mm/s]
//    uFF = k_ff * uFF;   // proportional coefficient

//    //saturation
//    double u_sat_FF;
//    if (uFF >= 5.4){
//        u_sat_FF = 5.4;
//    }
//    else if (uFF <= -5.4){
//        u_sat_FF = -5.4;
//    }
//    else{
//        u_sat_FF = uFF;
//    }


//    std::cout << "FF : " << u_sat_FF << std::endl;

//    // conversion polar to cartesian coordinates
//    // the vector of that speed has to be add to the speed computed by the PID
//    double u_FF_x = u_sat_FF * cos(alphaFF);
//    double u_FF_y = u_sat_FF * sin(alphaFF);

//    //********************** COMBINATION PID-FF ************************//

//    //vectors addition
//    double u_x = u_PID_x + u_FF_x;
//    double u_y = u_PID_y + u_FF_y;

//    //conversion cartesian to polar coordinates
//    u = sqrt((u_x*u_x) + (u_y*u_y));      //radial distance in mm
//    double alpha = getAlpha(u_x,u_y);     //orientation

//    //saturation
//    if (u >= 5.4){
//        u_sat = 5.4;
//    }
//    else if (u <= -5.4){
//        u_sat = -5.4;
//    }
//    else{
//        u_sat = u;
//    }
//    std::cout << "total : " << u_sat << std::endl;

//    //u_steady-state to r_las-part
//    double r_las_part;
//    if (u_sat >= 0){
//        r_las_part = (-1/1.433)*log(u_sat/26.086);
//    }
//    else{
//        r_las_part = (-1/1.433)*log(-u_sat/26.086);
//    }
//    alpha = alpha + (float)M_PI;

//    //convert back in cartesian coordinates and in pixels for the mirror
//    //and use the particle as reference
//    laser_x = r_las_part * cos(alpha) * 15.515 + particle_x;
//    laser_y = r_las_part * sin(alpha) * 15.515 + particle_y;

//}

//float Regulation::getAlpha (float thing_part_x, float thing_part_y) {
//    //take the distance between something and the particle in x and y, and return the angle alpha
//    //with respect to the x-axis
//    float alpha;
//    if (thing_part_x > 0){
//        alpha = atan((thing_part_y/thing_part_x));
//    }
//    else if (thing_part_x < 0){
//        alpha = (float)M_PI + atan((thing_part_y/thing_part_x));
//    }
//    else{
//        if (thing_part_y > 0){
//            alpha = (float)M_PI/2;
//        }
//        else if (thing_part_y < 0){
//            alpha = - (float)M_PI/2;
//        }
//        else{
//            alpha = 0;
//        }
//    }
//    return alpha;
//}

//// Dimitri's version
//void Regulation::Regulator2016 (float particle_x, float particle_y){

//    //target positions
//    if (target_type == 1){                                                       //tracking reference. target_x and y are computed at each step
//        if (flag == 1){                                                         //start of the tracking at the current particle position
//            start_x = particle_x;
//            start_y = particle_y;
//            flag = 0;
//        }
//        increment++;
//        target_x = start_x + 200*sin(0.05*increment);                                                  //parameterisation
//        target_y = start_y + 200*sin(0.05*increment)*cos(0.05*increment);
//    }

//    //error calculation
//    float x_error = 0.064453*target_x - 0.064453*particle_x;                             //results in mm
//    float y_error = 0.064453*target_y - 0.064453*particle_y;
//    float error = sqrt((x_error*x_error) + (y_error*y_error));                          //radial error

//    //angle orientation and r_particle_target calculation
//    float alpha;
//    if (x_error > 0){
//        alpha = atan((y_error/x_error));
//    }
//    else if (x_error < 0){
//        alpha = (float)M_PI + atan((y_error/x_error));
//    }
//    else{
//        if (y_error > 0){
//            alpha = (float)M_PI/2;
//        }
//        else if (y_error < 0){
//            alpha = - (float)M_PI/2;
//        }
//        else{
//            alpha = 0;
//        }
//    }
//    middleAngle = (float)M_PI + alpha;


//    //controller
//    if (target_type == 1){
//        //u_i = u_i + (k_p*T_s/T_i) * error - T_t*(u - u_sat);
//    }
//    float u_p = k_p/(2*T_i) * (T_s + 2*T_i) * error;
//    u = u_i + u_p;
//    if (u >= 7){
//        u_sat = 7;
//    }
//    else if (u <= -7){
//        u_sat = -7;
//    }
//    else{
//        u_sat = u;
//    }

//    //u_steady-state to r_las-part
//    double r_las_part;
//    if (u_sat >= 0){
//        r_las_part = (1/1.678)*log(24.93/u_sat)*1;
//    }
//    else{
//        r_las_part = (1/1.678)*log(-u_sat/24.93)*1;
//    }

//    //objective nearly reached. Use of a circle to stop the particle within the vicinity of the objective
//    if (abs(x_error) < (radius*0.064453) && abs(y_error) < (radius*0.064453) && target_type ==0 ){
//        objectifReached = true;
//        counter++;
//    }

//    if (regulation_type == 0) {
//        laser_x = r_las_part * cos(alpha) * 15.515 + particle_x;
//        laser_y = r_las_part * sin(alpha) * 15.515 + particle_y;
//    }

//    else {

//        //r_las-part to r_part_center
//        float r_part_center = radius - (r_las_part * 15.515);               //in pix

//        //r_part_center to x/y_part_center
//        float x_part_center = r_part_center * cos(alpha);
//        float y_part_center = r_part_center * sin(alpha);

//        //computation of the new center figure
//        laser_x = particle_x + x_part_center;
//        laser_y = particle_y + y_part_center;

//    }


//    //Transfer new data stream to the mirror

//    if (regulation_type == 0){          //Point actuation

//    }

//    else if (regulation_type == 1){     //Circle actuation
//        for(int j=0; j<ptsNumb; j++){
//            x_vector[j] = figure_x[j] + laser_x;
//            y_vector[j] = figure_y[j] + laser_y;
//        }
//    }

//    else {                              //Arc circle actuation

//        if (objectifReached == true){       //Objecive reached: use of the circle
//            float var;
//            if (counter < 25){
//                var = 2.;
//            }
//            else {
//                var = 1.5;
//            }
//            for(int j=0; j<ptsNumb; j++){
//                x_vector[j] = (float)var*(figure_x[j]) + target_x;
//                y_vector[j] = (float)var*(figure_y[j]) + target_y;
//            }

//        }


//        else {                              //Objecive not reached: use of the arc circle actuation

//            //Calculation of the index of the middleAngle within x_vector and y_vector
//            int middleAngleIndex = middleAngle / ((float)M_PI*2 / ptsNumb) +0.5;

//            //Construction of the arc circle
//            int index = 0;
//            int i = 0;
//            int circleIndex;

//            for(i; i <= (vectorLength/4); i++){
//                circleIndex = middleAngleIndex+index;
//                if (circleIndex>=ptsNumb){
//                    circleIndex = circleIndex - ptsNumb;        //If circleindex>100 subtract number points (Ex. circleindex=101, circleindex=101-100 = 1
//                }
//                if (circleIndex < 0){
//                    circleIndex = circleIndex + ptsNumb;        //If circleindex < 100, add number points (Ex. Circle index =-2, circleindex=-2+100 = 98
//                }
//                x_vector[i] = figure_x[circleIndex] + laser_x;
//                y_vector[i] = figure_y[circleIndex] + laser_y;
//                index++;
//            }

//            index = index - 2;

//            for(i; i <= (vectorLength*0.75); i++){
//                circleIndex = middleAngleIndex+index;
//                if (circleIndex>=ptsNumb){
//                    circleIndex = circleIndex - ptsNumb;
//                }
//                else if (circleIndex < 0){
//                    circleIndex = circleIndex + ptsNumb;
//                }
//                x_vector[i] = figure_x[circleIndex] + laser_x;
//                y_vector[i] = figure_y[circleIndex] + laser_y;
//                index--;
//            }

//            index = index + 2;

//            for(i; i < vectorLength; i++){
//                circleIndex = middleAngleIndex+index;
//                if (circleIndex>=ptsNumb){
//                    circleIndex = circleIndex - ptsNumb;
//                }
//                else if (circleIndex < 0){
//                    circleIndex = circleIndex + ptsNumb;
//                }
//                x_vector[i] = figure_x[circleIndex] + laser_x;
//                y_vector[i] = figure_y[circleIndex] + laser_y;
//                index++;
//            }

//        }

//    }
//}




//void Regulation::run_Traj_Generator(float x_particle, float y_particle, float x_targ, float y_targ){
//    //FRAMECOUNTER++;         //Has a value of 1 here already
////    if (Flag_LinearTracking)     //To define the trajectory, I must define where is the particle and where it should go.
////    {
//        if (FRAMECOUNTER==1)
//        {
//            x_traj.append(x_particle);   y_traj.append(y_particle);
//            float Des_Velocity = 4.0/Factor_pixTOmm;           //Stablish a given desired velocity in mm/s and convert it to mm/s
//            float Des_direction = atan2(y_targ-y_particle,x_targ-x_particle );
//            float size_traj= abs((x_targ-x_particle)/(Des_Velocity*T_s*cos(Des_direction)))+1;          //Number of parts on which to split the vector the +1 is to compensate the added initial value
//            int ii=1;

//            qDebug() << "Desired velocity and direction"<<Des_Velocity  << Des_direction;
//            qDebug() << "Particle position "<<x_traj.at(0)  << y_traj.at(0);
//            qDebug() << "Target position "<<x_targ  << y_targ;
//            qDebug() << "Number of points "<<size_traj  << floor(size_traj);

//            for (ii = 1 ; ii < floor(size_traj); ii++)
//            {
//                x_traj.append(x_traj[ii-1] +Des_Velocity*T_s*cos(Des_direction) );     //I create the trajectory based on finite differences X + deltaX
//                y_traj.append(y_traj[ii-1] +Des_Velocity*T_s*sin(Des_direction) );

//            }
//             x_traj.append(x_targ);     y_traj.append(y_targ);                      //As a last step, append the target value to the array

//            for (ii = 0; ii < x_traj.size(); ii++) {

//                qDebug() << x_traj.at(ii)  << y_traj.at(ii);

//            }

//        }

//        //I will enter this cycle even with the value of 1, which is that even in the first iteration I should be moving somewhere
//        if (FRAMECOUNTER<=x_traj.size())
//        {
//                    x_targ = x_traj[FRAMECOUNTER];      y_targ = y_traj[FRAMECOUNTER];      //Assign the target position from the list as long as there are elements
//                    qDebug() << "Current target" << x_targ  << y_targ;
//        } else if (FRAMECOUNTER> x_traj.size())
//        {                                                                                   //IN THEORY I SHOULD NEVER REACH THIS STATE
//            //x_targ = x_traj.last();         y_targ = y_traj.last();                         //When there are no more elements, just keep the last element
//            x_targ = x_particle;            y_targ = y_particle;                           //Alternatively, we can stop the tracking by saying that the particle is at the target location which would force the controller to stop
//            qDebug() << "Current target" << x_targ  << y_targ;
//        }

//    //}

//}


bool Regulation::run_Shielding(float x_particle, float y_particle, float x_particle2, float y_particle2, float x_targ, float y_targ)
{
    static bool hysFlag=1;
    float dx=x_particle2-x_particle;
    float dy=y_particle2-y_particle;
    float d=sqrt(dx*dx+dy*dy);
    float dL=velocity2distance(U_sat_global)/Factor_pixTOmm;
    float dTx=x_targ-x_particle;
    float dTy=y_targ-y_particle;
    float dT=sqrt(dTx*dTx+dTy*dTy);

    float dlim=shield_dLim;
    if (hysFlag)
        dlim*=shield_fHys;
    if (d>dlim){
        hysFlag=0;
        return false;
    }
    hysFlag=1;

    x_las=x_particle2-dL*dx/d;
    y_las=y_particle2-dL*dy/d;
    middleAngle=atan2(-dy,-dx);
    return true;
}

bool Regulation::run_SmartShielding(float x_particle, float y_particle, float x_particle2, float y_particle2, float x_targ, float y_targ)
{
    static bool hysFlag=1;
    float dx=x_particle2-x_particle;
    float dy=y_particle2-y_particle;
    float d=sqrt(dx*dx+dy*dy);
    float dL=velocity2distance(U_sat_global)/Factor_pixTOmm;
    float dTx=x_targ-x_particle;
    float dTy=y_targ-y_particle;
    float dT=sqrt(dTx*dTx+dTy*dTy);

    float dlim=shield_dLim;
    if (hysFlag)
        dlim*=shield_fHys;
    if (d>dlim){
        hysFlag=0;
        return false;
    }
    hysFlag=1;

    float proj=(dx*dTx+dy*dTy)/dT/dT;

    if (proj<=0){
        x_las=x_particle2-dL*dx/d;
        y_las=y_particle2-dL*dy/d;
        middleAngle=atan2(-dy,-dx);
    }else if(proj>=1){
        float dT2x=x_targ-x_particle2;
        float dT2y=y_targ-y_particle2;
        float dT2=sqrt(dT2x*dT2x+dT2y*dT2y);
        x_las=x_particle2+dL*dT2x/dT2;
        y_las=y_particle2+dL*dT2y/dT2;
        middleAngle=atan2(dT2y,dT2x);
    }else{
        float x_aux=x_particle+proj*dTx;
        float y_aux=y_particle+proj*dTy;
        float dauxx=x_aux-x_particle2;
        float dauxy=y_aux-y_particle2;
        float daux=sqrt(dauxx*dauxx+dauxy*dauxy);
        x_las=x_particle2+dL*dauxx/daux;
        y_las=y_particle2+dL*dauxy/daux;
        middleAngle=atan2(dauxy,dauxx);
    }
    return true;
}




//Particle position is received in Pixels
void Regulation::run_Cont_FB (float x_particle, float y_particle, float x_targ, float y_targ){

    //FILTERING STAGE
    if (FRAMECOUNTER==1)
    {   //First iteration, fill the previous particle positions with the actual one
        x_part_minus_2 =  x_particle; x_part_minus_1 =  x_particle;
        y_part_minus_2 =  y_particle; y_part_minus_1 =  y_particle;
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
        objectifReached=0;
        //Full equation but I compute first the part not affected by the Antiwindup gain and then I correct the input
        //u_0 = u_minus_2 + k_p*( (1+T_s/2.0/T_i+2*T_d/T_s)*e_0 + (T_s/T_i - 4*T_d/T_s)*e_minus_1 + (T_s/2.0/T_i-1+2*T_d/T_s)*e_minus_2 ) + T_s/2.0/T_t*(e_sat_0+2*e_sat_minus_1+e_sat_minus_2);

        //u_0 = u_minus_2 + k_p*( (1+T_s/2.0/T_i+2*T_d/T_s)*e_0 + (T_s/T_i - 4*T_d/T_s)*e_minus_1 + (T_s/2.0/T_i-1+2*T_d/T_s)*e_minus_2 ); //Non saturated input

        u_0=u_minus_1+FB_C1*e_0+FB_C2*e_minus_1+FB_C3*e_minus_2;


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
        theta_part_targ = atan2(y_targ-y_particle,x_targ-x_particle );
    }
    else
    {
        objectifReached=1;
        // If I am inside the tolerance region, just reset all the variables all the time so that the integrator does not acumulate
        resetIntegratorVariables();
    }


    //x_part_BEF = x_particle;                y_part_BEF = y_particle;            //Store the particle position previous value to compute the velocity direction Theta_part
                                                                                //EVEN if the particle is inside the tolerance region, it is better to store the previous particle position
    x_part_minus_2 = x_part_minus_1;  x_part_minus_1= x_particle;               //Variables to
    y_part_minus_2 = y_part_minus_1;  y_part_minus_1= y_particle;

    FRAMECOUNTER++;
}

void Regulation::run_Cont_FB_vel(float x_particle, float y_particle)
{
    //FILTERING STAGE
    if (FRAMECOUNTER==1)
    {   //First iteration, fill the previous particle positions with the actual one
        x_part_minus_2 =  x_particle; x_part_minus_1 =  x_particle;
        y_part_minus_2 =  y_particle; y_part_minus_1 =  y_particle;
    }else if (FRAMECOUNTER==2)
    {
        x_particle = x_particle/2.0 + x_part_minus_1/2.0;           //Second iteration, only the previous value is available so I just filter using the two
        y_particle = y_particle/2.0 + y_part_minus_1/2.0;
    } else
    {
        x_particle = x_particle/3.0 + x_part_minus_1/3.0 + x_part_minus_2/3.0;  //Third iteration, finally, I have the proper previous values to start using the entitre filter
        y_particle = y_particle/3.0 + y_part_minus_1/3.0 + y_part_minus_2/3.0;
    }

    float ux, uy;
    //Derivative
    ux=(x_particle-x_part_minus_1)*Factor_pixTOmm/T_s;
    uy=(x_particle-x_part_minus_1)*Factor_pixTOmm/T_s;

    //conversion cartesian to polar coordinates
    float u_curr=sqrt(ux*ux+uy*uy);


    e_0 = u_path-u_curr;


    u_0=u_minus_1+FB_C1*e_0+FB_C2*e_minus_1+FB_C3*e_minus_2;


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

    //x_part_BEF = x_particle;                y_part_BEF = y_particle;            //Store the particle position previous value to compute the velocity direction Theta_part
                                                                                //EVEN if the particle is inside the tolerance region, it is better to store the previous particle position
    x_part_minus_2 = x_part_minus_1;  x_part_minus_1= x_particle;               //Variables to
    y_part_minus_2 = y_part_minus_1;  y_part_minus_1= y_particle;

    FRAMECOUNTER++;
}


void Regulation::run_Cont_FF (float x_particle, float y_particle, float x_targ, float y_targ){
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

void Regulation::run_Ident(float x_particle, float y_particle, float dist_LasPart, float radius_pattern, int direction,float A_pattern_given)
{
    float dist_LasCent;
    if (pattern_type==0){
        dist_LasCent=dist_LasPart;
    }else{
        dist_LasCent=dist_LasPart-radius_pattern;
    }
    dist_LasCent/=Factor_pixTOmm;
    if (direction==0){
        x_las=x_particle;
        y_las=y_particle-dist_LasCent;
        middleAngle=-M_PI/2.0;

    }else if(direction==1){
        x_las=x_particle+dist_LasCent;
        y_las=y_particle;
        middleAngle=0.0;
    }else if(direction==2){
        x_las=x_particle;
        y_las=y_particle+dist_LasCent;
        middleAngle=M_PI/2.0;
    }else if(direction==3){
        x_las=x_particle-dist_LasCent;
        y_las=y_particle;
        middleAngle=M_PI;
    }
    pattern_aux=A_pattern_given;pattern_radius=radius_pattern;
}

void Regulation::run_ManualMode(float X_laser_given, float Y_laser_given, float A_laser_given, float R_pattern_given, float A_pattern_given)
{
    x_las=X_laser_given;y_las=Y_laser_given;middleAngle=A_laser_given;pattern_radius=R_pattern_given;pattern_aux=A_pattern_given;
}


void Regulation::get_Laser_Position(float x_particle, float y_particle){

    if (e_0>0.25)               //Error is in [mm]
    {
        //float u_Final, theta_corr_Final;

        //To program the long formula to combine both answers

        if (prog_mode==MODE_FB||prog_mode==MODE_SHIELDING)
        {
            u_Final = u_0;                  //NOTE this should be u_feedforward + u_feedback TO CODE LATER
            theta_corr_Final = theta_corr_FB;   //NOTE this should be theta_corr_FB + theta_corr_FF TO CODE LATER
        }else if (prog_mode==MODE_FF){
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


        //u_steady-state [mm/s] to r_las-part [mm]
        double r_las_part, theta_las_part;
        if (u_Final >= 0){
            r_las_part = velocity2distance(u_Final);
        }
        else{
            r_las_part = velocity2distance(-u_Final);
        }
        theta_las_part = theta_part_targ-theta_corr_Final;       //Eq 5.2 thesis Theta_las-part= Theta_part-targ - theta_corr

        middleAngle=theta_las_part+M_PI;

        x_las = x_particle + r_las_part * cos(middleAngle) /Factor_pixTOmm  ;               //Compute the laser position using the formula and converting to Pixels
        y_las = y_particle + r_las_part * sin(middleAngle) /Factor_pixTOmm ;                 //NOTE x_particle is in [pixel] and r_las_part [mm]. X_las must be in PIXELS

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

void Regulation::get_Laser_Position_vel(float x_particle, float y_particle)
{
        //float u_Final, theta_corr_Final;

        //To program the long formula to combine both answers


        //u_steady-state [mm/s] to r_las-part [mm]
        double r_las_part;
        if (u_0 >= 0){
            r_las_part = velocity2distance(u_0);
        }
        else{
            r_las_part = velocity2distance(-u_0);
        }


        x_las = x_particle + r_las_part * cos(middleAngle) /Factor_pixTOmm  ;               //Compute the laser position using the formula and converting to Pixels
        y_las = y_particle + r_las_part * sin(middleAngle) /Factor_pixTOmm ;                 //NOTE x_particle is in [pixel] and r_las_part [mm]. X_las must be in PIXELS

}



void Regulation::compute_Laser_Pattern(int patterntype, float X_laser_given, float Y_laser_given, float A_laser_given, float R_pattern_given, float A_pattern_given)
{
    //If we are in Manual mode, assign the given laser position to the regulator object
    double angle_laser;
    double radius_pattern;

    double var_aux1;
    double var_aux2;
    double var_aux3;
    angle_laser=A_laser_given+M_PI;
    if (patterntype != 1)
        A_pattern_given=abs(A_pattern_given);
    radius_pattern=R_pattern_given;

    //If Pattern type is arc+Circle then define the vector of components required to draw it
    if (patterntype == 1){
        double angle_pattern=2.0*M_PI*A_pattern_given;
        if (abs(A_pattern_given)<1){
            for(int j=0; j<ptsNumb/2; j++){
                var_aux1=angle_laser+angle_pattern*(((double)(2*j))/((double)(ptsNumb))-0.5);
                var_aux2=angle_laser+angle_pattern*(0.5-((double)(2*j))/((double)(ptsNumb)));

                x_vector[j] = X_laser_given-radius_pattern*cos(var_aux1);
                y_vector[j] = Y_laser_given-radius_pattern*sin(var_aux1);

                x_vector[j+ptsNumb/2] = X_laser_given-radius_pattern*cos(var_aux2);
                y_vector[j+ptsNumb/2] = Y_laser_given-radius_pattern*sin(var_aux2);
            }
        }
        else{
            for(int j=0; j<ptsNumb; j++){
                var_aux1=A_pattern_given*2.0*M_PI*((double)j)/((double)(ptsNumb));
                x_vector[j] = X_laser_given+radius_pattern*cos(var_aux1);
                y_vector[j] = Y_laser_given+radius_pattern*sin(var_aux1);
            }

        }
    }else if (patterntype == 2){
        for(int j=0; j<ptsNumb; j++){
            var_aux3=2.0*M_PI*((double)j)/((double)(ptsNumb));
            var_aux1=radius_pattern*cos(var_aux3);
            var_aux2=radius_pattern*A_pattern_given*sin(var_aux3);
            x_vector[j] = X_laser_given+cos(angle_laser)*var_aux1-sin(angle_laser)*var_aux2;
            y_vector[j] = Y_laser_given+sin(angle_laser)*var_aux1+cos(angle_laser)*var_aux2;
        }

    }else if (patterntype == 3){
        for(int j=0; j<ptsNumb/4; j++){
            var_aux3=radius_pattern*(((double)(8*j))/((double)(ptsNumb))-1);

            var_aux1=radius_pattern;
            var_aux2=var_aux3*A_pattern_given;
            x_vector[j] = X_laser_given+cos(angle_laser)*var_aux1-sin(angle_laser)*var_aux2;
            y_vector[j] = Y_laser_given+sin(angle_laser)*var_aux1+cos(angle_laser)*var_aux2;

            var_aux1=-var_aux3;
            var_aux2=radius_pattern*A_pattern_given;
            x_vector[j+ptsNumb/4] = X_laser_given+cos(angle_laser)*var_aux1-sin(angle_laser)*var_aux2;
            y_vector[j+ptsNumb/4] = Y_laser_given+sin(angle_laser)*var_aux1+cos(angle_laser)*var_aux2;

            var_aux1=-radius_pattern;
            var_aux2=-var_aux3*A_pattern_given;
            x_vector[j+ptsNumb/2] =X_laser_given+ cos(angle_laser)*var_aux1-sin(angle_laser)*var_aux2;
            y_vector[j+ptsNumb/2] = Y_laser_given+sin(angle_laser)*var_aux1+cos(angle_laser)*var_aux2;

            var_aux1=var_aux3;
            var_aux2=-radius_pattern*A_pattern_given;
            x_vector[j+(3*ptsNumb)/4] = X_laser_given+cos(angle_laser)*var_aux1-sin(angle_laser)*var_aux2;
            y_vector[j+(3*ptsNumb)/4] = Y_laser_given+sin(angle_laser)*var_aux1+cos(angle_laser)*var_aux2;
        }
    }else if (patterntype == 4){
        for(int j=0; j<ptsNumb/2; j++){
            var_aux3=radius_pattern*(((double)(2*j))/((double)(ptsNumb)));

            var_aux1=var_aux3;
            x_vector[j] = X_laser_given+cos(angle_laser)*var_aux1;
            y_vector[j] = Y_laser_given+sin(angle_laser)*var_aux1;

            var_aux1=radius_pattern-var_aux3;
            x_vector[j+ptsNumb/2] = X_laser_given+cos(angle_laser)*var_aux1;
            y_vector[j+ptsNumb/2] = Y_laser_given+sin(angle_laser)*var_aux1;
        }
    }
}

void Regulation::compute_Laser_Pattern_INT()
{
    compute_Laser_Pattern(pattern_type,x_las,y_las,middleAngle,pattern_radius,pattern_aux);
}

void Regulation::resetIntegratorVariables()
{
//    qDebug()<<"Reset regulator";
    u_0=0.0;    u_minus_1=0.0;   u_minus_2=0.0;
    e_0 = 0.0;  e_minus_1=0.0;  e_minus_2=0.0;
    e_sat_0=0.0;    e_sat_minus_1=0.0;  e_sat_minus_2=0.0;
    e_ang=0.0;      theta_corr_FB = 0.0;    theta_corr_FF = 0.0;    theta_corr=0.0;
}

void Regulation::multiplexLAserPosition(int index)
{
    x_las_mult[index]=x_las;
    y_las_mult[index]=y_las;
    middleAngle_mult[index]=middleAngle;
    pattern_radius_mult[index]=pattern_radius;
    pattern_aux_mult[index]=pattern_aux;
}

void Regulation::demultiplexLAserPosition(int index)
{
    x_las=x_las_mult[index];
    y_las=y_las_mult[index];
    middleAngle=middleAngle_mult[index];
    pattern_radius=pattern_radius_mult[index];
    pattern_aux=pattern_aux_mult[index];
}

void Regulation::computeClosedPath()
{
    qDebug()<<"Closed Path Creation";
    path_totD=0;
    for (int i=0; i<path_N; i++){
        path_x[i]=20/Factor_pixTOmm*cos(2*M_PI*i/path_N)+512;
        path_y[i]=15/Factor_pixTOmm*sin(4*M_PI*i/path_N)+512;
    }
    for (int i=0; i<path_N; i++){
        int ip=i+1;
        if (ip>=path_N)
            ip-=path_N;
        path_A[i]=atan2(path_y[ip]-path_y[i],path_x[ip]-path_x[i]);
        float dx=(path_x[ip]-path_x[i]);
        float dy=(path_y[ip]-path_y[i]);
        path_D[i]=sqrt(dx*dx+dy*dy);
        path_totD+=path_D[i];
    }
    for (int i=0; i<path_N; i++){
        int im=i-1;
        if (im<0)
            im+=path_N;
        float angdiff=abs(path_A[i]-path_A[im]);
        if (angdiff>M_PI/2)
            angdiff=abs(angdiff-M_PI);
        path_K[i]=2*angdiff/(path_D[i]+path_D[im]);
//        qDebug()<<path_K[i];
    }
    path_meanD=path_totD/path_N;
    path_PrevIndex=-1;
}

void Regulation::computeOpenPath()
{
    path_totD=0;
//    qDebug()<<"Open Path computation";
//    for (int i=0; i<path_N; i++){
//        path_x[i]=20/Factor_pixTOmm*i/(path_N-1)+512-10/Factor_pixTOmm;
//        path_y[i]=10/Factor_pixTOmm*sin(5*M_PI*i/(path_N-1))+512;
//    }
    for (int i=0; i<path_N-1; i++){
        int ip=i+1;
        path_A[i]=atan2(path_y[ip]-path_y[i],path_x[ip]-path_x[i]);
        float dx=(path_x[ip]-path_x[i]);
        float dy=(path_y[ip]-path_y[i]);
        path_D[i]=sqrt(dx*dx+dy*dy);
        path_totD+=path_D[i];
        path_Dac[i]=path_totD;
    }
    for (int i=0; i<path_N-1; i++){
        path_Dac[i]=path_totD-path_Dac[i];
    }
    path_K[0]=0;
    for (int i=1; i<path_N-1; i++){
        int im=i-1;
        float angdiff=abs(path_A[i]-path_A[im]);
        if (angdiff>M_PI/2)
            angdiff=abs(angdiff-M_PI);
        path_K[i]=2*angdiff/(path_D[i]+path_D[im]);
    }
    path_K[path_N-1]=0;
    path_meanD=path_totD/(path_N-1);
}

void Regulation::run_ClosedPath_Following(float x_particle, float y_particle)
{
    int indexmin;
    int space=50;
    float dxmin, dymin, d2min;
    if (path_PrevIndex<0){
        indexmin=0;
        dxmin=x_particle-path_x[indexmin];
        dymin=y_particle-path_y[indexmin];
        d2min=dxmin*dxmin+dymin*dymin;
        for (int i=1;i<path_N;i++){
            float dx=x_particle-path_x[i];
            float dy=y_particle-path_y[i];
            float d2=dx*dx+dy*dy;
            if (d2<d2min){
                dxmin=dx;
                dymin=dy;
                d2min=d2;
                indexmin=i;
            }
        }
    }else{
        indexmin=path_PrevIndex;
        dxmin=x_particle-path_x[indexmin];
        dymin=y_particle-path_y[indexmin];
        d2min=dxmin*dxmin+dymin*dymin;
        for (int i=1; i<=space; i++){
            int ip=path_PrevIndex+i;
            if (ip>=path_N)
                ip-=path_N;
            int im=path_PrevIndex-i;
            if (im<0)
                im+=path_N;
            float dx=x_particle-path_x[ip];
            float dy=y_particle-path_y[ip];
            float d2=dx*dx+dy*dy;
            if (d2<d2min){
                dxmin=dx;
                dymin=dy;
                d2min=d2;
                indexmin=ip;
            }
            dx=x_particle-path_x[im];
            dy=y_particle-path_y[im];
            d2=dx*dx+dy*dy;
            if (d2<d2min){
                dxmin=dx;
                dymin=dy;
                d2min=d2;
                indexmin=im;
            }
        }
    }

    int ip=indexmin+1;
    if (ip>=path_N)
        ip-=path_N;
    int im=indexmin-1;
    if (im<0)
        im+=path_N;
    float dxp=x_particle-path_x[ip];
    float dyp=y_particle-path_y[ip];
    float d2p=dxp*dxp+dyp*dyp;
    float dxm=x_particle-path_x[im];
    float dym=y_particle-path_y[im];
    float d2m=dxm*dxm+dym*dym;
    float angle;
    if (d2p<=d2m){
        angle=path_A[indexmin];
    }else{
        angle=path_A[im];
    }

    float d=-sin(angle)*dxmin+cos(angle)*dymin;
    middleAngle=angle-atan2(d,path_cL/Factor_pixTOmm)+M_PI;
    u_path=U_sat_global/(1+path_cA*path_K[indexmin]/Factor_pixTOmm);

    path_PrevIndex=indexmin;
}

void Regulation::run_OpenPath_Following(float x_particle, float y_particle)
{
    int indexmin;
    int space=20;
    float dxmin, dymin, d2min;
    if (path_PrevIndex<0){
        indexmin=0;
        dxmin=x_particle-path_x[indexmin];
        dymin=y_particle-path_y[indexmin];
        d2min=dxmin*dxmin+dymin*dymin;
        for (int i=1;i<path_N;i++){
            float dx=x_particle-path_x[i];
            float dy=y_particle-path_y[i];
            float d2=dx*dx+dy*dy;
            if (d2<d2min){
                dxmin=dx;
                dymin=dy;
                d2min=d2;
                indexmin=i;
            }
        }
    }else{
        indexmin=path_PrevIndex;
        dxmin=x_particle-path_x[indexmin];
        dymin=y_particle-path_y[indexmin];
        d2min=dxmin*dxmin+dymin*dymin;
        for (int i=1; i<=space; i++){
            int ip=path_PrevIndex+i;
            int im=path_PrevIndex-i;
            float dx,dy,d2;
            if (ip<path_N){
                dx=x_particle-path_x[ip];
                dy=y_particle-path_y[ip];
                d2=dx*dx+dy*dy;
                if (d2<d2min){
                    dxmin=dx;
                    dymin=dy;
                    d2min=d2;
                    indexmin=ip;
                }
            }
            if (im>=0){
            dx=x_particle-path_x[im];
            dy=y_particle-path_y[im];
            d2=dx*dx+dy*dy;
            if (d2<d2min){
                dxmin=dx;
                dymin=dy;
                d2min=d2;
                indexmin=im;
            }
            }
        }
    }

    int ip=indexmin+1;
    int im=indexmin-1;
    float angle;
    if (ip>=path_N){
        angle=path_A[im];
    }else if (im<0){
        angle=path_A[indexmin];
    }else{
        float dxp=x_particle-path_x[ip];
        float dyp=y_particle-path_y[ip];
        float d2p=dxp*dxp+dyp*dyp;
        float dxm=x_particle-path_x[im];
        float dym=y_particle-path_y[im];
        float d2m=dxm*dxm+dym*dym;
        if (d2p<=d2m){
            angle=path_A[indexmin];
        }else{
            angle=path_A[im];
        }
    }

    float d=-sin(angle)*dxmin+cos(angle)*dymin;
    middleAngle=angle-atan2(d,path_cL/Factor_pixTOmm)+M_PI;
    u_path=U_sat_global/(1+path_cA*path_K[indexmin]/Factor_pixTOmm)*(1-exp(-path_Dac[indexmin]*Factor_pixTOmm/2.0));

    path_PrevIndex=indexmin;
}

void Regulation::FindPath(float partX[], float partY[], float xtarg, float ytarg, int Nrpart)
{
    if (Nrpart<1)
        return;
    if (firstpath){
        qDebug()<<"First path finding";
        PathFinding_sendPosI(partX[0],partY[0]);
        PathFinding_sendTarget(xtarg,ytarg);
//        PathFinding_addObs(partX[1],partY[1]);
        for (int i=1;i<Nrpart;i++){
            PathFinding_addObs(partX[i],partY[i]);
//            qDebug()<<"Added Obstacle: "<<partX[i]<<partY[i];
        }
        pathready=PathFinding_run();
        if (pathready)
            firstpath=false;
    }
    if (pathready){
        if (pathisvalid){
            for (int i=0; i<200; i++){
                path_x[i]=myPathFinding.PathX_B[i];
                path_y[i]=myPathFinding.PathY_B[i];
            }
            path_PrevIndex=0;
            computeOpenPath();
        }
        PathFinding_sendPosI(partX[0],partY[0]);
        PathFinding_sendTarget(xtarg,ytarg);
//        PathFinding_addObs(partX[1],partY[1]);
        for (int i=1;i<Nrpart;i++){
            PathFinding_addObs(partX[i],partY[i]);
//            qDebug()<<"Added obstacle: "<<partX[i]<<partY[i];
        }
        pathready=false;
        qDebug()<<"New Path Calculation";
        Path_future = QtConcurrent::run(this,&Regulation::PathFinding_run);
        Path_watcher.setFuture(Path_future);
    }else{
        qDebug()<<"Path searching in process";
    }
}

bool Regulation::PathFinding_run()
{
    return myPathFinding.run();
}

void Regulation::PathFinding_sendPosI(float x, float y)
{
    myPathFinding.sendPosI(x,y);
}

void Regulation::PathFinding_sendTarget(float x, float y)
{
    myPathFinding.sendTarget(x,y);
}

void Regulation::PathFinding_addObs(float x, float y)
{
    myPathFinding.addObs(x,y);
}

void Regulation::PathFinding_clearObs()
{
    myPathFinding.clearObs();
}
