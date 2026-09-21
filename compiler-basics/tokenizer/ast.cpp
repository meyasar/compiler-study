#include "ast.h"
#include <stdexcept>
#include <utility>
#include <iostream>

NumberExpression::NumberExpression(int value) : value_(value) {}
VariableExpression::VariableExpression(const std::string& name) : name_(name){}
BinaryExpression::BinaryExpression(
    std::unique_ptr<Expression> left,
    BinaryOperator binaryOperator,
    std::unique_ptr<Expression> right
)
    : left_(std::move(left)),
      operator_(binaryOperator),
      right_(std::move(right)) {
}
LetStatement::LetStatement(const std::string& name, std::unique_ptr<Expression> initializer) : name_(name), initializer_(std::move(initializer)) {}
PrintStatement::PrintStatement(std::unique_ptr<Expression> expression) : expression_(std::move(expression)) {}


int NumberExpression::evaluate(const SymbolTable&) const {
    return value_;
}

int VariableExpression::evaluate(const SymbolTable& symbols) const {
    const auto iterator = symbols.find(name_);
    if (iterator == symbols.end()) {
        throw std::runtime_error("Undefined variable: " + name_);
    }
    return iterator->second;
}

int BinaryExpression::evaluate(const SymbolTable& symbols) const {
    const int leftValue = left_->evaluate(symbols);
    const int rightValue = right_->evaluate(symbols);

    switch (operator_) {
    case BinaryOperator::Add:
        return leftValue + rightValue;
    case BinaryOperator::Subtract:
        return leftValue - rightValue;
    case BinaryOperator::Multiply:
        return leftValue * rightValue;
    case BinaryOperator::Divide:
        if (rightValue == 0) {
            throw std::runtime_error("Division by zero");
        }
        return leftValue / rightValue;
    }

    throw std::logic_error("Unknown binary operator");
}

void LetStatement::execute(SymbolTable& symbols) const {
    if (symbols.find(name_) != symbols.end())
    {
        throw std::runtime_error(std::string("Variable ") + name_ + " already exists");
    }

    int value = initializer_->evaluate(symbols);
    symbols[name_] = value;

}

void PrintStatement::execute(SymbolTable& symbols) const {
    int value = expression_->evaluate(symbols);
    std::cout << value << std::endl;
}
