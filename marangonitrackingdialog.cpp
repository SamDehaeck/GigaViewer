#include "marangonitrackingdialog.h"
#include "ui_marangonitrackingdialog.h"
#include <QDebug>


MarangoniTrackingDialog::MarangoniTrackingDialog(QWidget *parent) :
    QDialog(parent),tSliderPressed(false),xSliderPressed(false),ySliderPressed(false),
    xLaserSlider_pressed(false),yLaserSlider_pressed(false),aLaserSlider_pressed(false),rPatternSlider_pressed(false),aPatternSlider_pressed(false),
    ui(new Ui::MarangoniTrackingDialog)
{
    ui->setupUi(this);
    extractData();
}

bool MarangoniTrackingDialog::extractData() {
    int aux;
    QMap<QString,QVariant> settings;
    settings["pluginName"]="MarangoniTracking";
    settings["activated"]=ui->activateBox->isChecked();
    settings["threshold"]=ui->thresholdSlider->value();
    settings["NroPart"]=ui->spinBox_NroPart->value();
    settings["targetX"]=ui->horPosition->value();
    settings["targetY"]=ui->vertPosition->value();
    //New control gains
    settings["kp"]=ui->line_kp->text();         //Proportional gain
    //qDebug()<<settings["kp"];
    settings["Ti"]=ui->line_Ti->text();         //Gain Integral
    settings["Td"]=ui->line_Td->text();         //Gain Derivative
    settings["Tt"]=ui->line_Tt->text();         //Gain anti windup
    settings["Tsamp"]=ui->line_Tsamp->text();      //Sampling time
    settings["Usat"]=ui->line_Usat->text();      //Gain limit (in terms of speed)

    settings["kff"]=ui->line_Kff->text();       //Gain FeedForward controller


    settings["OpMode"]= ui->cbox_OpMode->currentIndex();
    settings["PatternType"]= ui->cbox_PatternType->currentIndex();

    settings["Program_ID"]= 100*settings["OpMode"].toInt()+settings["PatternType"].toInt();


    settings["Ident_LasPart_D"] = ui->line_LasPartD->text();
    settings["Ident_PatternR_D"] = ui->line_PatternR->text();
    settings["Ident_PatternA_D"] = ui->line_PatternA->text();
    settings["Ident_LasPos"] = ui->cbox_LasOrient->currentIndex(); // 0 Bottom 1 Right 2 Top  3 Left

    //Extract the laser position
    settings["X_laserPOS"] = ui->Xlaser_slider->value();
    settings["Y_laserPOS"] = ui->Ylaser_slider->value();
    settings["A_laserPOS"] = ui->Alaser_slider->value();
    settings["R_patternPOS"] = (ui->Rpattern_slider->value());
    settings["A_patternPOS"] = ui->Aux_slider->value();
    settings["SampleRate"] = ui->line_SampleRate->text();
    settings["SampleSense"] = ui->cbox_SampleSense->currentIndex();
    settings["TrackingON"]=ui->chkbox_Tracking->isChecked();        //0 Not activated, 1 Activated

    emit stateChanged(settings);

/*
    QMap<QString, QVariant>::const_iterator i = settings.constBegin();
    while (i != settings.constEnd()) {
        qDebug() << i.key() << ": " << i.value();
        ++i;
    }
    qDebug()<<"-----------";
*/
    return true;
}

MarangoniTrackingDialog::~MarangoniTrackingDialog()
{
    delete ui;
}

void MarangoniTrackingDialog::on_activateBox_stateChanged(int val)
{

    extractData();
}

void MarangoniTrackingDialog::on_thresholdSlider_sliderPressed()
{
    tSliderPressed=true;
}

void MarangoniTrackingDialog::on_thresholdSlider_sliderReleased()
{
    tSliderPressed=false;
    int nr=ui->thresholdSlider->value();
    ui->threshEdit->setText(QString::number(nr));
    extractData();
}

void MarangoniTrackingDialog::on_thresholdSlider_valueChanged(int)
{
    if (!tSliderPressed) {
        int nr=ui->thresholdSlider->value();
        ui->threshEdit->setText(QString::number(nr));
        extractData();
    }
}

