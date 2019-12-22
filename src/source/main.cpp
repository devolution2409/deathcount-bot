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

    /* if (false){
    Matcher matcher;
    
  
    for (int i = 0; i < argc; ++i){
        std::cout << "lul:" << argv[i] << std::endl;
    }
    
    matcher.SetImage(std::string(argv[1]))
            .SetTemplate(argv[2])
            .SetMatchMethod(Matcher::MatchMethod::CV_TM_CCORR_NORMED)
        ;

    //matcher.SingleMatch(); 
    matcher.MultiScaleMatching();

    } */

    EastDetector detector;

    detector.SetImagePath("/source/tests/image3.jpg")
    .Detect()
    ;

    


  return 0;
} 