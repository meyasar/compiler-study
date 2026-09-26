#pragma once
#include "source_analyzer.h"
#include <string>

class ReportPrinter {
public:
    virtual ~ReportPrinter() = default;
    virtual void print(
        const std::string& fileName,
        const AnalysisResult& result
    ) const = 0;
};

class TextReportPrinter : public ReportPrinter {
public:
    void print(
        const std::string& fileName,
        const AnalysisResult& result
    ) const override;
};
