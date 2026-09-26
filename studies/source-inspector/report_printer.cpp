#include "report_printer.h"

#include <iostream>

void TextReportPrinter::print(
    const std::string& fileName,
    const AnalysisResult& result
) const {
    std::cout << "Analyzing: " << fileName << std::endl;
    std::cout << "Total Lines: " << result.totalLines << std::endl;
    std::cout << "Blank Lines: " << result.blankLines << std::endl;
    std::cout << "Code Lines: " << result.codeLines << std::endl;
}
