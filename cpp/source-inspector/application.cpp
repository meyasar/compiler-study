#include "application.h"
#include "source_analyzer.h"
#include <iostream>
#include <string>

int runApplication(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: source-inspector <source-file>" << std::endl;
        return 1;
    }
    const std::string fileName = argv[1];
    const AnalysisResult result = analyzeSource(fileName);
    if (!result.success) {
        std::cerr << "Error: could not open " << fileName << std::endl;
        return 1;
    }

    std::cout << "Analyzing: " << fileName << std::endl;
    std::cout << "Total Lines: " << result.totalLines << std::endl;
    std::cout << "Blank Lines: " << result.blankLines << std::endl;
    std::cout << "Code Lines: " << result.codeLines << std::endl;
    return 0;
}
