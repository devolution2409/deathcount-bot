#pragma once
#include "rapidjson/document.h"
#include <curl/curl.h>
#include <iostream>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <tesseract/baseapi.h>

class Detector {
  public:
    Detector();
    ~Detector();

    /**
     * @brief
     *
     */
    void Process();

    Detector& SetStreamer(std::string streamer);
    Detector& SetStreamer(char*);

    int Work();

  private:
    enum Type { OCR, LUA };

    std::string quality;

    std::string streamer;

    std::string streamUrl;

    Detector::Type type;
    // array of words to match, or array of lua files
    std::vector<std::string> params;

    /**
     * @brief Fetch Streams Urls using pwn.sh API and libcurl.
     * Will set this->streamUrl to a valid URL on success.
     *
     * @return true if stream is found
     * @return false otherwise
     */
    bool FetchStreamUrls();

    int dethCount;
};
/**
 * @brief To be able to write cUrl output to a std::string
 * Use    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
 *        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // response being std::string
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return size_t
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);