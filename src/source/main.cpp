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
#include <tuple>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "EastDetector.hpp"

/* int main()
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix *image = pixRead("/source/tests/image3.jpg");
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
 */

int main( int argc, char** argv )
{
    std::string streamUrl;
    if (argc < 2 )
    {
        return 1;
    }
    else
    { 
        std::cout << "setting stream URL";
        streamUrl = argv[1];   
        
    }
    
    
    //get the url i guess

    
    char *outText;


    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) { 
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    //Pix *image = pixRead("/source/tests/image3.jpg");
    
    
    cv::VideoCapture cap;
    cap.open(streamUrl.c_str());
    cv::Mat image;

    while(1){


    if(!cap.read(image)) {
            std::cout << "No frame" << std::endl;
    }else
    {
        api->SetImage(image.data, image.cols, image.rows, 4, 4*image.cols);
     //   api->SetImage(image);
       // cv::imwrite("test2.png",image);
    }
    

    // SetRectangle(left, top, width, height) 
   // api->SetRectangle(200, 250, 600, 150);
    // Get OCR result
    outText = api->GetUTF8Text();
  

    std::string test = outText;

    if (test.find("YOU DIED") != std::string::npos|| test.find("YOUDIED")  != std::string::npos){
        std::cout << "LOL U DIED; LOL LOL U DIED;";
    }

    // Destroy used object and release memory
  
    delete [] outText;
    //image.release();
    //pixDestroy(&image);

    }

    api->End();

} 
