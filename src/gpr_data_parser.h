#pragma once

#include <vector>
#include <filesystem>
#include "gpr_head_parser.h"

struct GprTrace {
    std::vector<float> samples;
};

enum class Endianness { Little, Big };

class GprDataParser {
public:
    std::vector<GprTrace> parse(const std::filesystem::path& dataPath,
                                 const GprMetadata& meta,
                                 Endianness endian = Endianness::Little);

private:
    static int16_t readInt16LE(const char* buf);
    static int16_t readInt16BE(const char* buf);
    static int32_t readInt32LE(const char* buf);
    static int32_t readInt32BE(const char* buf);
};
