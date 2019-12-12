#include "Matcher.hpp"



using cv::Mat;
Matcher::Matcher(){
    std::cout << "LUL I WAS INSTANCIED" << std::endl;
}

Matcher::~Matcher(){
    std::cout << "I was destroyed" << std::endl;

}  

/* Matcher& Matcher::SetTemplate(cv::Mat * other){
    if (_template != nullptr){
         delete this->_template;
    }
    this->_template = other;
    return *this;
}
Matcher& Matcher::SetImage(cv::Mat * other){
    if (image != nullptr){
        delete this->image;
    }
    this->image = other;
    return *this;
} */

Matcher& Matcher::SetTemplate(std::string other){
    _template = cv::imread(other.c_str());
    return *this;
}

Matcher& Matcher::SetImage(std::string other){
    image = cv::imread(other.c_str());
    return *this;
}

Matcher& Matcher::SetTemplate(char* other){
    _template = cv::imread(other);
    return *this;

}
Matcher& Matcher::SetImage(char* other){
    image = cv::imread(other);
    return *this;
}

Matcher& Matcher::SetMethod(Matcher::Method method){
    std::cout << "Setting method to: " << method << std::endl;

    this->method = method;
    return *this;

}



void Matcher::Process(){
    
    //cv::imwrite("templateProcess.jpg",_template);
    using cv::Point;
    using cv::Scalar;
    //ensure we are not null pointer
     
    //else we compare
  
    // Create the result matrix
    int result_cols = this->image.cols - this->_template.cols + 1;
    int result_rows = this->image.rows - this->_template.rows + 1;
    this->result.create( result_rows, result_cols, CV_32FC1 );


    // Do the Matching and Normalize
    //std::cout << "this->method before call: " << static_cast<int>(this->method)  <<" " << this->iMethod << std::endl;
  //  this->method= Matcher::Method::CV_TM_SQDIFF_NORMED;
    
    matchTemplate( this->image, this->_template, this->result, this->method );
    normalize( result, result, 0, 1, Matcher::NormTypes::NORM_MINMAX, -1, Mat() );
    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    // For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( this->method == Matcher::Method::CV_TM_SQDIFF ||this->method == Matcher::Method::CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }
  rectangle( image, matchLoc, Point( matchLoc.x + _template.cols , matchLoc.y + _template.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + _template.cols , matchLoc.y + _template.rows ), Scalar::all(0), 2, 8, 0 );

std::cout << "nyeeeh";
  cv::imwrite("result.png",result);
  cv::imwrite("image.png",image);

}
/*
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  matchTemplate( img, templ, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

  imshow( image_window, img_display );
  imshow( result_window, result );

  return;
}

*/
