#include "interferoplugindialog.h"
#include "ui_interferoplugindialog.h"
#include <QDebug>


InterferoPluginDialog::InterferoPluginDialog(QWidget *parent) :
    QDialog(parent),ssSliderPressed(false),skSliderPressed(false),newReference(true),rowPeak(-1),colPeak(-1),subsample(1),skip(0),fringePeriod(0),fringeAngle(0),
    ui(new Ui::InterferoPluginDialog)
{
    ui->setupUi(this);


    extractData();
}

bool InterferoPluginDialog::extractData() {
    activated=ui->activateBox->isChecked();
    skip=ui->skipSlider->value();
    if (subsample!=ui->subsampleSlider->value()) {
        newReference=true;
    }
    subsample=ui->subsampleSlider->value();
    return true;
}

bool InterferoPluginDialog::processImage(ImagePacket& currIm) {
    if (activated) {
        // deal with skips here.
        if (frameCounter%(skip+1)!=0) {
            frameCounter++;
            return false;
        }
        frameCounter+=1;

        if (currIm.pixFormat=="RGB8") {
            cv::Mat grayIm;
            cv::cvtColor(currIm.image,grayIm,cv::COLOR_RGB2GRAY);
            currIm.image=grayIm.clone();
            currIm.pixFormat="MONO8";  // will make it fall through to the next part.
        }
        if (currIm.pixFormat=="MONO8") {
            // first deal with subsampling
            cv::Mat subImage;
            cv::resize(currIm.image,subImage,cv::Size(),1.0/subsample,1.0/subsample,cv::INTER_NEAREST);

            // now do the fft
            cv::Mat padded;                            //expand input image to optimal size
            int m = cv::getOptimalDFTSize( subImage.rows );
            int n = cv::getOptimalDFTSize( subImage.cols ); // on the border add zero values
            if (m<n) {  // not really necessary but done to avoid problems in the analysis of the period and angle
                m=n;
            } else if (n<m) {
                n=m;
            }
            cv::copyMakeBorder(subImage, padded, 0, m - subImage.rows, 0, n - subImage.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

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

            if (newReference) {
                PeakFinder(result,&rowPeak,&colPeak);
                double newRow=rowPeak;
                if (result.rows-rowPeak<rowPeak) {
                    newRow=rowPeak-result.rows;
                }
                fringePeriod=(1.0*result.cols)/(sqrt(pow(colPeak,2)+pow(newRow,2)));
                fringeAngle=atan2(1.0*newRow,colPeak)*180.0/3.1415965359; // type conversion of cy to avoid an error with msvc10, again...
                //qDebug()<<"Period is "<<fringePeriod<<" angle is "<<fringeAngle;
                QString newLabel;
                newLabel.sprintf("Fringe period %.2f and angle %.2f",fringePeriod*subsample,fringeAngle);
                ui->fringeLabel->setText(newLabel);
                newReference=false;
            }
            filterDft(result,colPeak-colPeak/2,3*colPeak/2);

            bool recentrePeak=true;

            if (recentrePeak) {
                centrePeak(result,colPeak,rowPeak);
            }


            bool showPhase=true;
            if (showPhase) {
                cv::idft(result,result);
                cv::split(result, chan);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
                cv::Mat phaseI;
                cv::phase(chan[0], chan[1], phaseI);// planes[0] = magnitude

                cv::Mat phaseCrop=phaseI(cv::Rect(0,0,subImage.cols,subImage.rows));

                adaptForScreen(phaseCrop);
                currIm.image=phaseCrop.clone();
            }
            //currIm.image=padded.clone();

            bool showFourierPlane=false;

            if (showFourierPlane) {
                // compute the magnitude and switch to logarithmic scale
                // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
                cv::split(result, chan);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
                cv::Mat magI;
                cv::magnitude(chan[0], chan[1], magI);// planes[0] = magnitude

                magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

                fftshift(magI,true);

                cv::Mat logMagI;
                cv::log(magI,logMagI);
                adaptForScreen(logMagI);
                currIm.image=logMagI.clone();
            }



/* code to extract properties - not used for now
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
            cv::minMaxLoc(newTemp,nullptr,&maxMag,nullptr,&maxInd);
            double period=m/(sqrt(pow((maxInd.x),2.)+pow((maxInd.y-cy),2.))+xoffset); //problem if m!=n !!!! // add a '.' after the 2 to prevent error with type conversion msvc10 bug...
            double angle=atan2((double)(cy)-maxInd.y,maxInd.x)*180.0/3.1415965359; // type conversion of cy to avoid an error with msvc10, again...
            qDebug()<<"Period= "<<period<<" angle "<<angle<<"magnitude "<<(maxMag/(currIm.image.rows*currIm.image.cols));

            //currIm.image=magI;
            //ui->IntensLabel->setText("Max Intensity: "+QString::number(max)+" - Dominant period: "+QString::number(period,'f',2)+" at "+QString::number(angle,'f',2)+QChar(0xB0));
            */
        }
    }
    return true;
}

void InterferoPluginDialog::fftshift(cv::Mat& I,bool forward) {
    int cx = I.cols/2;
    int cy = I.rows/2;

    if (forward) {
        cv::Mat q0(I, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
        cv::Mat q1(I, cv::Rect(cx, 0, cx, cy));  // Top-Right
        cv::Mat q2(I, cv::Rect(0, cy, cx, cy));  // Bottom-Left
        cv::Mat q3(I, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

        cv::Mat tmp;
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);

        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);
    }
}

void InterferoPluginDialog::filterDft(cv::Mat& I,int startC,int stopC) {
    cv::Mat filterMatrix=cv::Mat(I.rows,I.cols,CV_32FC2,cv::Scalar(0.0,0.0));
    cv::Mat on=cv::Mat(filterMatrix,cv::Rect(startC,0,stopC-startC,I.rows));
    on=cv::Scalar(1.0,1.0);
    GaussianBlur(filterMatrix,filterMatrix,cv::Size(21,1),0);
    cv::multiply(I, filterMatrix,I);
}

void InterferoPluginDialog::PeakFinder(cv::Mat input,int* rowPos,int* colPos) {
    cv::Mat result2;
    std::vector<cv::Mat> chan;
    cv::split(input,chan);
    cv::magnitude(chan[0],chan[1],result2);
    int xshift=20;
    cv::Mat subresult=cv::Mat(result2,cv::Rect(xshift,0,(result2.cols/2)-2*xshift,result2.rows));

    double min,max;
    cv::Point minLoc,maxLoc;
    minMaxLoc(subresult,&min,&max,&minLoc,&maxLoc);
    *rowPos=maxLoc.y;
    *colPos=maxLoc.x+xshift;
}

void InterferoPluginDialog::centrePeak(cv::Mat matrix,int peakx,int peaky) {
    int R=matrix.rows;
    int C=matrix.cols;

    cv::Mat topright=cv::Mat(matrix,cv::Rect(peakx,0,C-peakx,peaky));
    cv::Mat bottomright=cv::Mat(matrix,cv::Rect(peakx,peaky,C-peakx,R-peaky));
    cv::Mat topleft=cv::Mat(matrix,cv::Rect(0,0,peakx,peaky));
    cv::Mat bottomleft=cv::Mat(matrix,cv::Rect(0,peaky,peakx,R-peaky));


    cv::Mat dummy=cv::Mat(R,C,matrix.type(),cv::Scalar(0.0,0.0));
    cv::Mat Dbottomleft=cv::Mat(dummy,cv::Rect(0,R-peaky,C-peakx,peaky));
    cv::Mat Dtopleft=cv::Mat(dummy,cv::Rect(0,0,C-peakx,R-peaky));
    cv::Mat Dtopright=cv::Mat(dummy,cv::Rect(C-peakx,0,peakx,R-peaky));
    cv::Mat Dbottomright=cv::Mat(dummy,cv::Rect(C-peakx,R-peaky,peakx,peaky));

    topright.copyTo(Dbottomleft);
    bottomright.copyTo(Dtopleft);
    bottomleft.copyTo(Dtopright);
    topleft.copyTo(Dbottomright);

    dummy.copyTo(matrix);
}


void InterferoPluginDialog::adaptForScreen(cv::Mat& I) {
    double min,max;
    cv::minMaxLoc(I,&min,&max);
    double stretch=255.0/(max-min);
    double shift=-min*stretch;
    cv::Mat temp;
    I.convertTo(temp,CV_8U,stretch,shift);
    I=temp;
}



InterferoPluginDialog::~InterferoPluginDialog()
{
    delete ui;
}

void InterferoPluginDialog::on_activateBox_stateChanged(int)
{
    extractData();
}

void InterferoPluginDialog::on_newReferenceButton_clicked()
{
    newReference=true;
    extractData();
}



void InterferoPluginDialog::on_subsampleSlider_sliderPressed()
{
    ssSliderPressed=true;
}

void InterferoPluginDialog::on_subsampleSlider_sliderReleased()
{
    ssSliderPressed=false;
    extractData();
}

void InterferoPluginDialog::on_subsampleSlider_valueChanged(int)
{
    if (!ssSliderPressed) {
        extractData();
    }
}

void InterferoPluginDialog::on_skipSlider_sliderPressed()
{
    skSliderPressed=true;
}

void InterferoPluginDialog::on_skipSlider_sliderReleased()
{
    skSliderPressed=false;
    extractData();
}

void InterferoPluginDialog::on_skipSlider_valueChanged(int)
{
    if (!skSliderPressed) {
        extractData();
    }
}

