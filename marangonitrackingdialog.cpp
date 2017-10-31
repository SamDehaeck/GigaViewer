#include "marangonitrackingdialog.h"
#include "ui_marangonitrackingdialog.h"
#include <QDebug>


MarangoniTrackingDialog::MarangoniTrackingDialog(QWidget *parent) :
    QDialog(parent),tSliderPressed(false),xSliderPressed(false),ySliderPressed(false),
    ui(new Ui::MarangoniTrackingDialog)
{
    ui->setupUi(this);
    extractData();
}

bool MarangoniTrackingDialog::extractData() {
    QMap<QString,QVariant> settings;
    settings["pluginName"]="MarangoniTracking";
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

MarangoniTrackingDialog::~MarangoniTrackingDialog()
{
    delete ui;
}

void MarangoniTrackingDialog::on_activateBox_stateChanged(int val)
{
    if (val==0) {
        // state changed to off => stop tracking as well!
        ui->trackButton->toggle();
    }
    extractData();
}

void MarangoniTrackingDialog::on_trackButton_clicked(bool checked)
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
    extractData();
}

void MarangoniTrackingDialog::on_horPosition_valueChanged(int)
{
    if (!xSliderPressed) {
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
    extractData();
}

void MarangoniTrackingDialog::on_vertPosition_valueChanged(int value)
{
    if (!ySliderPressed) {
        extractData();
    }
}

void MarangoniTrackingDialog::on_threshEdit_textChanged(const QString &arg1)
{
    int tt=ui->threshEdit->text().toInt();
    ui->thresholdSlider->setValue(tt);
    extractData();
}

//NEW BOXES ADDED by RONALD
void MarangoniTrackingDialog::on_cbox_PatternType_currentIndexChanged(int index)
{
    extractData();
}

void MarangoniTrackingDialog::on_cbox_OpMode_currentIndexChanged(int index)
{
    extractData();
}

void MarangoniTrackingDialog::on_cbox_LasOrient_currentIndexChanged(int index)
{
    extractData();
}

void MarangoniTrackingDialog::on_line_LasPartD_textChanged(const QString &arg1)
{
    float tt=ui->threshEdit->text().toFloat();

    //If the user sets something outside 1.1-5, just replace that value witht the boundary
    if (tt<1.1)
    {tt=1.1;}

    if (tt>5)
    {tt=5;}

    extractData();


}
