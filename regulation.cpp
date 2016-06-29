#include "regulation.h"
#include <QDebug>

Regulation::Regulation(): mirCtrl(){                                        //Creation of the regulation object
    qDebug()<<"Creation of Regulation object";

    ptsNumb = 100;                                                          //Cutting the circle into ptsNumb portions

    vectorLength = 100;                                                     //100 for 180°, 68 for 122.4° and 32 for 57.6°

    k_p =0.4558;
    T_i =8.962;                                                             //Creation of the regulator parameters
    k_t =0.37;
    T_s = 0.1;
}

bool Regulation::Initialisation(){
    bool properConnection = mirCtrl.Initialisation();                        //initialisation of the mirror
    return properConnection;
}


void Regulation::Figure (int type_regulation, int type_target, float r, int x_target, int y_target){

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
        qDebug()<<"Use of the point actuation. No figure creation";
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
}

void Regulation::Regulator (float particle_x, float particle_y){

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

    //r_las-part to r_part_center
    float r_part_center = radius - (r_las_part * 15.515);               //in pix

    //r_part_center to x/y_part_center
    float x_part_center = r_part_center * cos(alpha);
    float y_part_center = r_part_center * sin(alpha);

    //computation of the new center figure
    x = particle_x + x_part_center;
    y = particle_y + y_part_center;


    //Transfer new data stream to the mirror

    if (regulation_type == 0){          //Point actuation
        mirCtrl.ChangeMirrorPosition(x, y);
    }

    else if (regulation_type == 1){     //Circle actuation
        for(int j=0; j<ptsNumb; j++){
            x_vector[j] = figure_x[j] + x;
            y_vector[j] = figure_y[j] + y;
        }
        mirCtrl.ChangeMirrorStream (x_vector, y_vector);
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
            mirCtrl.ChangeMirrorStream (x_vector, y_vector);
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
                x_vector[i] = figure_x[circleIndex] + x;
                y_vector[i] = figure_y[circleIndex] + y;
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
                x_vector[i] = figure_x[circleIndex] + x;
                y_vector[i] = figure_y[circleIndex] + y;
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
                x_vector[i] = figure_x[circleIndex] + x;
                y_vector[i] = figure_y[circleIndex] + y;
                index++;
            }

            mirCtrl.ChangeMirrorStream (x_vector, y_vector);

        }

    }
}

void Regulation::closeRegulation (){
    qDebug()<<"Closing the mirror";
    mirCtrl.Closing();                                                  //Close the mirror
}
