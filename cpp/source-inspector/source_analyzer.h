#pragma once
#include <string>

struct AnalysisResult {
    bool success = false;
    int totalLines = 0;
    int blankLines = 0;
    int codeLines = 0;
};

class SourceAnalyzer {
    public:
          SourceAnalyzer(const std::string& filePath);
          AnalysisResult analyze() const;

    private:
          std::string filePath_;
};
