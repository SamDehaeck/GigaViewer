#include "cameracontrolsdialog.h"
#include "ui_cameracontrolsdialog.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <QDebug>



CameraControlsDialog::CameraControlsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraControlsDialog)
{
    ui->setupUi(this);
}

CameraControlsDialog::~CameraControlsDialog()
{
    delete ui;
}

void CameraControlsDialog::on_IntensityButton_clicked()
{
    emit NeedNewSample();
}

void CameraControlsDialog::GotNewSample(ImagePacket imP)
{
    double max;
    cv::minMaxLoc(imP.image,NULL,&max);
    ui->IntensLabel->setText(QString::number(max));
    ui->ROIRows->setText(QString::number(imP.image.rows));
    ui->ROICols->setText(QString::number(imP.image.cols));

    if (imP.image.channels()==1) {
        // now do the fft to find the pattern frequency
        cv::Mat padded;                            //expand input image to optimal size
        int m = cv::getOptimalDFTSize( imP.image.rows );
        int n = cv::getOptimalDFTSize( imP.image.cols ); // on the border add zero values
        if (m<n) {  // not really necessary but done to avoid problems in the analysis of the period and angle
            m=n;
        } else if (n<m) {
            n=m;
        }
        cv::copyMakeBorder(imP.image, padded, 0, m - imP.image.rows, 0, n - imP.image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

        cv::Mat result= cv::Mat(m,n,CV_32FC2);
        std::vector<cv::Mat> chan;
        cv::split(result,chan);
        padded.convertTo(chan[0],CV_32F);
        cv::Scalar me=cv::mean(chan[0]);
    //    qDebug()<<"Mean of image is"<<me[0];
        chan[0]= chan[0]-me;

        chan[1]=0.0;
        cv::merge(chan,result);

        cv::dft(result, result);            // this way the result may fit in the source matrix

        // compute the magnitude and switch to logarithmic scale
        // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
        cv::split(result, chan);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
        cv::Mat magI;
        cv::magnitude(chan[0], chan[1], magI);// planes[0] = magnitude

        magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
        int cx = magI.cols/2;
        int cy = magI.rows/2;

        cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    //    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
        cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    //    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

        cv::Mat tmp;             // swap quadrants (Keep only left part of original (right after swap)
        q0.copyTo(tmp);
        q2.copyTo(q0);
        tmp.copyTo(q2);

        int xoffset=10; // to avoid using the DC peak, implies a minimum period of n/10 => 1024x1024 => 102 pixel period minimum!!
        cv::Mat newTemp=magI(cv::Rect(xoffset,0,cx-xoffset,magI.rows));

        cv::Point maxInd;
        double maxMag;
        cv::minMaxLoc(newTemp,NULL,&maxMag,NULL,&maxInd);
        double period=m/(sqrt(pow((maxInd.x),2.)+pow((maxInd.y-cy),2.))+xoffset); //problem if m!=n !!!! // add a '.' after the 2 to prevent error with type conversion msvc10 bug...
        double angle=atan2((double)(cy)-maxInd.y,maxInd.x)*180.0/3.1415965359; // type conversion of cy to avoid an error with msvc10, again...

        ui->IntensLabel->setText("Max Intensity: "+QString::number(max)+" - Dominant period: "+QString::number(period,'f',2)+" at "+QString::number(angle,'f',2)+QChar(0xB0));
    } else {
        ui->IntensLabel->setText("Max Intensity: "+QString::number(max));
    }



}

void CameraControlsDialog::GotNewShutterSpeed(int shut)
{
    shutSpeed=shut;
    ui->shutterSpinBox->setValue(shut);
}

void CameraControlsDialog::on_shutterSpinBox_valueChanged(int arg1)
{
    if (arg1!=shutSpeed) {
        emit SetShutterSpeed(arg1);
        shutSpeed=arg1;
    }
}

void CameraControlsDialog::on_FitRangeButton_clicked()
{
    emit SetAutoShutter(true);
}

void CameraControlsDialog::on_FitMeanButton_clicked()
{
    emit SetAutoShutter(false);
}
