#include "interferoplugindialog.h"
#include "ui_interferoplugindialog.h"
#include <QDebug>


InterferoPluginDialog::InterferoPluginDialog(QWidget *parent) :
    QDialog(parent),tSliderPressed(false),xSliderPressed(false),ySliderPressed(false),
    ui(new Ui::InterferoPluginDialog)
{
    ui->setupUi(this);
    extractData();
}

bool InterferoPluginDialog::extractData() {
    QMap<QString,QVariant> settings;
    settings["pluginName"]="InterferoPlugin";
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

InterferoPluginDialog::~InterferoPluginDialog()
{
    delete ui;
}

void InterferoPluginDialog::on_activateBox_stateChanged(int val)
{
    extractData();
}

void InterferoPluginDialog::on_feedbackButton_clicked(bool checked)
{
    feedback=checked;
    extractData();
}



void InterferoPluginDialog::on_thresholdSlider_sliderPressed()
{
    tSliderPressed=true;
}

void InterferoPluginDialog::on_thresholdSlider_sliderReleased()
{
    tSliderPressed=false;
    extractData();
}

void InterferoPluginDialog::on_thresholdSlider_valueChanged(int)
{
    if (!tSliderPressed) {
        extractData();
    }
}

void InterferoPluginDialog::on_MinDiameter_sliderPressed()
{
    xSliderPressed=true;
}

void InterferoPluginDialog::on_MinDiameter_sliderReleased()
{
    xSliderPressed=false;
    extractData();
}

void InterferoPluginDialog::on_MinDiameter_valueChanged(int)
{
    if (!xSliderPressed) {
        extractData();
    }
}

void InterferoPluginDialog::on_MaxDiameter_sliderPressed()
{
    ySliderPressed=true;
}

void InterferoPluginDialog::on_MaxDiameter_sliderReleased()
{
    ySliderPressed=false;
    extractData();
}

void InterferoPluginDialog::on_MaxDiameter_valueChanged(int)
{
    if (!ySliderPressed) {
        extractData();
    }
}
