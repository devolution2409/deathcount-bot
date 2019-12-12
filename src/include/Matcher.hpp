#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>


class Matcher{
    enum Method : int{
        CV_TM_SQDIFF        =0,
        CV_TM_SQDIFF_NORMED =1,
        CV_TM_CCORR         =2,
        CV_TM_CCORR_NORMED  =3,
        CV_TM_CCOEFF        =4,
        CV_TM_CCOEFF_NORMED =5

};
    public:
        Matcher();

        Matcher& SetTemplate(cv::Mat * other);
        Matcher& SetImage(cv::Mat * other);
        Matcher& SetTemplate(std::string other);
        Matcher& SetImage(std::string other);
        Matcher& SetTemplate(char* other);
        Matcher& SetImage(char* other);
        Matcher& SetMethod(Matcher::Method method);

        void Process();

        ~Matcher();
    private:
    /* https://github.com/opencv/opencv/blob/c2b6c6743175241d895fe4e67028fd960a405b00/modules/core/include/opencv2/core/mat.hpp#L791 */
        cv::Mat * _template;
        cv::Mat * image;
        cv::Mat * result;
        Matcher::Method method;

    

};
