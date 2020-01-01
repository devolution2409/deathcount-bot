#include "Detector.hpp"

Detector::Detector() : dethCount(0), type(Detector::Type::UNSET){};

Detector::~Detector(){};

Detector& Detector::SetStreamer(std::string streamer)
{
    // todo: right a btter thing? idk kev

    std::string search = "https://www.twitch.tv/";
    bool found = false;
    size_t pos;
    if (!found && (pos = streamer.find(search) != std::string::npos)) {
        this->streamer = streamer.erase(0, search.length());
        found = true;
    }
    search = "www.twitch.tv/";
    if (!found && (pos = streamer.find(search) != std::string::npos)) {
        this->streamer = streamer.erase(0, search.length());
        found = true;
    }
    search = "twitch.tv/";
    if (!found && (pos = streamer.find(search) != std::string::npos)) {
        this->streamer = streamer.erase(0, search.length());
        found = true;
    }
    if (!found)
        this->streamer = streamer;

    // once we have the streamer name we can call FetchStreamUrls
    // From the recording function

    return *this;
}
Detector& Detector::SetStreamer(char* streamer)
{
    this->SetStreamer(std::string(streamer));
    return *this;
}

bool Detector::FetchStreamUrls()
{
    // TODO: use regex to check url ? not sure kev
    // Get it using the API of pwn.sh since streamlink doesnt work
    std::string url = "https://pwn.sh/tools/streamapi.py?url=twitch.tv/" + this->streamer;
    std::cout << "url api:" << url << std::endl;
    CURL* curl;
    CURLcode res;

    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        /* telling curl to store the answer in the string instead of stdout */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return false;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);

        // TODO: refactor using schema for better maintainability

        rapidjson::Document document;
        //  std::cout << "here";
        // std::cout << response;
        document.Parse(response.c_str());
        // std::cout << response.c_str() << std::endl;

        /* Tons of assertion */
        assert(document.IsObject());
        assert(document.HasMember("success"));
        assert(document["success"].IsBool());
        // if success we can parse the URLs
        if (document["success"].GetBool()) {
            assert(document.HasMember("urls"));
            // static const char* kTypeNames[] = {
            // "Null", "False", "True", "Object", "Array", "String", "Number"};
            const char* qualities[] = {"1080p60", "720p60", "720p",
                                       "480p",    "360p",   "160p"};

            for (auto i : qualities) {
                if (document["urls"].HasMember(i)) {
                    std::cout << "Highest quality found:" << i << std::endl;
                    this->streamUrl = document["urls"][i].GetString();
                    this->quality = i;
                    return true;
                    // break;
                }
            }
        }
        else {
            return false; // no stream found
        }
    }
    return false;
}
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int Detector::Work()
{

    this->FetchStreamUrls();
    // TODO: very later, implement LUA

    // If detector is still unset, it means the config is fucked up, or wasn't found
    if (this->type == Detector::Type::UNSET) {
        return 4; // no valid configuration for this game
    }

    if (this->streamer.empty()) {
        return 3; // no valid streamer provided at this point
    }
    // If we can't find anything we return
    if (!this->FetchStreamUrls()) {
        return 1; // no valid stream URL
    }

    if (this->type == Detector::Type::OCR) {
        // now this->streamUrl is set. Time to initialize Tesseract
        tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();

        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (api->Init(NULL, "eng")) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            return 2; // tesseract failed to init.
        }

        // Creating video capture object and opening the stream
        cv::VideoCapture cap;
        cap.open(streamUrl.c_str());
        cv::Mat image;

        while (1) {

            if (!cap.read(image)) {
                std::cout << "No frame" << std::endl;
            }
            else {
                constexpr int fps = 30;
                // 30 fps
                if (static_cast<int>(cap.get(1)) == 1 ||
                    static_cast<int>(cap.get(1)) % fps == 0) {
                    // std::cout << "capget" << cap.get(1);

                    std::string name = "./debug/temp_" + this->streamer + "_" +
                                       std::to_string(cap.get(1)) + ".png";

                    // Preprocess the image.
                    // TODO: add a block for those parameters in the json
                    // cv::threshold(image, image, 90, 255, cv::THRESH_BINARY);
                    cv::medianBlur(image, image, 3);
                    // cv::imwrite(name, image);

                    // TODO: find a way to wait until image finishing 'downloading' into ram, or written to disk

                    cv::Mat temp = this->PreProcessImage(image);
                    Pix* image2 = Utils::Mat::Mat8ToPix(&temp);
                    // Pix* image2 = pixRead(name.c_str());

                    api->SetImage(image2);

                    std::string text(api->GetUTF8Text());

                    std::cout << "frame: " << std::to_string(cap.get(1)) << std::endl
                              << text << std::endl;

                    // cv::putText(temp, text, cv::Point(100, 100), cv::FONT_HERSHEY_SIMPLEX,
                    //             1, cv::Scalar(255, 255, 255), 1, 8);
                    cv::imwrite(name, temp);

                    // Do the matching here.
                    // Match phrases are in the params vector
                    for (auto it : this->params) {
                        // TODO: do we need to "normalize the string?"
                        // TODO: any combination of the string with or without
                        // their space 2^n complexity feelsdankman
                        // std::cout << std::endl
                        //   << "Trying to find " << it << " in string"
                        //   << std::endl;
                        if (text.find(it) != std::string::npos) {
                            ++this->dethCount;

                            std::cout << "Deth at frame:" << cap.get(1) << " which evaluates to "
                                      << cap.get(1) / static_cast<double>(fps)
                                      << " seconds after recording started.";
                            break;
                        }
                        else {
                            // remove image only if not a death
                            // if (static_cast<int>(cap.get(1)) % 60 == 0)
                            //   remove(name.c_str());
                        }
                    }
                    // Destroy used object and release memory
                    image.release();
                    temp.release();
                    pixDestroy(&image2);
                    // TODO: replace remove with std::filesystem
                }
            }
        }

        api->End();
    }
    return 0; // done working
}

