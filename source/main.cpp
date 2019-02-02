#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/core/core.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
	using namespace cv;

	if (argc < 2)
	{

		std::cout << Please specify a file !
			return 1;
	}	


	char *outText;
	//trying to HOG this shit
	Mat image;
	img = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE  );
	/*
	   img.convertTo(img, CV_32F, 1/255.0);

	// Calculate gradients gx, gy
	/*Mat gx, gy; 
	Sobel(img, gx, CV_32F, 1, 0, 1);
	Sobel(img, gy, CV_32F, 0, 1, 1);

	Mat mag, angle; 
	cartToPolar(gx, gy, mag, angle, 1); */
	Mat imgAdaptiveThreshold = new Mat();
	Imgproc.adaptiveThreshold(img, imgAdaptiveThreshold, 255, CV_ADAPTIVE_THRESH_MEAN_C ,CV_THRESH_BINARY, 99, 4);
	Imgcodecs.imwrite("./adaptive_threshold.png", imgAdaptiveThreshold);




	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	// Open input image with leptonica library
	Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
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
