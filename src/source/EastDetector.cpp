#include "EastDetector.hpp"

EastDetector::EastDetector(){

    
}
EastDetector::~EastDetector(){


}

EastDetector& EastDetector::SetModelPath(std::string & path){
    this->modelPath = path;
    return *this;
}

EastDetector& EastDetector::SetImagePath(std::string & path){
    this->imagePath = path;
    return *this;
}

EastDetector& EastDetector::SetPreProcessWidth(int width){
    this->inpWidth = width;
    return *this;
}

EastDetector& EastDetector::SetPreProcessHeight(int height){
    this->inpHeight = height;
    return *this;
}
EastDetector& EastDetector::SetConfidenceThreshold(float threshold){
    this->confThreshold = threshold;
    return *this;
}
EastDetector& EastDetector::SetNonMaximumSuppresionThreshold(float threshold){
    this->nmsThreshold = threshold;
    return *this;
}
