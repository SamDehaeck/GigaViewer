#ifndef AMPLIPLUGINDIALOG_H
#define AMPLIPLUGINDIALOG_H
#include <QtCore>
#include <QDialog>
#include <QtCharts>
#include "imagepacket.h"
#include <opencv2/opencv.hpp>

namespace Ui {
class AmpliPluginDialog;
}

class AmpliPluginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AmpliPluginDialog(QWidget *parent = nullptr);
    bool processImage(ImagePacket& currIm);
    ~AmpliPluginDialog();

signals:
    void stateChanged(QMap<QString,QVariant> newSettings);

private slots:
    void on_activateBox_stateChanged(int);

    void on_rowSlider_sliderPressed();
    void on_rowSlider_sliderReleased();
    void on_rowSlider_valueChanged(int value);

private:
    bool extractData();
    bool tSliderPressed;
    bool activated;
    bool axisDefined;
    QChart *myChart;
    int row;




    Ui::AmpliPluginDialog *ui;
};

#endif // AMPLIPluginDIALOG_H
