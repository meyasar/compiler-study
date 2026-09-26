#pragma once
#include "source_location.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

using Integer = std::int32_t;
using SymbolTable = std::unordered_map<std::string, Integer>;
using DefinedNames = std::unordered_set<std::string>;

class Expression {
public:
    virtual ~Expression() = default;
    virtual Integer evaluate(const SymbolTable& symbols) const = 0;
    virtual void analyze(const DefinedNames& names) const = 0;
};

class NumberExpression : public Expression {
public:
    NumberExpression(Integer value);
    Integer evaluate(const SymbolTable& symbols) const override;
    void analyze(const DefinedNames& names) const override;
private:
    Integer value_ = 0;
};

class VariableExpression : public Expression {
public:
    VariableExpression(const std::string& name, SourceLocation location);
    Integer evaluate(const SymbolTable& symbols) const override;
    void analyze(const DefinedNames& names) const override;
private:
    std::string name_;
    SourceLocation location_;
};

class UnaryExpression : public Expression {
public:
    UnaryExpression(std::unique_ptr<Expression> operand, SourceLocation location);
    Integer evaluate(const SymbolTable& symbols) const override;
    void analyze(const DefinedNames& names) const override;
private:
    std::unique_ptr<Expression> operand_;
    SourceLocation location_;
};

enum class BinaryOperator {
    Add,
    Subtract,
    Multiply,
    Divide
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(
        std::unique_ptr<Expression> left,
        BinaryOperator binaryOperator,
        std::unique_ptr<Expression> right,
        SourceLocation location
    );
    Integer evaluate(const SymbolTable& symbols) const override;
    void analyze(const DefinedNames& names) const override;
private:
    std::unique_ptr<Expression> left_;
    BinaryOperator operator_;
    std::unique_ptr<Expression> right_;
    SourceLocation location_;
};

class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute(SymbolTable& symbols) const = 0;
    virtual void analyze(DefinedNames& names) const = 0;
};

class LetStatement : public Statement {
public:
    LetStatement(const std::string& name, std::unique_ptr<Expression> initializer, SourceLocation location);
    void execute(SymbolTable& symbols) const override;
    void analyze(DefinedNames& names) const override;
private:
    std::string name_;
    std::unique_ptr<Expression> initializer_;
    SourceLocation location_;
};

class PrintStatement : public Statement {
public:
    PrintStatement(std::unique_ptr<Expression> expression);
    void execute(SymbolTable& symbols) const override;
    void analyze(DefinedNames& names) const override;
private:
    std::unique_ptr<Expression> expression_;
};
