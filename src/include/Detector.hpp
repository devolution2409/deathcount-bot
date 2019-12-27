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
};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);