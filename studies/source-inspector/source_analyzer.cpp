#include "source_analyzer.h"
#include <fstream>
#include <string>

SourceAnalyzer::SourceAnalyzer(const std::string& filePath)
    :filePath_(filePath) {
}

AnalysisResult SourceAnalyzer::analyze() const {
    AnalysisResult result;
    std::ifstream input(filePath_);

    if (!input.is_open()) {
        return result;
    }

    std::string line;
    const auto isBlankLine = [](const std::string& currentLine) {
        return currentLine.find_first_not_of(" \t\r")
        == std::string::npos;
    };

    while (std::getline(input, line)) {
        if (isBlankLine(line)) {
            result.blankLines++;
        } else {
            result.codeLines++;
        }
    }
    result.totalLines = result.blankLines + result.codeLines;
    result.success = true;

    return result;
}
