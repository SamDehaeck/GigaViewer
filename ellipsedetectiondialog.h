#ifndef ELLIPSEDETECTIONDIALOG_H
#define ELLIPSEDETECTIONDIALOG_H
#include <QtCore>
#include <QDialog>

namespace Ui {
class EllipseDetectionDialog;
}

class EllipseDetectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EllipseDetectionDialog(QWidget *parent = 0);
    ~EllipseDetectionDialog();

signals:
    void stateChanged(QMap<QString,QVariant> newSettings);

private slots:
    void on_activateBox_stateChanged(int val);
    void on_feedbackButton_clicked(bool checked);

    void on_thresholdSlider_sliderPressed();
    void on_thresholdSlider_sliderReleased();
    void on_thresholdSlider_valueChanged(int value);

    void on_MinDiameter_sliderPressed();
    void on_MinDiameter_sliderReleased();
    void on_MinDiameter_valueChanged(int value);

    void on_MaxDiameter_sliderPressed();
    void on_MaxDiameter_sliderReleased();
    void on_MaxDiameter_valueChanged(int value);

private:
    bool extractData();
    bool tSliderPressed,xSliderPressed,ySliderPressed;
    bool feedback;




    Ui::EllipseDetectionDialog *ui;
};

#endif // EllipseDetectionDIALOG_H
