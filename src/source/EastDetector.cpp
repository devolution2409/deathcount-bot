#include "EastDetector.hpp"

//TODO: do we need a factory
EastDetector::EastDetector(){
    //set default values
    std::cout << "hello";
    this->inpWidth  = 32;
    this->inpHeight = 32;
    this->confThreshold = 0.5;
    this->nmsThreshold = 0.4;
    this->modelPath = "/source/model/frozen_east_text_detection.pb";
}
EastDetector::~EastDetector(){


}

EastDetector& EastDetector::SetModelPath(std::string path){
    this->modelPath = path;
    return *this;
}

EastDetector& EastDetector::SetImagePath(std::string path){
    this->imagePath = path;
    return *this;
}

EastDetector& EastDetector::SetPreProcessWidth(int width){
    this->inpWidth = width;
    return *this;
}

EastDetector& EastDetector::SetPreProcessHeight(int height){
    this->inpHeight = height;
    return *this;
}
EastDetector& EastDetector::SetConfidenceThreshold(float threshold){
    this->confThreshold = threshold;
    return *this;
}
EastDetector& EastDetector::SetNonMaximumSuppresionThreshold(float threshold){
    this->nmsThreshold = threshold;
    return *this;
}


void EastDetector::Decode(const cv::Mat& scores, const cv::Mat& geometry, float scoreThresh,
            std::vector<RotatedRect>& detections, std::vector<float>& confidences)
{

    using cv::Point2f;
    using cv::Size2f;
    using cv::RotatedRect;
 detections.clear();
    CV_Assert(scores.dims == 4); CV_Assert(geometry.dims == 4); CV_Assert(scores.size[0] == 1);
    CV_Assert(geometry.size[0] == 1); CV_Assert(scores.size[1] == 1); CV_Assert(geometry.size[1] == 5);
    CV_Assert(scores.size[2] == geometry.size[2]); CV_Assert(scores.size[3] == geometry.size[3]);

    const int height = scores.size[2];
    const int width = scores.size[3];
    for (int y = 0; y < height; ++y)
    {
        const float* scoresData = scores.ptr<float>(0, 0, y);
        const float* x0_data = geometry.ptr<float>(0, 0, y);
        const float* x1_data = geometry.ptr<float>(0, 1, y);
        const float* x2_data = geometry.ptr<float>(0, 2, y);
        const float* x3_data = geometry.ptr<float>(0, 3, y);
        const float* anglesData = geometry.ptr<float>(0, 4, y);
        for (int x = 0; x < width; ++x)
        {
            float score = scoresData[x];
            if (score < scoreThresh)
                continue;

            // Decode a prediction.
            // Multiple by 4 because feature maps are 4 time less than input image.
            float offsetX = x * 4.0f, offsetY = y * 4.0f;
            float angle = anglesData[x];
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            float h = x0_data[x] + x2_data[x];
            float w = x1_data[x] + x3_data[x];

            Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x],
                           offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
            Point2f p1 = Point2f(-sinA * h, -cosA * h) + offset;
            Point2f p3 = Point2f(-cosA * w, sinA * w) + offset;
            RotatedRect r(0.5f * (p1 + p3), Size2f(w, h), -angle * 180.0f / (float)CV_PI);
            detections.push_back(r);
            confidences.push_back(score);
        }
    }
}

void EastDetector::Detect(){
   // parser.about("Use this script to run TensorFlow implementation (https://github.com/argman/EAST) of "
         //         "EAST: An Efficient and Accurate Scene Text Detector (https://arxiv.org/abs/1704.03155v2)");

    
    std::cout << "hallo zuuluul";
    using namespace cv::dnn; //neural network
    using cv::Point;
    using cv::Mat;
    //TODO: replace by stdstring
    using cv::String;
    using cv::Scalar;
    using cv::HersheyFonts;
    using cv::Point2f;
    

    cv::String model = this->modelPath;
    CV_Assert(!model.empty());

    // Load network.
    Net net = readNet(model);
    
    std::vector<Mat> outs;
    std::vector<String> outNames(2);
    outNames[0] = "feature_fusion/Conv_7/Sigmoid";
    outNames[1] = "feature_fusion/concat_3";


    cv::Mat frame, blob;
    std::cout << this->inpWidth << " " << this->inpHeight;

    //TODO: can we avoid usuing VideoCapture anyway
    //cap >> frame;
    frame = cv::imread(this->imagePath);

    blobFromImage(frame, blob, 1.0, cv::Size(this->inpWidth, this->inpHeight), Scalar(123.68, 116.78, 103.94), true, false);
    
    net.setInput(blob);
    net.forward(outs, outNames);

    cv::Mat scores = outs[0];

    
    cv::Mat geometry = outs[1];

    // Decode predicted bounding boxes.
    std::vector<RotatedRect> boxes;
    std::vector<float> confidences;
     std::cout << "hallo";
    this->Decode(scores, geometry, this->confThreshold, boxes, confidences);
     // Apply non-maximum suppression procedure.
    for (auto i: confidences){
        std::cout << i;
    }

    std::vector<int> indices;
    std::cout << "hallo";
    cv::dnn::NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);
     std::cout << "here" << indices.size();
     // Render detections.
    Point2f ratio((float)frame.cols / this->inpWidth, (float)frame.rows / this->inpHeight);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        std::cout << "gneu";
        RotatedRect& box = boxes[indices[i]];
        Point2f vertices[4];
        box.points(vertices);
        for (int j = 0; j < 4; ++j)
        {
            vertices[j].x *= ratio.x;
            vertices[j].y *= ratio.y;
        }
        for (int j = 0; j < 4; ++j)
            line(frame, vertices[j], vertices[(j + 1) % 4], Scalar(0, 255, 0), 1);
    }

    // Put efficiency information.
    std::vector<double> layersTimes;
    double freq = cv::getTickFrequency() / 1000;
    double t = net.getPerfProfile(layersTimes) / freq;
    std::string label = cv::format("Inference time: %.2f ms", t);
    cv::putText(frame, label, Point(0, 15), HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
    cv::imwrite("test.png",frame);

    
}