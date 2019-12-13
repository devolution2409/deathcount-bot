#include "Matcher.hpp"



using cv::Mat;
Matcher::Matcher(){
    std::cout << "LUL I WAS INSTANCIED" << std::endl;
	this->filename = "output.png";
	// default values
	this->resizeMethod = Matcher::ResizeType::INTER_LINEAR;
	this->matchMethod = Matcher::MatchMethod::CV_TM_CCOEFF_NORMED;
}

Matcher::~Matcher(){
    std::cout << "I was destroyed" << std::endl;

}  

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

Matcher& Matcher::SetMatchMethod(Matcher::MatchMethod method){
    // std::cout << "Setting method to: " << method << std::endl;
    this->matchMethod = method;
    return *this;
}

Matcher& Matcher::SetResizeMethod(Matcher::ResizeType method){
    // std::cout << "Setting method to: " << method << std::endl;
    this->resizeMethod = method;
    return *this;
}

Matcher& Matcher::SetOutputFilename(std::string name){
	if (name.empty()){
		return *this;
	}
	// sanitize i guess?
	this->filename = name;
}

//std::tuple<double, cv::Point> 
cv::Point Matcher::SingleMatch(bool DrawBoundingBox){
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
    
    matchTemplate( this->image, this->_template, this->result, this->matchMethod );
    normalize( result, result, 0, 1, Matcher::NormTypes::NORM_MINMAX, -1, Mat() );
    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    // For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( this->matchMethod == Matcher::MatchMethod::CV_TM_SQDIFF ||this->matchMethod == Matcher::MatchMethod::CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }
   
   
   if (DrawBoundingBox){
   	rectangle( image, matchLoc, Point( matchLoc.x + _template.cols , matchLoc.y + _template.rows ), Scalar(0,0,255), 2, 8, 0 );
   }



  	//cv::imwrite(this->filename.c_str(),image);
	return matchLoc;
}

void Matcher::MultiScaleMatching(){
	//found; ?
	std::vector<std::tuple<double, cv::Point>> data;

	float maxScale = 1, minScale = 0.2, stepNumber = 20 ,step = (maxScale - minScale) / stepNumber;
	float ratio;
	std::string filename = this->filename;
	// we're going to need the dimension of the template
	cv::Size s = this->_template.size();
	int tHeight = s.height , tWidth = s.width;

	Mat scaled;

	// avoid shitloads of bounding boxes
	Mat originalImg = this->image;

	// scaling down image and applying match until image is smaller than template
	for (double i=maxScale; i > minScale; i-= step){
		
		std::cout << "i: " << i << std::endl;
		//To shrink an image, it will generally look best with CV_INTER_AREA interpolation, whereas to enlarge an image, it will generally look best with CV_INTER_CUBIC (slow) or CV_INTER_LINEAR (faster but still looks OK).

		cv::resize(originalImg, scaled, cv::Size(), i, i, Matcher::ResizeType::INTER_AREA);
		ratio = originalImg.size().width / scaled.size().width;

		// compute scaled image size
		s = scaled.size();
		int sHeight = s.height, sWidth = s.width;
		// if scaled image is smoler than template we break
		if ( sHeight < tHeight || sWidth < tWidth ){
			std::cout << "forsenE ?"; 
			break;
		}
		
		this->image = scaled;
		this->filename = std::to_string(i) + "_" + filename;

		// Applying the matching template method
		cv::Point result = this->SingleMatch(false);
		data.push_back(std::make_tuple<double,cv::Point>(std::move(i),std::move(result)));
	}




	/*
	*	Heatmap:
	*	Foreach scale/ratio:
	*      store the bounding box and the ratio
	*	   normalize bounding box
	*	   so i guess, make the function return tuple with all paramaters from result. So basically result.
	*	Foreach bounding box:
	*		all pixels in area are 1, every other is 0	
	*	Use https://github.com/lucasb-eyer/libheatmap to calculate heatmap
	*
	*
	*/

}
