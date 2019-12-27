#pragma once
#include "rapidjson/document.h"
#include <curl/curl.h>

#include <iostream>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <tesseract/baseapi.h>

#include <fstream>
#include <sstream>
#include <vector>
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

    /**
     * @brief Read the provided config file and search for gameID
     *
     * @param path path to a config.json file
     * @param gameID gameID
     * @return Detector&
     */
    Detector& ReadConfig(std::string path, int gameID);

    int Work();

  private:
    enum Type { UNKNOWN, OCR, LUA };

    // Stream vars
    std::string quality;
    std::string streamer;
    std::string streamUrl;

    // Detector vars
    Detector::Type type;
    std::vector<std::string> params; // array of words to match, or array of lua files
    int deathCooldown;

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