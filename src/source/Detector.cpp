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
                constexpr int fps = 15;
                // 30 fps
                if (static_cast<int>(cap.get(1)) == 1 ||
                    static_cast<int>(cap.get(1)) % fps == 0) {
                    // std::cout << "capget" << cap.get(1);

                    std::string name = "temp_" + this->streamer + "_" +
                                       std::to_string(cap.get(1)) + ".png";

                    cv::imwrite(name, image);

                    // api->SetImage(image.data, image.cols, image.rows, 4, 4*image.cols);
                    Pix* image2 = pixRead(name.c_str());

                    api->SetImage(image2);

                    std::string text(api->GetUTF8Text());
                    std::cout << "frame: " << std::to_string(cap.get(1)) << std::endl
                              << text << std::endl;
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

                            std::cout << "Deth at frame:" << cap.get(1)
                                      << " which evaluates to " << cap.get(1) * fps
                                      << " seconds after recording started.";
                            break;
                        }
                    }
                    // Destroy used object and release memory
                    image.release();
                    pixDestroy(&image2);
                    // TODO: replace remove with std::filesystem
                    // remove(name.c_str());
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