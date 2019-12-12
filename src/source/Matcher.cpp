#include "Matcher.hpp"


using cv::Mat;
Matcher::Matcher(){
    std::cout << "LUL I WAS INSTANCIED";
}

Matcher::~Matcher(){
    delete this->_template;
    delete this->image;

}  

Matcher& Matcher::SetTemplate(cv::Mat * other){
    this->_template = other;
    return *this;
}
Matcher& Matcher::SetImage(cv::Mat * other){
    this->image = other;
    return *this;
}

Matcher& Matcher::SetTemplate(std::string other){
    *_template = cv::imread(other.c_str());
    return *this;
}

Matcher& Matcher::SetImage(std::string other){
    *image = cv::imread(other.c_str());
    return *this;
}

Matcher& Matcher::SetTemplate(char* other){
    *_template = cv::imread(other);
    return *this;

}
Matcher& Matcher::SetImage(char* other){
    *image = cv::imread(other);
    return *this;

}
