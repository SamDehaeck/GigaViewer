#include "cameracontrolsdialog.h"
#include "ui_cameracontrolsdialog.h"
#include <opencv2/opencv.hpp>

CameraControlsDialog::CameraControlsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraControlsDialog)
{
    ui->setupUi(this);
}

CameraControlsDialog::~CameraControlsDialog()
{
    delete ui;
}

void CameraControlsDialog::on_IntensityButton_clicked()
{
    emit NeedNewSample();
}

void CameraControlsDialog::GotNewSample(ImagePacket imP)
{
    double max;
    cv::minMaxLoc(imP.image,NULL,&max);
    ui->IntensLabel->setText(QString::number(max));
    ui->ROIRows->setText(QString::number(imP.image.rows));
    ui->ROICols->setText(QString::number(imP.image.cols));
}

void CameraControlsDialog::on_shutterSpinBox_valueChanged(int arg1)
{
    emit SetShutterSpeed(arg1);
}
