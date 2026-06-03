#include "gpr_dataset.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: gpr_demo <base_path>" << std::endl;
        std::cerr << "Example: gpr_demo testData/华明路_001_A01" << std::endl;
        return 1;
    }

    try {
        GprDatasetLoader loader;
        GprDataset dataset = loader.load(argv[1]);
        dataset.printSummary();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
