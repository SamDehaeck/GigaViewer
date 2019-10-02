#include "ampliplugindialog.h"
#include "ui_ampliplugindialog.h"
#include <QDebug>


AmpliPluginDialog::AmpliPluginDialog(QWidget *parent) :
    QDialog(parent),tSliderPressed(false),activated(false),axisDefined(false),
    ui(new Ui::AmpliPluginDialog)
{
    ui->setupUi(this);
    extractData();
}

bool AmpliPluginDialog::extractData() {
    activated=ui->activateBox->isChecked();
    row=ui->rowSlider->value();
    return true;
}

bool AmpliPluginDialog::processImage(ImagePacket& currIm) {
    if (activated) {
        if (currIm.pixFormat=="RGB8") {
            cv::Mat grayIm;
            cv::cvtColor(currIm.image,grayIm,cv::COLOR_RGB2GRAY);
            currIm.image=grayIm.clone();
            currIm.pixFormat="MONO8";  // will make it fall through to the next part.
        }
        if (currIm.pixFormat=="MONO8") {
            int line=static_cast<int>(currIm.image.rows*(row/100.0));

            // extract pixels and plot
            QLineSeries *series = new QLineSeries();
            for (int j=0;j<currIm.image.cols;++j) {
                    *series << QPointF(j,currIm.image.at<uchar>(line,j));
            }

            if (not axisDefined) {
                myChart = new QChart();
                myChart->legend()->hide();
                myChart->addSeries(series);
                myChart->createDefaultAxes();
                myChart->setTitle("Simple line chart example");
                ui->pluginChart->setChart(myChart);
                axisDefined=true;
            } else {
                myChart->removeAllSeries();
                myChart->addSeries(series);
                myChart->createDefaultAxes();
            }

            // now indicate line in figure
            cv::line(currIm.image,cv::Point(0,line),cv::Point(currIm.image.cols,line),125,3);

        } else {
            qDebug()<<"Image format not yet supported! "<<currIm.pixFormat;
        }
    } else {
        //currIm.message.insert("Interference Contrast",-1);  // will show a value for the fringe amplitude (Max-Min illumination)
    }
    return true;
}

AmpliPluginDialog::~AmpliPluginDialog()
{
    delete ui;
}

void AmpliPluginDialog::on_activateBox_stateChanged(int)
{
    extractData();
}

void AmpliPluginDialog::on_rowSlider_sliderPressed()
{
    tSliderPressed=true;
}

void AmpliPluginDialog::on_rowSlider_sliderReleased()
{
    tSliderPressed=false;
    extractData();
}

void AmpliPluginDialog::on_rowSlider_valueChanged(int)
{
    if (!tSliderPressed) {
        extractData();
    }
}
