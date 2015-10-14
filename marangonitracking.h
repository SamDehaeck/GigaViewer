#ifndef MARANGONITRACKING_H
#define MARANGONITRACKING_H

#include <opencv2/opencv.hpp>

class MarangoniTracking
{
private:
    int threshold;
    int nrParts;
public:
    MarangoniTracking(int thresh,int nrParticles);
    cv::Point FindParticle(cv::Mat image);

};

#endif // MARANGONITRACKING_H