void MarangoniTrackingDialog::on_horPosition_sliderPressed()
{
    xSliderPressed=true;
}

void MarangoniTrackingDialog::on_horPosition_sliderReleased()
{
    xSliderPressed=false;
    int nr=ui->horPosition->value();                //Get the current slider value and set it to the textbox
    ui->line_Xtarg->setText(QString::number(nr));
    extractData();

}

void MarangoniTrackingDialog::on_horPosition_valueChanged(int)
{
    if (!xSliderPressed) {
        int nr=ui->horPosition->value();                //Get the current slider value and set it to the textbox
        ui->line_Xtarg->setText(QString::number(nr));
        extractData();
    }
}

void MarangoniTrackingDialog::on_vertPosition_sliderPressed()
{
    ySliderPressed=true;
}

void MarangoniTrackingDialog::on_vertPosition_sliderReleased()
{
    ySliderPressed=false;
    int nr=ui->vertPosition->value();                //Get the current slider value and set it to the textbox
    ui->line_Ytarg->setText(QString::number(nr));
    extractData();
}

void MarangoniTrackingDialog::on_vertPosition_valueChanged(int value)
{
    if (!ySliderPressed) {
        int nr=ui->vertPosition->value();                //Get the current slider value and set it to the textbox
        ui->line_Ytarg->setText(QString::number(nr));
        extractData();
    }
}

void MarangoniTrackingDialog::on_threshEdit_textChanged(const QString &arg1)
{
    int tt=ui->threshEdit->text().toInt();
    ui->thresholdSlider->setValue(tt);
    extractData();
}

void MarangoniTrackingDialog::on_line_Xtarg_textChanged(const QString &arg1)
{
    int tt=ui->line_Xtarg->text().toInt();
    ui->horPosition->setValue(tt);
    extractData();
}

void MarangoniTrackingDialog::on_line_Ytarg_textChanged(const QString &arg1)
{
    int tt=ui->line_Ytarg->text().toInt();
    ui->vertPosition->setValue(tt);
    extractData();
}

void MarangoniTrackingDialog::on_spinBox_NroPart_valueChanged(int i)
{
    extractData();
}

void MarangoniTrackingDialog::LclickOnImageDetected(QPointF p)
{
    ui->Xlaser_slider->setValue(p.x()*1024);
    ui->Ylaser_slider->setValue(p.y()*1024);
    ui->horPosition->setValue(p.x()*1024);
    ui->vertPosition->setValue(p.y()*1024);
    extractData();
}

void MarangoniTrackingDialog::RclickOnImageDetected(QPointF p)
{
    int xp=p.x()*1024-ui->Xlaser_slider->value();
    int yp=p.y()*1024-ui->Ylaser_slider->value();
    int a=qAtan2(yp,xp)*180.0/M_PI;
    if (a>180)
        a-=360;
    ui->Alaser_slider->setValue(a);
    if (ui->cbox_PatternType->currentIndex()==4){
        double r=sqrt((double)xp*(double)xp+(double)yp*(double)yp)*650.0/1024.0;
        ui->Rpattern_slider->setValue(r);
    }
    extractData();
}


//NEW BOXES ADDED by RONALD
void MarangoniTrackingDialog::on_cbox_PatternType_currentIndexChanged(int index)
{
    int Program_Id = ui->cbox_OpMode->currentIndex()*100 + ui->cbox_PatternType->currentIndex();
            ; //By default everything is in the mode Point-wise with FB

    if( Program_Id == 100||Program_Id == 500 )         //Change gains to Point-wise  + FeedBack
        { ui->line_kp->clear();      ui->line_kp->insert("3.0933");
        ui->line_Ti->clear();       ui->line_Ti->insert("1.8521");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("28.309");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.0");

    } else if( Program_Id == 101||Program_Id == 501 ) //Change gains to Arc/Circle  + FeedBack
    {ui->line_kp->clear();      ui->line_kp->insert("0.4558");
        ui->line_Ti->clear();       ui->line_Ti->insert("0.11158");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("0.37");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.0");

    } else if( Program_Id ==200 ) //Change gains to Point-wise  + FeedForward
    {ui->line_kp->clear();      ui->line_kp->insert("2.6653");
        ui->line_Ti->clear();       ui->line_Ti->insert("20.2429");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("305.644");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.7");

    }   else if( Program_Id ==201 ) //Change gains to Arc/Circle + FeedForward
    {ui->line_kp->clear();      ui->line_kp->insert("0.0");
        ui->line_Ti->clear();       ui->line_Ti->insert("0.0");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("0.0");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.0");

    }


    extractData();

}

