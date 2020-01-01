#pragma once

#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

namespace Utils {

namespace ColorSpace {
inline cv::Scalar HSVtoBGR(uchar H, uchar S, uchar V)
{
    cv::Mat rgb;
    cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(H, S, V));
    cv::cvtColor(hsv, rgb, cv::COLOR_HSV2BGR);
    return cv::Scalar(rgb.data[0], rgb.data[1], rgb.data[2]);
};
inline cv::Scalar BGRtoHSV(uchar B, uchar G, uchar R)
{
    cv::Mat rgb(1, 1, CV_8UC3, cv::Scalar(B, G, R));
    cv::Mat hsv;
    cv::cvtColor(rgb, hsv, cv::COLOR_RGB2HSV);
    return cv ::Scalar(hsv.data[0], hsv.data[1], hsv.data[2]);
};

inline cv::Scalar RGBtoHSV(uchar R, uchar G, uchar B)
{
    return Utils::ColorSpace::BGRtoHSV(B, G, R);
};

} // namespace ColorSpace

namespace Mat {
inline Pix* Mat8ToPix(cv::Mat* mat8)
{
    Pix* pixd = pixCreate(mat8->size().width, mat8->size().height, 8);
    for (int y = 0; y < mat8->rows; y++) {
        for (int x = 0; x < mat8->cols; x++) {
            pixSetPixel(pixd, x, y, (l_uint32)mat8->at<uchar>(y, x));
        }
    }
    return pixd;
};
inline cv::Mat Pix8ToMat(Pix* pix8)
{
    cv::Mat mat(cv::Size(pix8->w, pix8->h), CV_8UC1);
    uint32_t* line = pix8->data;
    for (uint32_t y = 0; y < pix8->h; ++y) {
        for (uint32_t x = 0; x < pix8->w; ++x) {
            mat.at<uchar>(y, x) = GET_DATA_BYTE(line, x);
        }
        line += pix8->wpl;
    }
    return mat;
};
} // namespace Mat

} // namespace Utils