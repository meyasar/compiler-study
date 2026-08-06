#pragma once
#include <memory>
#include <string>
#include <unordered_map>

using SymbolTable = std::unordered_map<std::string, int>;

class Expression {
public:
    virtual ~Expression() = default;
    virtual int evaluate(const SymbolTable& symbols) const = 0;
};

class NumberExpression : public Expression {
public:
    NumberExpression(int value);
    int evaluate(const SymbolTable& symbols) const override;

private:
    int value_ = 0;
};

class VariableExpression : public Expression {
public:
    VariableExpression(const std::string& name);
    int evaluate(const SymbolTable& symbols) const override;
private:
    std::string name_;
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
        std::unique_ptr<Expression> right
    );
    int evaluate(const SymbolTable& symbols) const override;
private:
    std::unique_ptr<Expression> left_;
    BinaryOperator operator_;
    std::unique_ptr<Expression> right_;
};

class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute(SymbolTable& symbols) const = 0;
};

class LetStatement : public Statement {
public:
    LetStatement(const std::string& name, std::unique_ptr<Expression> initializer);
    void execute(SymbolTable& symbols) const override;
private:
    std::string name_;
    std::unique_ptr<Expression> initializer_;
};

class PrintStatement : public Statement {
public:
    PrintStatement(std::unique_ptr<Expression> expression);
    void execute(SymbolTable& symbols) const override;
private:
    std::unique_ptr<Expression> expression_;
};
