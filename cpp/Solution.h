#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <cstdint>
#include <cstdint>
#include <string_view>

#include "utils/Image.h"
#include "utils/FunctionTracer.h"
#include "utils/EyePatterns.h"

void print(const StrideImage &image) {

    for (int row = 0; row < image.resolution.height; ++row) {
        for (int col = 0; col < image.resolution.width; ++col) {

            std::cout << (image.redPixels.at(row * image.resolution.width + col) >= 200 ? "." : " ");
        }
        std::cout << std::endl;
    }
}

bool isMatched(const StrideImage &image, int imageCol, int imageRow, const EyePattern &pattern) {
    int rowOffset = 0;

    if (static_cast<size_t>(image.resolution.height) < imageRow + pattern.size()) {
        return false;
    }

    for (auto &row: pattern) {

        if (static_cast<size_t>(image.resolution.width) < imageCol + std::string_view(row).size()) {
            return false;
        }

        int colOffset = 0;
        for (char col: std::string_view(row)) {
            auto red = image.redPixels.at((imageRow + rowOffset) * image.resolution.width + imageCol + colOffset);

            if (col != ' ') {
                if (red < 200) {
                    return false;
                }
            }
            colOffset++;
        }

        rowOffset++;
    }

    return true;
}

const EyePattern *match(const StrideImage &image, int imageCol, int imageRow) {

    for (auto &pattern: EYE_PATTERNS) {
        if (isMatched(image, imageCol, imageRow, pattern)) {
            return &pattern;
        }
    }

    return nullptr;
}

void filter(StrideImage &image, int imageCol, int imageRow, const EyePattern &pattern) {

    int rowOffset = 0;

    for (auto &row: pattern) {
        int colOffset = 0;

        for ([[maybe_unused]]char col: std::string_view(row)) {
            auto &red = image.redPixels.at((imageRow + rowOffset) * image.resolution.width + imageCol + colOffset);

            if (red >= 200 && std::string_view(pattern.at(rowOffset)).at(colOffset)) {
                red -= 150;
            }

            colOffset++;
        }

        rowOffset++;
    }
}

class Solution {
public:
//    void compute([[maybe_unused]]std::vector<PackedImage> &images) {
//        FunctionTracer<std::chrono::milliseconds> tracer("compute", "ms");
//        throw std::runtime_error("Not supported");
//    }

    void compute([[maybe_unused]]std::vector<StrideImage> &images) {

        FunctionTracer<std::chrono::milliseconds> tracer("compute", "ms");

        for (auto &image: images) {

            for (int row = 0; row < image.resolution.height; ++row) {
                for (int col = 0; col < image.resolution.width; ++col) {

                    auto *matched = match(image, col, row);

                    if (matched != nullptr) {
                        filter(image, col, row, *matched);
                    }
                }
            }
        }
    }
};

#endif /* SOLUTION_H_ */