#include "application.h"
#include "source_analyzer.h"
#include <iostream>
#include <string>
#include <vector>

int runApplication(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: source-inspector <source-file>" << std::endl;
        return 1;
    }
    bool hadError = false;
    std::vector<std::string> fileNames;


    for (int i = 1; i < argc; i++) {
        fileNames.emplace_back(argv[i]);
    }


    for (const std::string& fileName : fileNames) {
        const SourceAnalyzer analyzer(fileName);
        const AnalysisResult result = analyzer.analyze();

        if (!result.success) {
            std::cerr << "Error: could not open " << fileName << std::endl;
            hadError = true;
            continue;
        }

        std::cout << "Analyzing: " << fileName << std::endl;
        std::cout << "Total Lines: " << result.totalLines << std::endl;
        std::cout << "Blank Lines: " << result.blankLines << std::endl;
        std::cout << "Code Lines: " << result.codeLines << std::endl;
    }

    if (hadError) {
        return 1;
    }
    return 0;
}
