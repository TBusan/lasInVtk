#pragma once

#include <string>
#include <vector>
#include <filesystem>

struct GprCorRecord {
    int         traceNumber;
    std::string date;
    std::string time;       // raw: HH:MM:SS:mmm
    double      northing;
    std::string latType;
    double      easting;
    std::string lonType;
    double      elevation;
    std::string elevUnit;
    int         status;
};

class GprCorParser {
public:
    std::vector<GprCorRecord> parse(const std::filesystem::path& corPath,
                                     int expectedTraceCount);
};
