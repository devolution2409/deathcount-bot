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

Matcher& Matcher::SetMethod(Matcher::Method method){
    this->method = method;
    return *this;

}



void Matcher::Process(){
    //ensure we are not null pointer
    if ( _template == nullptr || image == nullptr){
        return;
    }
    //else we compare

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