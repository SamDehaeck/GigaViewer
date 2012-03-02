#ifndef CAMERACONTROLSDIALOG_H
#define CAMERACONTROLSDIALOG_H

#include <QDialog>
#include <QtGui>
#include "imagepacket.h"

namespace Ui {
class CameraControlsDialog;
}

class CameraControlsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CameraControlsDialog(QWidget *parent = 0);
    void GotNewShutterSpeed(int shut);
    ~CameraControlsDialog();
    
signals:
    void NeedNewSample();
    void SetShutterSpeed(int shut);

public slots:
    void GotNewSample(ImagePacket imP);


private slots:
    void on_IntensityButton_clicked();
    void on_shutterSpinBox_valueChanged(int arg1);

private:
    Ui::CameraControlsDialog *ui;
    int shutSpeed;
};

#endif // CAMERACONTROLSDIALOG_H