void MarangoniTrackingDialog::on_cbox_OpMode_currentIndexChanged(int index)
{
    int Program_Id = ui->cbox_OpMode->currentIndex()*100 + ui->cbox_PatternType->currentIndex();
            ; //By default everything is in the mode Point-wise with FB

    if( Program_Id == 100||Program_Id == 500 )         //Change gains to Point-wise  + FeedBack
        { ui->line_kp->clear();      ui->line_kp->insert("3.0933");
        ui->line_Ti->clear();       ui->line_Ti->insert("1.8521");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("28.309");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.0");

    } else if( Program_Id == 101||Program_Id == 501 ) //Change gains to Arc/Circle  + FeedBack
    {ui->line_kp->clear();      ui->line_kp->insert("0.4558");
        ui->line_Ti->clear();       ui->line_Ti->insert("0.11158");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("0.37");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.0");

    } else if( Program_Id ==200 ) //Change gains to Point-wise  + FeedForward
    {ui->line_kp->clear();      ui->line_kp->insert("2.6653");
        ui->line_Ti->clear();       ui->line_Ti->insert("20.2429");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("0.00164");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.7");

    }   else if( Program_Id ==201 ) //Change gains to Arc/Circle + FeedForward
    {ui->line_kp->clear();      ui->line_kp->insert("0.0");
        ui->line_Ti->clear();       ui->line_Ti->insert("0.0");
        ui->line_Td->clear();       ui->line_Td->insert("0.0");
        ui->line_Tt->clear();       ui->line_Tt->insert("0.0");
        ui->line_Kff->clear();      ui->line_Kff->insert("0.0");

    }


    //        k_p =2.6653;
    //        T_i =20.2429;                                          //Creation of the regulator parameters
    //        k_t =0.00164;
    //        T_s = 0.0666;                                          //IDS limited to 15FPS for some reasons
    //        k_ff = 0.7;


    extractData();
}

void MarangoniTrackingDialog::on_cbox_LasOrient_currentIndexChanged(int index)
{
    extractData();
}

void MarangoniTrackingDialog::on_line_LasPartD_textChanged(const QString &arg1)
{
//    float tt=ui->line_LasPartD->text().toFloat();

    //If the user sets something outside 1.1-5, just replace that value witht the boundary
//    if (tt<1.0)
//    {tt=1.0;  ui->line_LasPartD->clear(); ui->line_LasPartD->insert( QString::number(tt) );}

//    if (tt>5)
//    {tt=5;    ui->line_LasPartD->clear(); ui->line_LasPartD->insert( QString::number(tt) );}



    extractData();


}
void MarangoniTrackingDialog::on_line_PatternR_textChanged(const QString &arg1)
{
    extractData();
}
void MarangoniTrackingDialog::on_line_PatternA_textChanged(const QString &arg1)
{
    extractData();
}

void MarangoniTrackingDialog::on_line_kp_editingFinished()
{
    extractData();
}

void MarangoniTrackingDialog::on_line_Ti_editingFinished()
{
    extractData();
}

void MarangoniTrackingDialog::on_line_Td_editingFinished()
{
    extractData();
}

void MarangoniTrackingDialog::on_line_Usat_editingFinished()
{
    extractData();
}

void MarangoniTrackingDialog::on_line_Tt_editingFinished()
{
    extractData();
}

void MarangoniTrackingDialog::on_line_Tsamp_editingFinished()
{
    extractData();
}

void MarangoniTrackingDialog::on_line_Kff_editingFinished()
{
    extractData();
}


//Sliders to change the laser position
void MarangoniTrackingDialog::on_Xlaser_slider_sliderPressed()
{
    xLaserSlider_pressed=true;
}

void MarangoniTrackingDialog::on_Xlaser_slider_sliderReleased()
{
    xLaserSlider_pressed=false;
    int nr=ui->Xlaser_slider->value();                //Get the current slider value and set it to the textbox
    ui->Xlaser_label->setText(QString::number(nr)+"px");
    extractData();
}


