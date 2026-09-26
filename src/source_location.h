#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>

struct SourceLocation {
    std::size_t line = 1;
    std::size_t column = 1;
};

inline std::runtime_error sourceError(SourceLocation location, const std::string& message) {
    return std::runtime_error(
        "at " + std::to_string(location.line) + ":" +
        std::to_string(location.column) + ": " + message);
}
