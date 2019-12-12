#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>


class Matcher{
    enum MatcherMethod : int{
        CV_TM_SQDIFF = 0,
        CV_TM_SQDIFF_NORMED,
        CV_TM_CCORR,
        CV_TM_CCORR_NORMED,
        CV_TM_CCOEFF,
        CV_TM_CCOEFF_NORMED

};
    public:
        Matcher();
        
        Matcher& SetImgA(cv::Mat other);
        Matcher& SetImgB(cv::Mat other);

        ~Matcher();
    private:
        cv::Mat imgA;
        cv::Mat imgB;



};
