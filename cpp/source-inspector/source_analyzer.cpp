#include "source_analyzer.h"
#include <fstream>
#include <string>

AnalysisResult analyzeSource(const std::string& filePath){
    AnalysisResult result;
    std::ifstream input(filePath);

    if(!input.is_open()){
        return result;
    }
    std::string line;

    while(std::getline(input, line)){
        result.totalLines++;
    }
    result.success = true;

    return result;
}
