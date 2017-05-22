#include "ellipsedetectiondialog.h"
#include "ui_ellipsedetectiondialog.h"
#include <QDebug>


EllipseDetectionDialog::EllipseDetectionDialog(QWidget *parent) :
    QDialog(parent),tSliderPressed(false),xSliderPressed(false),ySliderPressed(false),aSliderPressed(false),
    ui(new Ui::EllipseDetectionDialog)
{
    ui->setupUi(this);
    extractData();
}

bool EllipseDetectionDialog::extractData() {
    QMap<QString,QVariant> settings;
    settings["pluginName"]="EllipseDetection";
    settings["activated"]=ui->activateBox->isChecked();
    settings["threshold"]=ui->thresholdSlider->value();
    settings["targetX"]=ui->horPosition->value();
    settings["targetY"]=ui->vertPosition->value();
    settings["targetAspectRatio"]=ui->AspectRatioSlider->value();
    settings["targetValidationRatio"]=ui->ValidationRatio->value();
    settings["targetCenterLine"]=ui->CenterLineSlider->value();
    settings["targetVerticalLine"]=ui->VerticalLineSlider->value();
    settings["shouldTrack"]=ui->trackButton->isChecked();
    settings["CriteriumDiameterMaxMin"]=ui->IntervalleDiameter->isChecked();
    settings["CriteriumAspectRatio"]=ui->AspectRatioBox->isChecked();
    settings["CriteriumBlackWhite"]=ui->BlackWhiteBox->isChecked();
    settings["CriteriumCoverage"]=ui->CoverageBox->isChecked();
    settings["targetAcceptableDist"]=ui->accDistance->text();
    settings["targetB&WDistance"]=ui->DistanceBlackWhite->text();
    settings["targetDistanceCenterLine"]=ui->DistanceCenterLine->text();
    settings["CriteriumGeometry"]=ui->GeometryLineBox->isChecked();

    //settings["ContoursIntervalle"]=ui->ContoursIntervalleBox->isChecked();


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

EllipseDetectionDialog::~EllipseDetectionDialog()
{
    delete ui;
}

void EllipseDetectionDialog::on_activateBox_stateChanged(int val)
{
    if (val==0) {
        // state changed to off => stop tracking as well!
        ui->trackButton->toggle();
    }
    extractData();
}

void EllipseDetectionDialog::on_trackButton_clicked(bool checked)
{
    if (checked) {
        extractData();
    }
}



void EllipseDetectionDialog::on_thresholdSlider_sliderPressed()
{
    tSliderPressed=true;
}

void EllipseDetectionDialog::on_thresholdSlider_sliderReleased()
{
    tSliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_thresholdSlider_valueChanged(int)
{
    if (!tSliderPressed) {
        extractData();
    }
}

void EllipseDetectionDialog::on_horPosition_sliderPressed()
{
    xSliderPressed=true;
}

void EllipseDetectionDialog::on_horPosition_sliderReleased()
{
    xSliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_horPosition_valueChanged(int)
{
    if (!xSliderPressed) {
        extractData();
    }
}

void EllipseDetectionDialog::on_vertPosition_sliderPressed()
{
    ySliderPressed=true;
}

void EllipseDetectionDialog::on_vertPosition_sliderReleased()
{
    ySliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_vertPosition_valueChanged(int)
{
    if (!ySliderPressed) {
        extractData();
    }
}



void EllipseDetectionDialog::on_IntervalleDiameter_stateChanged(int)
{
    extractData();
}

void EllipseDetectionDialog::on_AspectRatioBox_stateChanged(int)
{
    extractData();
}


void EllipseDetectionDialog::on_BlackWhiteBox_stateChanged(int)
{
    extractData();
}

void EllipseDetectionDialog::on_AspectRatioSlider_sliderPressed()
{
    aSliderPressed=true;
}

void EllipseDetectionDialog::on_AspectRatioSlider_sliderReleased()
{
    aSliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_AspectRatioSlider_valueChanged(int)
{
    if(!aSliderPressed){
        extractData();
    }
}

//void EllipseDetectionDialog::on_ContoursIntervalleBox_stateChanged(int arg1)
//{
    //extractData();
//}

void EllipseDetectionDialog::on_CoverageBox_stateChanged(int)
{
    extractData();
}


void EllipseDetectionDialog::on_ValidationRatio_sliderPressed()
{
    ValidationRatioSliderPressed=true;
}

void EllipseDetectionDialog::on_ValidationRatio_sliderReleased()
{
    ValidationRatioSliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_ValidationRatio_valueChanged(int)
{
    if(!ValidationRatioSliderPressed){
        extractData();
    }
}

void EllipseDetectionDialog::on_accDistance_textChanged(const QString &arg1)
{
    extractData();
}



void EllipseDetectionDialog::on_GeometryLineBox_stateChanged(int)
{
    extractData();
}



void EllipseDetectionDialog::on_CenterLineSlider_sliderPressed()
{
    CenterLineSliderPressed=true;
}

void EllipseDetectionDialog::on_CenterLineSlider_sliderReleased()
{
    CenterLineSliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_CenterLineSlider_valueChanged(int)
{
    if(!CenterLineSliderPressed){
        extractData();
    }
}

void EllipseDetectionDialog::on_VerticalLineSlider_sliderPressed()
{
    VerticalLineSliderPressed=true;
}

void EllipseDetectionDialog::on_VerticalLineSlider_sliderReleased()
{
    VerticalLineSliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_VerticalLineSlider_valueChanged(int)
{
    if(!VerticalLineSliderPressed){
        extractData();
    }
}

void EllipseDetectionDialog::on_lineEdit_textChanged(const QString &arg1)
{
    extractData();
}


void EllipseDetectionDialog::on_DistanceCenterLine_textChanged(const QString &arg1)
{
    extractData();
}
