#include "gpr_dataset.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

GprDataset GprDatasetLoader::load(const std::filesystem::path& basePath,
                                    Endianness endian) {
    GprDataset dataset;

    // Parse .head first — other parsers depend on it
    auto headPath = basePath;
    headPath += ".head";
    GprHeadParser headParser;
    dataset.metadata = headParser.parse(headPath);

    // Parse .cor
    auto corPath = basePath;
    corPath += ".cor";
    GprCorParser corParser;
    dataset.coordinates = corParser.parse(corPath, dataset.metadata.lastTrace);

    // Parse .data
    auto dataPath = basePath;
    dataPath += ".data";
    GprDataParser dataParser;
    dataset.traces = dataParser.parse(dataPath, dataset.metadata, endian);

    return dataset;
}

void GprDataset::printSummary() const {
    const auto& m = metadata;
    std::cout << "=== GPR Dataset Summary ===" << std::endl;
    std::cout << "Data Version (bits): " << m.dataVersion << std::endl;
    std::cout << "Samples per trace:   " << m.samples << std::endl;
    std::cout << "Total traces:        " << m.lastTrace << std::endl;
    std::cout << "Frequency:           " << m.frequency << " MHz" << std::endl;
    std::cout << "Time Window:         " << m.timeWindow << " ns" << std::endl;
    std::cout << "Distance Interval:   " << std::fixed << std::setprecision(6)
              << m.distanceInterval << " m" << std::endl;
    std::cout << "Stop Position:       " << std::fixed << std::setprecision(3)
              << m.stopPosition << " m" << std::endl;
    std::cout << "Soil Velocity:       " << m.soilVelocity << " mm/ns" << std::endl;
    std::cout << "Antenna:             " << m.antenna << std::endl;
    std::cout << "Channels:            " << m.channels << std::endl;
    std::cout << "Date:                " << m.date << std::endl;
    std::cout << "Start Time:          " << m.startTime << std::endl;
    std::cout << "Stop Time:           " << m.stopTime << std::endl;
    std::cout << "Expected data size:  " << m.expectedDataSize() << " bytes" << std::endl;
    std::cout << std::endl;

    std::cout << "--- Coordinates ---" << std::endl;
    std::cout << "Total records:       " << coordinates.size() << std::endl;
    if (!coordinates.empty()) {
        const auto& first = coordinates.front();
        const auto& last  = coordinates.back();
        std::cout << "First trace:         #" << first.traceNumber
                  << "  N=" << std::fixed << std::setprecision(7) << first.northing
                  << "  E=" << first.easting
                  << "  Elev=" << std::setprecision(2) << first.elevation << " m" << std::endl;
        std::cout << "Last  trace:         #" << last.traceNumber
                  << "  N=" << std::fixed << std::setprecision(7) << last.northing
                  << "  E=" << last.easting
                  << "  Elev=" << std::setprecision(2) << last.elevation << " m" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "--- Trace Data ---" << std::endl;
    std::cout << "Total traces parsed: " << traces.size() << std::endl;
    if (!traces.empty()) {
        std::cout << "First trace (first 10 samples): ";
        const auto& t = traces[0];
        int count = std::min<int>(10, static_cast<int>(t.samples.size()));
        for (int i = 0; i < count; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << static_cast<int>(t.samples[i]);
        }
        std::cout << std::endl;
    }
}
