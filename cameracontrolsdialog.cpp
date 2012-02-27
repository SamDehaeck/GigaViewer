#include "cameracontrolsdialog.h"
#include "ui_cameracontrolsdialog.h"

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
