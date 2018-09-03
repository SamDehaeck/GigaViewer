#ifndef MARANGONITRACKINGDIALOG_H
#define MARANGONITRACKINGDIALOG_H
#include <QtCore>
#include <QDialog>

namespace Ui {
class MarangoniTrackingDialog;
}

class MarangoniTrackingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MarangoniTrackingDialog(QWidget *parent = 0);
    ~MarangoniTrackingDialog();

signals:
    void stateChanged(QMap<QString,QVariant> newSettings);

private slots:
    void on_activateBox_stateChanged(int val);

    void on_thresholdSlider_sliderPressed();
    void on_thresholdSlider_sliderReleased();
    void on_thresholdSlider_valueChanged(int value);

    void on_horPosition_sliderPressed();
    void on_horPosition_sliderReleased();
    void on_horPosition_valueChanged(int value);

    void on_vertPosition_sliderPressed();
    void on_vertPosition_sliderReleased();
    void on_vertPosition_valueChanged(int value);

    void on_threshEdit_textChanged(const QString &arg1);

    void on_cbox_PatternType_currentIndexChanged(int index);

    void on_cbox_OpMode_currentIndexChanged(int index);

    void on_cbox_LasOrient_currentIndexChanged(int index);

    void on_line_LasPartD_textChanged(const QString &arg1);

    void on_line_kp_editingFinished();

    void on_line_Ti_editingFinished();

    void on_line_Td_editingFinished();

    void on_line_Usat_editingFinished();

    void on_line_Tt_editingFinished();

    void on_line_Tsamp_editingFinished();

    void on_line_Kff_editingFinished();

    void on_Xlaser_slider_sliderPressed();

    void on_Xlaser_slider_sliderReleased();

    void on_Xlaser_slider_valueChanged(int value);

    void on_Ylaser_slider_sliderPressed();

    void on_Ylaser_slider_sliderReleased();

    void on_Ylaser_slider_valueChanged(int value);

    void on_chkbox_Tracking_stateChanged(int arg1);

    void on_line_Xtarg_textChanged(const QString &arg1);

    void on_line_Ytarg_textChanged(const QString &arg1);

private:
    bool extractData();
    bool tSliderPressed,xSliderPressed,ySliderPressed;
    bool xLaserSlider_pressed,yxLaserSlider_pressed;




    Ui::MarangoniTrackingDialog *ui;
};

#endif // MARANGONITRACKINGDIALOG_H
