#include "marangonitracking.h"

using namespace cv;

MarangoniTracking::MarangoniTracking(int thresh,int nrParticles) : threshold(thresh),nrParts(nrParticles){

}

Point MarangoniTracking::FindParticle(Mat image) {
    return Point(200,300);
}

