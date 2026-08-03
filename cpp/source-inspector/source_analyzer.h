#pragma once
#include <string>

struct AnalysisResult {
    bool success = false;
    int totalLines = 0;
};

AnalysisResult analyzeSource(const std::string& filePath);
