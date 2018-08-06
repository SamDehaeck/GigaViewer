#include "ellipsedetection.h"


using namespace cv;

EllipseDetection::EllipseDetection(int thresh) : threshold(thresh),activated(false),shouldTrack(false){

}

void EllipseDetection::ChangeSettings(QMap<QString,QVariant> settings) {
    targetX=settings["targetX"].toInt();
    targetY=settings["targetY"].toInt();
    threshold=settings["threshold"].toInt();
    if ((!activated)&&(settings["activated"].toBool())) {
       // qDebug()<<"Should initialise";
    }
    if (activated&&(!settings["activated"].toBool())) {
       // qDebug()<<"Should write to disk";
    }

    activated=settings["activated"].toBool();
    shouldTrack=settings["shouldTrack"].toBool();
//    qDebug()<<"threshold="<<threshold;
}

bool EllipseDetection::processImage(ImagePacket& currIm) {
    if (activated) {


        if (currIm.pixFormat=="RGB8") {
            cv::Mat grayIm;
            cv::cvtColor(currIm.image,grayIm,CV_RGB2GRAY);
            currIm.image=grayIm.clone();
            currIm.pixFormat="MONO8";
        }
        if (currIm.pixFormat=="MONO8") {
            int newT=static_cast<int>(round(threshold*255/100.0));  //change here if it needs to work for 16-bit

            int apSize=3;
            // Find edge pixels
            Mat I=currIm.image.clone();
            Mat edges;
//            Mat sobx,soby,sobang,sobmag,edges;
//            Sobel( I, sobx,CV_32F,1, 0, apSize );
//            Sobel( I, soby,CV_32F, 0, 1, apSize );
//            cartToPolar(sobx,soby,sobmag,sobang);
            Canny( I, edges,0.5*newT,newT,apSize,true);

            Mat labs; // = Mat::zeros( edges.size(), CV_32S );  // should be able to store all contours in a different label => 8U not suff
            Mat stats;// = Mat::zeros( edges.size(), CV_32S );  // should be able to store all contours in a different label => 8U not suff
            Mat centres;
            int nrLabs=connectedComponentsWithStats(edges,labs,stats,centres);

            //            qDebug()<<"Amount of labels: "<<nrLabs;
            if (nrLabs>1) {
                // now do single loop over labs to create a vector of vector of points where each subvector contains the coordinates
                // of that label. Also make a separate vector to contain amount of edge points for each label.
                // Easier to eliminate too small or too large labels before full loop.
                int32_t* pixPointer;
                std::vector<std::vector<Point> > labCont(nrLabs);
                std::vector<int> counter(nrLabs);

                for (int i=0;i<labs.rows;i++) {
                    pixPointer=labs.ptr<int32_t>(i);
                    for (int j=0;j<labs.cols;j++) {
                        int32_t label=pixPointer[j];
                        if (label==0) continue;

                        labCont[label-1].push_back(Point(j,i));
                        if (labCont[label-1].size()==0) {
                            counter[label-1]=1;
                        } else {
                            counter[label-1]=counter[label-1]+1;
                        }

                    }
                }

//                vector<Mat> accum;
//                int solCounter=0;
//                int resColumns=0;
                int minContour= 20;
                int maxSize=max(I.rows,I.cols);
                float minDiam = targetX/100.0*maxSize;
                float maxDiam = targetY/100.0*maxSize;
                std::vector<RotatedRect> foundEllipses;
                for (int i=0;i<nrLabs;i++) {
                    if (counter[i]<=minContour) {
                        continue;
                    } else {
                        RotatedRect fEll=cv::fitEllipse(labCont[i]);
                        float aspRat=fEll.size.width/fEll.size.height;
                        float minAxis=min(fEll.size.width,fEll.size.height);
                        float maxAxis=max(fEll.size.width,fEll.size.height);
//                        qDebug()<<"Min and max are: "<<minAxis<<" - "<<maxAxis;
                        if (aspRat>0.8 && aspRat < 1.2) {
                            if (minAxis>minDiam && maxAxis<maxDiam) {
                                cv::ellipse(I,fEll,150,5);
                                foundEllipses.push_back(fEll);
                            }
                        }
                    }

                }

                // now calculate mean equivalent diameter
                double accumMean=0;
                for (uint i=0;i<foundEllipses.size();i++) {
                    accumMean=accumMean+sqrt(foundEllipses[i].size.width*foundEllipses[i].size.height);
                }
                double myMean=accumMean/foundEllipses.size();
                //qDebug()<<"Mean: "<<myMean;
                char str[200];
                sprintf(str,"D=%.2f",myMean);
                putText(I, str, Point2f(20,100), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));



                currIm.image=I;
            } else {
                currIm.image=edges;
            }



//            cv::threshold ( currIm.image, processed, newT, 255, THRESH_BINARY_INV );

//            std::vector<std::vector<cv::Point> > contours;
//            cv::Mat hierarchy;
//            cv::findContours( processed, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

//            cv::Mat outImage=currIm.image.clone(); // use a clone to avoid storing on raw image which is stored.
//            cv::drawContours(outImage,contours,-1,255,2);

//            cv::Point newLoc(targetX*currIm.image.cols/100.0,targetY*currIm.image.rows/100.0);
//            cv::ellipse(outImage,newLoc,cv::Size(5,5),0,0,360,150,-1);

//            currIm.image=outImage;

            // now make string to save
            //QString outst(""
//            outst=""+
//            currIm.timeStamp+" "+threshold+" "+targetX+""
//            dataToSave.append(outst);


        }
    }
    return true;
}

