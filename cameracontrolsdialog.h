#ifndef CAMERACONTROLSDIALOG_H
#define CAMERACONTROLSDIALOG_H

#include <QDialog>

namespace Ui {
class CameraControlsDialog;
}

class CameraControlsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CameraControlsDialog(QWidget *parent = 0);
    ~CameraControlsDialog();
    
private:
    Ui::CameraControlsDialog *ui;
};

#endif // CAMERACONTROLSDIALOG_H
