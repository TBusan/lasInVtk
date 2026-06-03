#include "gpr_cor_parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<GprCorRecord> GprCorParser::parse(
    const std::filesystem::path& corPath,
    int expectedTraceCount) {

    if (!std::filesystem::exists(corPath)) {
        throw std::runtime_error("Cor file not found: " + corPath.string());
    }

    std::ifstream file(corPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open cor file: " + corPath.string());
    }

    std::vector<GprCorRecord> records;
    records.reserve(expectedTraceCount);

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) continue;

        std::istringstream iss(line);
        GprCorRecord rec;
        std::string northingStr, eastingStr, elevationStr, statusStr;

        if (!(iss >> rec.traceNumber >> rec.date >> rec.time
                  >> northingStr >> rec.latType
                  >> eastingStr >> rec.lonType
                  >> elevationStr >> rec.elevUnit
                  >> statusStr)) {
            continue;
        }

        rec.northing  = std::stod(northingStr);
        rec.easting   = std::stod(eastingStr);
        rec.elevation = std::stod(elevationStr);
        rec.status    = std::stoi(statusStr);

        records.push_back(std::move(rec));
    }

    if (static_cast<int>(records.size()) != expectedTraceCount) {
        throw std::runtime_error(
            "Cor file line count mismatch: expected " +
            std::to_string(expectedTraceCount) + ", got " +
            std::to_string(records.size()));
    }

    return records;
}
