#include <iostream>
#include "application.h"
#include "source_analyzer.h"

int runApplication(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: source-inspector <source-file>" << std::endl;
        return 1;
    }
    const char* fileName = argv[1];
    int lineCount = countLines(fileName);
    if (lineCount == -1) {
        std::cout << "Error: could not open " << fileName << std::endl;
        return 1;
    }
    std::cout << "Analyzing: " << fileName << std::endl;
    std::cout << "Total Lines: " << lineCount << std::endl;
    return 0;
}
