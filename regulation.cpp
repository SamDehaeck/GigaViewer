#include "regulation.h"
#include <QDebug>




Regulation::Regulation() {                                        //Creation of the regulation object
    qDebug()<<"Creation of Regulation object";

    ptsNumb = 100;                                                          //Cutting the circle into ptsNumb portions

    vectorLength = 100;                                                     //100 for 180°, 68 for 122.4° and 32 for 57.6°

    k_p =2.6653;
    T_i =20.2429;                                                             //Creation of the regulator parameters
    k_t =0.00164;
    T_s = 0.0333;

}

void Regulation::Configure (int type_regulation, int type_target, float r, int x_target, int y_target){

    regulation_type = type_regulation; target_type = type_target; radius = r;
    increment = 0;
    counter = 0;

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

    if (regulation_type == 3) {                                //parameters for Regulator2017
        k_p =2.6653;
        T_i =20.2429;                                          //Creation of the regulator parameters
        k_t =0.00164;
        T_s = 0.0666;                                          //IDS limited to 15FPS for some reasons
        k_ff = 0.7;

        u0 = 0;          // state u(k)
        u1 = 0;          // state u(k-1)
        e0 = 0;          // state e(k)
        e1 = 0;          // state e(k-1)

        obstacle_x = 545;                                     //position of the obstacle
        obstacle_y = 298;
    }
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
    u_i = u_i + (k_p*T_s/T_i) * e0 - k_t*(u0 - u_sat);   //u_i(k+1) = ui(k) + smth * e(k) - kt*(u(k) - u_sat(k))


    // conversion polar to cartesian coordinates
    // the vector of that speed has to be add to the speed computed by the FF
    double u_PID_x = u_sat_PID * cos(alphaPID);
    double u_PID_y = u_sat_PID * sin(alphaPID);


    //************************** FEED FORWARD **************************//


    //distance between obstacle and particle (pixels to mm)
    float distFF_x = 0.064453*obstacle_x - 0.064453*particle_x;      //results in mm
    float distFF_y = 0.064453*obstacle_y - 0.064453*particle_y;

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
        //u_i = u_i + (k_p*T_s/T_i) * error - k_t*(u - u_sat);
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
                    circleIndex = circleIndex - ptsNumb;
                }
                if (circleIndex < 0){
                    circleIndex = circleIndex + ptsNumb;
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
