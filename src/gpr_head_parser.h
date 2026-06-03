#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <cstddef>

struct GprMetadata {
    int     headerVersion     = 0;
    int     dataVersion       = 0;      // bits per sample: 16 or 32
    std::string date;
    std::string startTime;
    std::string stopTime;
    std::string antenna;
    double  antennaSeparation = 0.0;
    int     samples           = 0;      // samples per trace
    int     signalPosition    = 0;
    int     clippedSamples    = 0;
    int     runs              = 0;
    int     maxStacks         = 0;
    int     autostacks        = 0;
    double  frequency         = 0.0;    // sampling frequency
    double  timeWindow        = 0.0;    // time window in ns
    int     lastTrace         = 0;      // total trace count
    std::string trigSource;
    double  timeInterval      = 0.0;
    double  distanceInterval  = 0.0;
    double  userDistanceInterval = 0.0;
    double  stopPosition      = 0.0;
    std::string wheelName;
    double  wheelCalibration  = 0.0;
    int     zeroLevel         = 0;
    double  soilVelocity      = 0.0;
    std::string preprocessing;
    std::string operatorComment;
    std::string antennaFW;
    std::string antennaHW;
    std::string antennaFPGA;
    std::string antennaSerial;
    std::string antennaName;
    std::string softwareVersion;
    int     positioning       = 0;
    int     channels          = 0;
    std::string channelConfiguration;
    double  chXOffset         = 0.0;
    double  chYOffset         = 0.0;
    int     measurementDirection = 0;

    int bytesPerSample() const { return dataVersion / 8; }
    std::size_t expectedDataSize() const {
        return static_cast<std::size_t>(lastTrace) * samples * bytesPerSample();
    }
};

class GprHeadParser {
public:
    GprMetadata parse(const std::filesystem::path& headPath);

private:
    bool parseLine(const std::string& line, std::string& key, std::string& value);
    void populateMetadata(const std::unordered_map<std::string, std::string>& kvMap,
                          GprMetadata& meta);
};
