#include "ellipsedetectiondialog.h"
#include "ui_ellipsedetectiondialog.h"
#include <QDebug>


EllipseDetectionDialog::EllipseDetectionDialog(QWidget *parent) :
    QDialog(parent),tSliderPressed(false),xSliderPressed(false),ySliderPressed(false),
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
    settings["shouldTrack"]=ui->trackButton->isChecked();

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
