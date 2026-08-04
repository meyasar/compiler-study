#include "application.h"
#include "source_analyzer.h"
#include "report_printer.h"
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

    const TextReportPrinter textPrinter;
    const ReportPrinter& printer = textPrinter;

    for (const std::string& fileName : fileNames) {
        const SourceAnalyzer analyzer(fileName);
        const AnalysisResult result = analyzer.analyze();

        if (!result.success) {
            std::cerr << "Error: could not open " << fileName << std::endl;
            hadError = true;
            continue;
        }

        printer.print(fileName, result);
    }

    if (hadError) {
        return 1;
    }
    return 0;
}
