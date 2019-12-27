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
    const std::string red("\033[0;31m");
    const std::string green("\033[1;32m");
    const std::string yellow("\033[1;33m");
    const std::string cyan("\033[0;36m");
    const std::string magenta("\033[0;35m");
    const std::string reset("\033[0m");
    std::string streamUrl;
    if (argc < 2) {
        return 1;
    }
    else {
        std::cout << "setting stream URL";
        streamUrl = argv[1];
    }

    Detector detector;
    detector.SetStreamer(streamUrl).Work();

    return 0;

    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    // Pix *image = pixRead("/source/tests/image3.jpg");

    cv::VideoCapture cap;
    cap.open(streamUrl.c_str());
    cv::Mat image;
    // if thisn't a frame modulo 30 we fuck off

    while (1) {
        if (!cap.read(image)) {
            std::cout << "No frame" << std::endl;
        }
        else {
            // 30 fps
            if (static_cast<int>(cap.get(1)) == 1 || static_cast<int>(cap.get(1)) % 30 == 0) {
                std::cout << "capget" << cap.get(1);

                std::string name = "temp" + std::to_string(cap.get(1)) + ".png";

                cv::imwrite(name, image);

                // api->SetImage(image.data, image.cols, image.rows, 4, 4*image.cols);
                Pix* image2 = pixRead(name.c_str());

                api->SetImage(image2);

                std::string test(api->GetUTF8Text());
                std::cout << "test" << test;

                if (test.find("YOU DIED") != std::string::npos ||
                    test.find("YOUDIED") != std::string::npos) {
                    std::cout << "LOL U DIED; LOL LOL U DIED;";
                }
                else {
                    std::cout << red << test << std::endl;
                }

                // Destroy used object and release memory

                // delete [] outText;
                image.release();
                pixDestroy(&image2);
                remove(name.c_str());
                /*             try {
                                fs::path path(name);
                                if (fs::remove(path))
                                    std::cout << "file " << name << " deleted.\n";
                                else
                                std::cout << "file " << name << " not found.\n";
                            }
                            catch(const fs::filesystem_error& err) {
                                std::cout << "filesystem error: " << err.what() << '\n';
                            }
                 */
            }

            // SetRectangle(left, top, width, height)
            // api->SetRectangle(200, 250, 600, 150);
            // Get OCR result
            /*

               */
            // image.release();
        }
    }

    api->End();
}
