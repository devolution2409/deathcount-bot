#include <cstdio>
#include <opencv2/opencv.hpp>
/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h> */
using namespace cv;

#include <iostream>
#include <leptonica/allheaders.h>
#include <string>
#include <tesseract/baseapi.h>
#include <tuple>

//#include <filesystem>
// namespace fs = std::filesystem;
#include <cstdio>

#include "Detector.hpp"

#include <boost/program_options.hpp>

//#include <filesystem>

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

int main(int argc, char** argv)
{
    std::string streamUrl;
    if (argc < 2) {
        return 1;
    }
    boost::program_options::options_description desc("Allowed options");
    // The add_options method of that class returns a special proxy object that defines operator(). Calls to that operator actually declare options.
    desc.add_options()("help", "produce help message")(
        "streamer", boost::program_options::value<std::string>(),
        "Supply which streamer to monitor");

    // Detector detector;
    // detector.SetStreamer(streamUrl).Work();

    return 0;
}
