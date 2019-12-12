#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace Matcher{
    enum MatcherMethod : int{
        CV_TM_SQDIFF = 1,
        CV_TM_SQDIFF_NORMED,
        CV_TM_CCORR,
        CV_TM_CCORR_NORMED,
        CV_TM_CCOEFF,
        CV_TM_CCOEFF_NORMED
}
}

class Matcher(){

    public:
        Matcher();
        
        Matcher& SetImgA(Mat other);
        Matcher& SetImgB(Mat other);

        ~Matcher();
    private:
        Mat imgA;
        Mat imgB;



};