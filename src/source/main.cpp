#include <cstdio>
#include <opencv2/opencv.hpp>
/* 
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h> */
using namespace cv;
#include <string>
#include "Matcher.hpp"
#include <iostream>

/** @function main */
int main( int argc, char** argv )
{
    Matcher matcher;
    
  /// Load image and template
 // img = imread( argv[1], 1 );
  //templ = imread( argv[2], 1 );
    for (int i = 0; i < argc; ++i){
        std::cout << "lul:" << argv[i] << std::endl;
    }
    
    matcher.SetImage(std::string(argv[1]))
            .SetTemplate(argv[2])
            .SetMethod(Matcher::Method::CV_TM_CCOEFF_NORMED)
        ;

    matcher.Process(); 
 
  return 0;
}