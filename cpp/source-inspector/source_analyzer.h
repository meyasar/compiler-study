#pragma once
#include <string>

struct AnalysisResult {
    bool success = false;
    int totalLines = 0;
    int blankLines = 0;
    int codeLines = 0;
};

AnalysisResult analyzeSource(const std::string& filePath);
