#include <cstdio>
#include <opencv2/opencv.hpp>
/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h> */
using namespace cv;

#include <iostream>
#include <leptonica/allheaders.h>
#include <string>
#include <tesseract/baseapi.h>
#include <tuple>

//#include <filesystem>
// namespace fs = std::filesystem;
#include <cstdio>

#include "Detector.hpp"

#include <boost/program_options.hpp>
#include <fstream>

int main(int argc, char** argv)
{
    std::string streamUrl;

    boost::program_options::options_description desc("Usage");
    // The add_options method of that class returns a special proxy object that defines operator(). Calls to that operator actually declare options.
    // clang-format off

    std::string streamer, config;
    int gameID;

    desc.add_options()("help", "Show this message")
        ("streamer", boost::program_options::value<std::string>(&streamer),"Which streamer to monitor")
        ("force-game-id", boost::program_options::value<int>(&gameID),"Force a game configuration")
        ("config-file", boost::program_options::value<std::string>(&config)->default_value("DethcountBot.config.json"),"Path to a valid config.json file. Defaults to DethcountBot.config.json")
        ;
    // clang-format on
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store(
            boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return false;
    }
    catch (...) {
        std::cerr << "Unknown error!"
                  << "\n";
        return false;
    }

    // if help we show help
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    // for now, force-game-id is mandatory.
    // TODO: remove it when we get use twitch API to fetch gameID
    const char* mandatoryArgs[] = {"streamer", "force-game-id"};
    bool wrongUsage = false;
    for (auto i : mandatoryArgs) {
        if (!vm.count(i)) {
            std::cout << "Missing mandatory argument: --" << i << std::endl;
            wrongUsage = true;
        }
    }
    if (wrongUsage) {
        std::cout << desc << std::endl;
        return 2;
    }
    std::ifstream t(config);
    if (!t.good()) {
        std::cout << "Error reading: '" << config << "' file" << std::endl;
        return 3;
    }
    // at this point streamer, config and gameID shoudl be set.

    Detector detector;
    detector.SetStreamer(streamUrl).ReadConfig(config, gameID).Work();

    return 0;
}
