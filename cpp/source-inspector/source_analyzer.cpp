#include "source_analyzer.h"
#include <fstream>
#include <string>

AnalysisResult analyzeSource(const std::string& filePath){
    AnalysisResult result;
    std::ifstream input(filePath);

    if(!input.is_open()) {
        return result;
    }

    std::string line;

    while(std::getline(input, line)) {
        if(line.find_first_not_of(" \t\r") == std::string::npos){
            result.blankLines++;
        }
        else {
            result.codeLines++;
        }
    }
    result.totalLines = result.blankLines + result.codeLines;
    result.success = true;

    return result;
}
