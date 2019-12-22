#pragma once
#include <string>

class EastDetector{

  private:

    std::string modelPath; // Path to a binary .pb file contains trained network.
    std::string imagePath; // Path to input image or video file. Skip this argument to capture frames from a camera
    int inpWidth; // Preprocess input image by resizing to a specific width. It should be multiple by 32.
    int inpHeight; // Preprocess input image by resizing to a specific height. It should be multiple by 32.
    float confThreshold; //Confidence threshold.
    float nmsThreshold; //Non-maximum suppression threshold. 


  public:

    EastDetector();
    ~EastDetector();
    EastDetector& SetModelPath(std::string & path);
    EastDetector& SetImagePath(std::string & path);
    EastDetector& SetPreProcessWidth(int width);
    EastDetector& SetPreProcessHeight(int height);
    EastDetector& SetConfidenceThreshold(float threshold);
    EastDetector& SetNonMaximumSuppresionThreshold(float threshold);



};