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
	
	float maxScale = 1, minScale = 0.2, stepNumber = 1 ,step = (maxScale - minScale) / stepNumber;
	double ratio;
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
		ratio = static_cast<double>(originalImg.size().width) / static_cast<double>(scaled.size().width);

		// compute scaled image size
		s = scaled.size();
		int sHeight = s.height, sWidth = s.width;
		// if scaled image is smoler than template we break
		if ( sHeight < tHeight || sWidth < tWidth ){
			std::cout << "forsenE ?"; 
			break;
		}
		
		this->image = scaled;
		

		// Applying the matching template method
		cv::Point result = this->SingleMatch(false);
		this->filename = std::to_string(i) + "_" + filename;
		//this->WriteImage();

		//data.push_back( std::make_tuple<double, cv::Point>(std::move(ratio),std::move(result)));
		data.push_back( std::make_tuple<double, cv::Point>(std::move(i),std::move(result)));



	}
	
	// heatmap_t* hm = heatmap_new(originalImg.size().width, originalImg.size().height);
	// get the area of the stuff and scale the points to match original image size
	// scale them
	std::for_each(data.begin(), data.end(),[&](std::tuple<double, cv::Point> p){

		std::vector<unsigned char> heatmap(originalImg.size().width*originalImg.size().height*4);
		heatmap_t* hm = heatmap_new(originalImg.size().width, originalImg.size().height);

		
		cv::Point matchLoc = std::get<cv::Point>(p);

	int lul = 0;
 		for (int x = matchLoc.x ; x < matchLoc.x + _template.cols; ++x){
			for (int y = matchLoc.y ; y < matchLoc.y + _template.rows; ++y){
				std::cout << std::endl << "LUL: " << lul << ": " <<  (matchLoc.x + x)  / std::get<double>(p)  << ":" <<  (matchLoc.y + y)/ std::get<double>(p);
			//	heatmap_add_point(hm,  static_cast<double>((matchLoc.x + x) / std::get<double>(p)) /2.0, static_cast<double>((matchLoc.y + y)/ std::get<double>(p)) /2.0);
		//		heatmap_add_point(hm,  static_cast<double>((matchLoc.x + x) / std::get<double>(p)), static_cast<double>((matchLoc.y + y)/ std::get<double>(p)));
		        heatmap_add_point(hm,  static_cast<double>(x) / std::get<double>(p), static_cast<double>((y)/ std::get<double>(p)) );
				lul++;	
			}
		}




		heatmap_add_point(hm,  static_cast<double>((matchLoc.x ))					/ std::get<double>(p) , (matchLoc.y)					/ std::get<double>(p));
     	heatmap_add_point(hm,  static_cast<double>((matchLoc.x)) 					/ std::get<double>(p) , (matchLoc.y + _template.rows)	/ std::get<double>(p));
     	heatmap_add_point(hm,  static_cast<double>((matchLoc.x + _template.cols)) 	/ std::get<double>(p) , (matchLoc.y )					/ std::get<double>(p));
     	heatmap_add_point(hm,  static_cast<double>((matchLoc.x + _template.cols)) 	/ std::get<double>(p) , (matchLoc.y + _template.rows)	/ std::get<double>(p));


		rectangle( this->image, matchLoc, cv::Point( matchLoc.x + _template.cols , matchLoc.y + _template.rows ), cv::Scalar(0,0,255), 2, 8, 0 );



		    heatmap_render_default_to(hm, &heatmap[0]);
			heatmap_free(hm); 
		if(unsigned error = lodepng::encode((std::to_string(std::get<double>(p)) + "_" + std::string("heatmap.png")).c_str() , heatmap, originalImg.size().width,originalImg.size().height)) {
    		    std::cerr << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
        return;
		this->WriteImage();
		heatmap.clear();
    }


	});


/*

 	std::vector<unsigned char> heatmap(originalImg.size().width*originalImg.size().height*4);
    heatmap_render_default_to(hm, &heatmap[0]);

    // Now that we've got a finished heatmap picture, we don't need the map anymore.
    heatmap_free(hm);

	//Mat m = Mat(originalImg.rows, originalImg.cols, CV_64FC4); // initialize matrix of uchar of 1-channel where you will store vec data
    //copy vector to mat
   // memcpy(m.data, heatmap.data(), heatmap.size()*sizeof(unsigned char)); 

	//cv::imwrite(this->filename.c_str(),m);
	//std::cout << "img dimension:" << originalImg.cols << "x" << originalImg.rows;


	if(unsigned error = lodepng::encode("heatmap.png", heatmap, originalImg.size().width,originalImg.size().height)) {
        std::cerr << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
        return;
    }
	//IMREAD_UNCHANGED == -1
	//https://github.com/opencv/opencv/blob/8c0b0714e76efef4a8ca2a7c410c60e55c5e9829/modules/imgcodecs/include/opencv2/imgcodecs.hpp#L64
	Mat _hm = cv::imread("heatmap.png");
	Mat lul;
	cv::addWeighted( _hm,0.5, originalImg, 0.5, 0.0, lul);
	cv::imwrite("test.png",lul);
 	
*/



/* 
	std::for_each(data.begin(), data.end(),[&](std::tuple<double, cv::Point> p){

		std::get<cv::Point>(p)/= std::get<double>(p);
		std::cout << "Ratio: " << std::get<double>(p) << " Matchloc:" << std::get<cv::Point>(p) << std::endl;


	}); */

	//	rectangle( image, matchLoc, Point( matchLoc.x + _template.cols , matchLoc.y + _template.rows ), Scalar(0,0,255), 2, 8, 0 );

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

Matcher& Matcher::WriteImage(std::string name){
	if (name.empty()){
		name = this->filename;
	}
	cv::imwrite(name,this->image);

}