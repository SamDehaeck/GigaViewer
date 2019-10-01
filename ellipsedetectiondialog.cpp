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
    settings["MinD"]=ui->MinDiameter->value();
    settings["MaxD"]=ui->MaxDiameter->value();
    settings["showFeedback"]=ui->feedbackButton->isChecked();
    //qInfo()<<"Sending new state"<<settings["activated"];
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
        if (feedback) {
            ui->feedbackButton->toggle();
        }
    }
    extractData();
}

void EllipseDetectionDialog::on_feedbackButton_clicked(bool checked)
{
    feedback=checked;
    extractData();
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

void EllipseDetectionDialog::on_MinDiameter_sliderPressed()
{
    xSliderPressed=true;
}

void EllipseDetectionDialog::on_MinDiameter_sliderReleased()
{
    xSliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_MinDiameter_valueChanged(int)
{
    if (!xSliderPressed) {
        extractData();
    }
}

void EllipseDetectionDialog::on_MaxDiameter_sliderPressed()
{
    ySliderPressed=true;
}

void EllipseDetectionDialog::on_MaxDiameter_sliderReleased()
{
    ySliderPressed=false;
    extractData();
}

void EllipseDetectionDialog::on_MaxDiameter_valueChanged(int)
{
    if (!ySliderPressed) {
        extractData();
    }
}
