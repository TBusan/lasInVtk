#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "gpr_head_parser.h"
#include "gpr_cor_parser.h"
#include "gpr_data_parser.h"

struct GprDataset {
    GprMetadata               metadata;
    std::vector<GprCorRecord> coordinates;
    std::vector<GprTrace>     traces;

    void printSummary() const;
};

class GprDatasetLoader {
public:
    GprDataset load(const std::filesystem::path& basePath,
                    Endianness endian = Endianness::Little);
};
