#ifndef INTERFEROPLUGINDIALOG_H
#define INTERFEROPLUGINDIALOG_H
#include <QtCore>
#include <QDialog>
#include <opencv2/opencv.hpp>

#include "imagepacket.h"

namespace Ui {
class InterferoPluginDialog;
}

class InterferoPluginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterferoPluginDialog(QWidget *parent = nullptr);
    bool processImage(ImagePacket& currIm);
    ~InterferoPluginDialog();

signals:
    void stateChanged(QMap<QString,QVariant> newSettings);

private slots:
    void on_activateBox_stateChanged(int val);
    void on_newReferenceButton_clicked();

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
    void fftshift(cv::Mat& I,bool forward);
    void filterDft(cv::Mat& I,int startC,int stopC);
    void PeakFinder(cv::Mat input,int* rowPos,int* colPos);
    void centrePeak(cv::Mat I,int rowPeak,int colPeak);
    void adaptForScreen(cv::Mat& I);

    bool tSliderPressed,xSliderPressed,ySliderPressed;
    bool activated,newReference;
    int rowPeak,colPeak;




    Ui::InterferoPluginDialog *ui;
};

#endif // InterferoPluginDIALOG_H
