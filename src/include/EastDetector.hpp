#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
using cv::RotatedRect;

#include <iostream>


class EastDetector{

  private:

    std::string modelPath; // Path to a binary .pb file contains trained network.
    std::string imagePath; // Path to input image or video file. Skip this argument to capture frames from a camera
    int inpWidth; // Preprocess input image by resizing to a specific width. It should be multiple by 32.
    int inpHeight; // Preprocess input image by resizing to a specific height. It should be multiple by 32.
    float confThreshold; //Confidence threshold.
    float nmsThreshold; //Non-maximum suppression threshold. 

    cv::Point2f vertices[4];

    void Decode(const cv::Mat& scores, const cv::Mat& geometry, float scoreThresh,
        std::vector<RotatedRect>& detections, std::vector<float>& confidences); 


  public:


    EastDetector();
    ~EastDetector();
    EastDetector& SetModelPath(std::string path);
    EastDetector& SetImagePath(std::string path);
    EastDetector& SetPreProcessWidth(int width);
    EastDetector& SetPreProcessHeight(int height);
    EastDetector& SetConfidenceThreshold(float threshold);
    EastDetector& SetNonMaximumSuppresionThreshold(float threshold);
    
    //call SetRectangle(left, top, width, height
    int GetOCRLeft();
    int GetOCRTop();
    int GetOCRWidth();
    int GetOCRHeight();

    void Detect(bool renderPicture = false);
    

};