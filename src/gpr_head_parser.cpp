#include "gpr_head_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

GprMetadata GprHeadParser::parse(const std::filesystem::path& headPath) {
    if (!std::filesystem::exists(headPath)) {
        throw std::runtime_error("Head file not found: " + headPath.string());
    }

    std::ifstream file(headPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open head file: " + headPath.string());
    }

    std::unordered_map<std::string, std::string> kvMap;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        std::string key, value;
        if (parseLine(line, key, value)) {
            kvMap[key] = value;
        }
    }

    GprMetadata meta;
    populateMetadata(kvMap, meta);
    return meta;
}

bool GprHeadParser::parseLine(const std::string& line, std::string& key, std::string& value) {
    // Skip empty lines and full comment lines (# at start)
    std::string trimmed = line;
    size_t firstNonSpace = trimmed.find_first_not_of(" \t");
    if (firstNonSpace == std::string::npos || trimmed[firstNonSpace] == '#') {
        return false;
    }

    // Find first ": " delimiter
    size_t colonPos = trimmed.find(": ");
    if (colonPos == std::string::npos) {
        return false;
    }

    key = trimmed.substr(0, colonPos);
    value = trimmed.substr(colonPos + 2);

    // Strip inline comment (# and everything after)
    size_t commentPos = value.find('#');
    if (commentPos != std::string::npos) {
        value = value.substr(0, commentPos);
    }

    // Trim trailing whitespace from value
    size_t lastNonSpace = value.find_last_not_of(" \t");
    if (lastNonSpace != std::string::npos) {
        value = value.substr(0, lastNonSpace + 1);
    } else {
        value.clear();
    }

    return !key.empty();
}

void GprHeadParser::populateMetadata(
    const std::unordered_map<std::string, std::string>& kvMap,
    GprMetadata& meta) {

    auto getInt = [&](const std::string& k) -> int {
        auto it = kvMap.find(k);
        return it != kvMap.end() ? std::stoi(it->second) : 0;
    };
    auto getDouble = [&](const std::string& k) -> double {
        auto it = kvMap.find(k);
        return it != kvMap.end() ? std::stod(it->second) : 0.0;
    };
    auto getStr = [&](const std::string& k) -> std::string {
        auto it = kvMap.find(k);
        return it != kvMap.end() ? it->second : "";
    };

    meta.headerVersion        = getInt("HEADER VERSION");
    meta.dataVersion          = getInt("DATA VERSION");
    meta.date                 = getStr("DATE");
    meta.startTime            = getStr("START TIME");
    meta.stopTime             = getStr("STOP TIME");
    meta.antenna              = getStr("ANTENNA");
    meta.antennaSeparation    = getDouble("ANTENNA SEPARATION");
    meta.samples              = getInt("SAMPLES");
    meta.signalPosition       = getInt("SIGNAL POSITION");
    meta.clippedSamples       = getInt("CLIPPED SAMPLES");
    meta.runs                 = getInt("RUNS");
    meta.maxStacks            = getInt("MAX STACKS");
    meta.autostacks           = getInt("AUTOSTACKS");
    meta.frequency            = getDouble("FREQUENCY");
    meta.timeWindow           = getDouble("TIMEWINDOW");
    meta.lastTrace            = getInt("LAST TRACE");
    meta.trigSource           = getStr("TRIG SOURCE");
    meta.timeInterval         = getDouble("TIME INTERVAL");
    meta.distanceInterval     = getDouble("DISTANCE INTERVAL");
    meta.userDistanceInterval = getDouble("USER DISTANCE INTERVAL");
    meta.stopPosition         = getDouble("STOP POSITION");
    meta.wheelName            = getStr("WHEEL NAME");
    meta.wheelCalibration     = getDouble("WHEEL CALIBRATION");
    meta.zeroLevel            = getInt("ZERO LEVEL");
    meta.soilVelocity         = getDouble("SOIL VELOCITY");
    meta.preprocessing        = getStr("PREPROCESSING");
    meta.operatorComment      = getStr("OPERATOR COMMENT");
    meta.antennaFW            = getStr("ANTENNA F/W");
    meta.antennaHW            = getStr("ANTENNA H/W");
    meta.antennaFPGA          = getStr("ANTENNA FPGA");
    meta.antennaSerial        = getStr("ANTENNA SERIAL");
    meta.antennaName          = getStr("ANTENNA NAME");
    meta.softwareVersion      = getStr("SOFTWARE VERSION");
    meta.positioning          = getInt("POSITIONING");
    meta.channels             = getInt("CHANNELS");
    meta.channelConfiguration = getStr("CHANNEL CONFIGURATION");
    meta.chXOffset            = getDouble("CH_X_OFFSET");
    meta.chYOffset            = getDouble("CH_Y_OFFSET");
    meta.measurementDirection = getInt("MEASUREMENT DIRECTION");
}
