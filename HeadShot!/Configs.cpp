#include "Configs.h"

#include <charconv>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <sstream>


Pixel HexToRgb(const std::string& hex) {
    if(hex.size() != 7 || hex[0] != '#') {
        throw std::invalid_argument("Invalid hex color format");
    }

    Pixel color;
    std::from_chars(hex.data() + 1, hex.data() + 3, color.Red, 16);
    std::from_chars(hex.data() + 3, hex.data() + 5, color.Green, 16);
    std::from_chars(hex.data() + 5, hex.data() + 7, color.Blue, 16);

    return color;
}

Configuration ParseCommandLineArgs(const std::string& args)
{
    std::istringstream iss(args);
    std::unordered_map<std::string, std::string> argMap;
    std::string key, value;

    while(iss >> key >> value) {
        argMap[key] = value;
    }

    Configuration config;
    config.ShowConsole = argMap["-console"] == "1";
    config.DetectibleColor.Lower = HexToRgb(argMap["-colorl"]);
    config.DetectibleColor.Higher = HexToRgb(argMap["-colorr"]);
    config.ViewportParameters.X = std::stoul(argMap["-wpx"]);
    config.ViewportParameters.Y = std::stoul(argMap["-wpy"]);
    config.ViewportParameters.Width = std::stoul(argMap["-wpw"]);
    config.ViewportParameters.Height = std::stoul(argMap["-wph"]);

    return config;
}