void MarangoniTrackingDialog::on_Xlaser_slider_valueChanged(int value)
{
    if(!xLaserSlider_pressed){
        int nr=ui->Xlaser_slider->value();                //Get the current slider value and set it to the textbox
        ui->Xlaser_label->setText(QString::number(nr)+"px");
        extractData();
    }
}

void MarangoniTrackingDialog::on_Ylaser_slider_sliderPressed()
{
    yLaserSlider_pressed=true;
}

void MarangoniTrackingDialog::on_Ylaser_slider_sliderReleased()
{
    yLaserSlider_pressed=false;
    int nr=ui->Ylaser_slider->value();                //Get the current slider value and set it to the textbox
    ui->Ylaser_label->setText(QString::number(nr)+"px");
    extractData();
}

void MarangoniTrackingDialog::on_Ylaser_slider_valueChanged(int value)
{
    if(!yLaserSlider_pressed){
        int nr=ui->Ylaser_slider->value();                //Get the current slider value and set it to the textbox
        ui->Ylaser_label->setText(QString::number(nr)+"px");
        extractData();
    }
}

//Sliders to change the laser orientation
void MarangoniTrackingDialog::on_Alaser_slider_sliderPressed()
{
    aLaserSlider_pressed=true;
}

void MarangoniTrackingDialog::on_Alaser_slider_sliderReleased()
{
    aLaserSlider_pressed=false;
    int nr=ui->Alaser_slider->value();                //Get the current slider value and set it to the textbox
    ui->Alaser_label->setText(QString::number(nr)+"°");
    extractData();
}

void MarangoniTrackingDialog::on_Alaser_slider_valueChanged(int value)
{
    if(!aLaserSlider_pressed){
        int nr=ui->Alaser_slider->value();                //Get the current slider value and set it to the textbox
        ui->Alaser_label->setText(QString::number(nr)+"°");
        extractData();
    }
}

//Sliders to change the laser pattern parameters
void MarangoniTrackingDialog::on_Rpattern_slider_sliderPressed()
{
    rPatternSlider_pressed=true;
}

void MarangoniTrackingDialog::on_Rpattern_slider_sliderReleased()
{
    rPatternSlider_pressed=false;
    float nr=ui->Rpattern_slider->value();                //Get the current slider value and set it to the textbox
    ui->Rpattern_label->setText(QString::number(nr/10)+"mm");
    extractData();
}

void MarangoniTrackingDialog::on_Rpattern_slider_valueChanged(int value)
{
    if(!rPatternSlider_pressed){
        float nr=ui->Rpattern_slider->value();                //Get the current slider value and set it to the textbox
        ui->Rpattern_label->setText(QString::number(nr/10)+"mm");
        extractData();
    }
}

void MarangoniTrackingDialog::on_Aux_slider_sliderPressed()
{
    aPatternSlider_pressed=true;
}

void MarangoniTrackingDialog::on_Aux_slider_sliderReleased()
{
    aPatternSlider_pressed=false;
    int nr=ui->Aux_slider->value();                //Get the current slider value and set it to the textbox
    ui->Aux_label->setText(QString::number(nr)+"%");
    extractData();
}

void MarangoniTrackingDialog::on_Aux_slider_valueChanged(int value)
{
    if(!aPatternSlider_pressed){
        int nr=ui->Aux_slider->value();                //Get the current slider value and set it to the textbox
        ui->Aux_label->setText(QString::number(nr)+"%");
        extractData();
    }
}

void MarangoniTrackingDialog::on_cbox_SampleSense_currentIndexChanged(int index)
{
    extractData();
}

void MarangoniTrackingDialog::on_line_SampleRate_editingFinished()
{
    if (ui->line_SampleRate->text().toFloat()>100000)
        ui->line_SampleRate->setText(QString::number(100000));
    if (ui->line_SampleRate->text().toFloat()<200)
        ui->line_SampleRate->setText(QString::number(200));
    extractData();
}


void MarangoniTrackingDialog::on_chkbox_Tracking_stateChanged(int arg1)
{
    extractData();
}



