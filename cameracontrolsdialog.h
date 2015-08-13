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
    void SetAutoShutter(bool fitRange);
    void SetRoiRows(int rows);
    void SetRoiCols(int cols);

public slots:
    void GotNewSample(ImagePacket imP);


private slots:
    void on_IntensityButton_clicked();
    void on_shutterSpinBox_valueChanged(int arg1);

    void on_FitRangeButton_clicked();

    void on_FitMeanButton_clicked();

    void on_ROIRows_editingFinished();

    void on_ROICols_editingFinished();

    void on_pushButton_2_clicked();

private:
    Ui::CameraControlsDialog *ui;
    int shutSpeed;
};

#endif // CAMERACONTROLSDIALOG_H
