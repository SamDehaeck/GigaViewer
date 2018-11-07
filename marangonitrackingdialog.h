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

    void on_checkBox_orientation_stateChanged(int val);

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

    void on_line_PatternR_textChanged(const QString &arg1);

    void on_line_PatternA_textChanged(const QString &arg1);

    void on_line_PathL_textChanged(const QString &arg1);

    void on_line_PathA_textChanged(const QString &arg1);

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

    void on_Alaser_slider_sliderPressed();

    void on_Alaser_slider_sliderReleased();

    void on_Alaser_slider_valueChanged(int value);

    void on_Rpattern_slider_sliderPressed();

    void on_Rpattern_slider_sliderReleased();

    void on_Rpattern_slider_valueChanged(int value);

    void on_Aux_slider_sliderPressed();

    void on_Aux_slider_sliderReleased();

    void on_Aux_slider_valueChanged(int value);

    void on_cbox_SampleSense_currentIndexChanged(int index);

    void on_line_SampleRate_editingFinished();

    void on_chkbox_Tracking_stateChanged(int arg1);

    void on_line_Xtarg_textChanged(const QString &arg1);

    void on_line_Ytarg_textChanged(const QString &arg1);

    void on_spinBox_NroPart_valueChanged(int i);

    void LclickOnImageDetected(QPointF p);
    void RclickOnImageDetected(QPointF p);

private:
    bool extractData();
    bool tSliderPressed,xSliderPressed,ySliderPressed,aSliderPressed;
    bool xLaserSlider_pressed,yLaserSlider_pressed,aLaserSlider_pressed;
    bool rPatternSlider_pressed,aPatternSlider_pressed;




    Ui::MarangoniTrackingDialog *ui;
};

#endif // MARANGONITRACKINGDIALOG_H
