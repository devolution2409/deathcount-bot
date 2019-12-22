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

void EastDetector::Detect(){
    using namespace cv::dnn;
    float confThreshold = this->confThreshold;
    float nmsThreshold = this->nmsThreshold;
    int inpWidth = this->inpWidth;
    int inpHeight =  this->inpHeight;
    cv::String model = this->modelPath;

    CV_Assert(!model.empty());

    // Load network.
    Net net = readNet(model);

    cv::Mat frame, blob;
    // Open a video file or an image file or a camera stream.
    cv::VideoCapture cap;
    cap.open(this->imagePath);
    cap >> frame;

    std::vector<cv::Mat> outs;
    std::vector<cv::String> outNames(2);
    outNames[0] = "feature_fusion/Conv_7/Sigmoid";
    outNames[1] = "feature_fusion/concat_3";


    blobFromImage(frame, blob, 1.0, cv::Size(inpWidth, inpHeight), cv::Scalar(123.68, 116.78, 103.94), true, false);
    net.setInput(blob);
    net.forward(outs, outNames);
    for (cv::Mat i : outs){
        std::cout << i;
    }

    cv::Mat scores = outs[0];
    cv::Mat geometry = outs[1];
 
    // Decode predicted bounding boxes.
    std::vector<RotatedRect> boxes;
    std::vector<float> confidences;
    this->Decode(scores, geometry, confThreshold, boxes, confidences);
 
    // Apply non-maximum suppression procedure.
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    
    // Render detections.
    cv::Point2f ratio((float)frame.cols / inpWidth, (float)frame.rows / inpHeight);
    // in working east branch: ratio:[3.2, 2]root@394bba6087c4:/source/build $ 
    std::cout << "ratio:" << ratio;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        RotatedRect& box = boxes[indices[i]];
         cv::Point2f vertices[4];
        box.points(vertices);
        for (int j = 0; j < 4; ++j)
        {
            vertices[j].x *= ratio.x;
            vertices[j].y *= ratio.y;
        }
        for (int j = 0; j < 4; ++j)
            line(frame, vertices[j], vertices[(j + 1) % 4], cv::Scalar(0, 255, 0), 1);
    }
 
    // Put efficiency information.
    std::vector<double> layersTimes;
    double freq = cv::getTickFrequency() / 1000;
    double t = net.getPerfProfile(layersTimes) / freq;
    std::string label = cv::format("Inference time: %.2f ms", t);
    cv::putText(frame, label, cv::Point(0, 15), cv::HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
    cv::imwrite("test.png",frame);
    //imshow(kWinName, frame);
}


void EastDetector::Decode(const cv::Mat& scores, const cv::Mat& geometry, float scoreThresh,
            std::vector<RotatedRect>& detections, std::vector<float>& confidences)
{
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

            cv::Point2f offset(offsetX + cosA * x1_data[x] + sinA * x2_data[x],
                           offsetY - sinA * x1_data[x] + cosA * x2_data[x]);
            cv::Point2f p1 = cv::Point2f(-sinA * h, -cosA * h) + offset;
            cv::Point2f p3 = cv::Point2f(-cosA * w, sinA * w) + offset;
            RotatedRect r(0.5f * (p1 + p3), cv::Size2f(w, h), -angle * 180.0f / (float)CV_PI);
            detections.push_back(r);
            confidences.push_back(score);
            std::cout << "made it;";
        }
    }
}