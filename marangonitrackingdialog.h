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

    void on_threshEdit_textChanged(const QString &arg1);

private:
    bool extractData();
    bool tSliderPressed,xSliderPressed,ySliderPressed;




    Ui::MarangoniTrackingDialog *ui;
};

#endif // MARANGONITRACKINGDIALOG_H
