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
    void on_trackButton_clicked(bool checked);

    void on_thresholdSlider_sliderPressed();
    void on_thresholdSlider_sliderReleased();
    void on_thresholdSlider_valueChanged(int value);

    void on_horPosition_sliderPressed();
    void on_horPosition_sliderReleased();
    void on_horPosition_valueChanged(int value);

    void on_vertPosition_sliderPressed();
    void on_vertPosition_sliderReleased();
    void on_vertPosition_valueChanged(int value);

    void on_IntervalleDiameter_stateChanged(int arg1);

    void on_AspectRatioBox_stateChanged(int arg1);

    void on_BlackWhiteBox_stateChanged(int arg1);

    void on_AspectRatioSlider_sliderPressed();

    void on_AspectRatioSlider_sliderReleased();

    void on_AspectRatioSlider_valueChanged(int value);

    //void on_ContoursIntervalleBox_stateChanged(int arg1);

    void on_CoverageBox_stateChanged(int arg1);

    void on_ValidationRatio_sliderPressed();

    void on_ValidationRatio_sliderReleased();

    void on_ValidationRatio_valueChanged(int value);

    void on_accDistance_textChanged(const QString &arg1);


    void on_GeometryLineBox_stateChanged(int);



    void on_CenterLineSlider_sliderPressed();

    void on_CenterLineSlider_sliderReleased();

    void on_CenterLineSlider_valueChanged(int value);

    void on_VerticalLineSlider_sliderPressed();

    void on_VerticalLineSlider_sliderReleased();

    void on_VerticalLineSlider_valueChanged(int value);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_DistanceCenterLine_textChanged(const QString &arg1);

private:
    bool extractData();
    bool tSliderPressed,xSliderPressed,ySliderPressed,aSliderPressed,ValidationRatioSliderPressed,CenterLineSliderPressed,VerticalLineSliderPressed;




    Ui::EllipseDetectionDialog *ui;
};

#endif // EllipseDetectionDIALOG_H
