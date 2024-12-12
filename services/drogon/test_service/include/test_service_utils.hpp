#pragma once

#include <string>
#include <stdexcept>

namespace TestServiceUtils {
    // Utility to convert strings to integers with error handling
    inline int safeStringToInt(const std::string& str) {
        try {
            return std::stoi(str);
        } catch (const std::exception&) {
            throw std::invalid_argument("Invalid integer: " + str);
        }
    }
}
