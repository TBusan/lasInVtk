#include "gpr_data_parser.h"
#include <fstream>
#include <stdexcept>
#include <cstdint>

std::vector<GprTrace> GprDataParser::parse(
    const std::filesystem::path& dataPath,
    const GprMetadata& meta,
    Endianness endian) {

    if (!std::filesystem::exists(dataPath)) {
        throw std::runtime_error("Data file not found: " + dataPath.string());
    }

    auto fileSize = std::filesystem::file_size(dataPath);
    auto expectedSize = meta.expectedDataSize();
    if (fileSize != expectedSize) {
        throw std::runtime_error(
            "Data file size mismatch: expected " +
            std::to_string(expectedSize) + " bytes, got " +
            std::to_string(fileSize) + " bytes");
    }

    if (meta.dataVersion != 16 && meta.dataVersion != 32) {
        throw std::runtime_error(
            "Unsupported DATA VERSION: " + std::to_string(meta.dataVersion));
    }

    // Read entire file into buffer
    std::ifstream file(dataPath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open data file: " + dataPath.string());
    }

    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), static_cast<std::ptrdiff_t>(fileSize));
    if (!file) {
        throw std::runtime_error("Failed to read data file: " + dataPath.string());
    }

    const int bytesPerSample = meta.bytesPerSample();
    const int numTraces = meta.lastTrace;
    const int numSamples = meta.samples;

    std::vector<GprTrace> traces(numTraces);
    size_t offset = 0;

    for (int t = 0; t < numTraces; ++t) {
        traces[t].samples.resize(numSamples);
        for (int s = 0; s < numSamples; ++s) {
            const char* ptr = buffer.data() + offset;
            if (meta.dataVersion == 16) {
                int16_t val = (endian == Endianness::Little)
                    ? readInt16LE(ptr) : readInt16BE(ptr);
                traces[t].samples[s] = static_cast<float>(val);
            } else {
                int32_t val = (endian == Endianness::Little)
                    ? readInt32LE(ptr) : readInt32BE(ptr);
                traces[t].samples[s] = static_cast<float>(val);
            }
            offset += bytesPerSample;
        }
    }

    return traces;
}

int16_t GprDataParser::readInt16LE(const char* buf) {
    auto b0 = static_cast<uint16_t>(static_cast<uint8_t>(buf[0]));
    auto b1 = static_cast<uint16_t>(static_cast<uint8_t>(buf[1]));
    return static_cast<int16_t>(b0 | (b1 << 8));
}

int16_t GprDataParser::readInt16BE(const char* buf) {
    auto b0 = static_cast<uint16_t>(static_cast<uint8_t>(buf[0]));
    auto b1 = static_cast<uint16_t>(static_cast<uint8_t>(buf[1]));
    return static_cast<int16_t>((b0 << 8) | b1);
}

int32_t GprDataParser::readInt32LE(const char* buf) {
    auto b0 = static_cast<uint32_t>(static_cast<uint8_t>(buf[0]));
    auto b1 = static_cast<uint32_t>(static_cast<uint8_t>(buf[1]));
    auto b2 = static_cast<uint32_t>(static_cast<uint8_t>(buf[2]));
    auto b3 = static_cast<uint32_t>(static_cast<uint8_t>(buf[3]));
    return static_cast<int32_t>(b0 | (b1 << 8) | (b2 << 16) | (b3 << 24));
}

int32_t GprDataParser::readInt32BE(const char* buf) {
    auto b0 = static_cast<uint32_t>(static_cast<uint8_t>(buf[0]));
    auto b1 = static_cast<uint32_t>(static_cast<uint8_t>(buf[1]));
    auto b2 = static_cast<uint32_t>(static_cast<uint8_t>(buf[2]));
    auto b3 = static_cast<uint32_t>(static_cast<uint8_t>(buf[3]));
    return static_cast<int32_t>((b0 << 24) | (b1 << 16) | (b2 << 8) | b3);
}