Detector& Detector::ReadConfig(std::string path, int gameID)
{
    // read the file
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string config = buffer.str();
    t.close();
    // create json object
    rapidjson::Document document;
    //  std::cout << "here";
    // std::cout << response;
    document.Parse(config.c_str());

    /* Tons of assertion */
    assert(document.IsObject());
    assert(document.HasMember("version"));
    assert(document.HasMember("config"));
    assert(document["config"].IsArray());
    // version 1
    if (document["version"].GetInt() == 1) {
        for (auto& i : document["config"].GetArray()) {
            // all games without IDs aren't valid so we skip them
            if (i.HasMember("ID")) {
                // account for int inside quotation mark in the json
                if ((i["ID"].IsString() && i["ID"].GetString() == std::to_string(gameID)) ||
                    ((i["ID"].IsInt() && i["ID"].GetInt() == gameID))) {

                    std::cout << "Found game configuration for game ID: " << gameID
                              << std::endl;
                    // assert we have what we need
                    assert(i.HasMember("method"));
                    assert(i["method"].HasMember("type"));
                    assert(i["method"].HasMember("params"));
                    assert(i["method"]["params"].IsArray());
                    // Fetch detector type
                    if (i["method"]["type"] == "OCR") {
                        this->type = Detector::Type::OCR;
                    }
                    else if (i["method"]["type"] == "LUA") {
                        this->type = Detector::Type::LUA;
                    }

                    // Fetch list of params
                    for (auto& k : i["method"]["params"].GetArray()) {
                        assert(k.IsString());
                        this->params.push_back(k.GetString());
                    }
                    assert(i.HasMember("deathCooldown"));
                    // account for quotes
                    this->deathCooldown = i["deathCooldown"].IsString()
                                              ? std::stoi(i["deathCooldown"].GetString())
                                              : i["deathCooldown"].GetInt();

                    break;
                }
            }
        }
    }
    return *this;
}

void Detector::OCRWorkOnce(std::string image)
{
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();

    cv::Mat img = cv::imread(image.c_str());
    cv::Mat new_image = cv::Mat::zeros(img.size(), img.type());

    // double threshold(InputArray src, OutputArray dst, double thresh, double maxval, int type);

    cv::threshold(img, new_image, 90, 255, cv::THRESH_BINARY);
    cv::medianBlur(new_image, new_image, 3);

    // Convert from BGR to HSV colorspace
    // cv::cvtColor(new_image, new_image, cv::COLOR_BGR2HSV);
    // somehow at this point YOU DIED stuff is in yellow instead of red

    /*    cv::Scalar lowerRed = Utils::ColorSpace::RGBtoHSV(255, 255, 0);
       cv::Scalar upperRed = Utils::ColorSpace::RGBtoHSV(255, 255, 255); */

    /*     cv::Mat frame_threshold;
        cv::inRange(new_image, lowerRed, upperRed, frame_threshold);

        cv::imwrite("framethreshold.png", frame_threshold);
        cv::imwrite("test.png", new_image); */

    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return; // tesseract failed to init.
    }
    Pix* image2 = pixRead("test.png");

    api->SetImage(image2);

    std::string text(api->GetUTF8Text());
    std::cout << text << std::endl;
    api->End();
    pixDestroy(&image2);
}

// TODO: write xamples with image along side the json to explain wtf is going on smileyW
cv::Mat Detector::PreProcessImage(const cv::Mat& image)
{
    // add parameter such as css margin, as in: margin top right left bot or smth

    // TODO: check for the preprocessing in the json
    // TODO: parameters as %? as pixels? idk kev feelsmegadankman

    int x = 0;                          // starting point x coordinates
    int y = 200;                        // starting point y coordinates
    int width = image.size().width - x; // get all "remaining image"
    int height = 300;
    // int height = image.size().height - y; // get all remaining image

    /* individual assertions to debug if needed */
    /*  assert(0 <= x);
     assert(0 <= width);
     assert((x + width) <= image.cols);
     assert(0 <= y);
     assert(0 <= height);
     assert((y + height) <= image.rows); */

    // Check for Region of Interest 'inside' image
    if (0 <= x && 0 <= width && (x + width) <= image.cols && 0 <= y &&
        0 <= height && (y + height) <= image.rows) {

        cv::Mat temp = image(cv::Rect(x, y, width, height));
        return temp;
    }
    else {
        // TODO: check which dimension is too large and supposed we wanted to crop until image is done
        return image;
    }
}