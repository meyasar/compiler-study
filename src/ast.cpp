#include "ast.h"
#include <stdexcept>
#include <utility>
#include <iostream>
#include <limits>

namespace {
Integer checkedInteger(std::int64_t value, SourceLocation location) {
    if (value < std::numeric_limits<Integer>::min() ||
        value > std::numeric_limits<Integer>::max()) {
        throw sourceError(location, "Integer overflow");
    }
    return static_cast<Integer>(value);
}
}

NumberExpression::NumberExpression(Integer value) : value_(value) {}
VariableExpression::VariableExpression(const std::string& name, SourceLocation location)
    : name_(name), location_(location) {}
BinaryExpression::BinaryExpression(
    std::unique_ptr<Expression> left,
    BinaryOperator binaryOperator,
    std::unique_ptr<Expression> right,
    SourceLocation location
)
    : left_(std::move(left)),
      operator_(binaryOperator),
      right_(std::move(right)),
      location_(location) {
}
LetStatement::LetStatement(const std::string& name, std::unique_ptr<Expression> initializer, SourceLocation location)
    : name_(name), initializer_(std::move(initializer)), location_(location) {}
PrintStatement::PrintStatement(std::unique_ptr<Expression> expression) : expression_(std::move(expression)) {}


Integer NumberExpression::evaluate(const SymbolTable&) const {
    return value_;
}

void NumberExpression::analyze(const DefinedNames&) const {
}

Integer VariableExpression::evaluate(const SymbolTable& symbols) const {
    const auto iterator = symbols.find(name_);
    if (iterator == symbols.end()) {
        throw sourceError(location_, "Undefined variable: " + name_);
    }
    return iterator->second;
}
void VariableExpression::analyze(const DefinedNames& names) const
{
    const auto iterator = names.find(name_);
    if (iterator == names.end())
    {
        throw sourceError(location_, "Undefined variable: " + name_);
    }
}

UnaryExpression::UnaryExpression(std::unique_ptr<Expression> operand, SourceLocation location)
    : operand_(std::move(operand)), location_(location) {}

Integer UnaryExpression::evaluate(const SymbolTable& symbols) const {
    const Integer value = operand_->evaluate(symbols);
    if (value == std::numeric_limits<Integer>::min()) {
        throw sourceError(location_, "Integer overflow in unary minus");
    }
    return -value;
}

void UnaryExpression::analyze(const DefinedNames& names) const {
    operand_->analyze(names);
}

Integer BinaryExpression::evaluate(const SymbolTable& symbols) const {
    const std::int64_t leftValue = left_->evaluate(symbols);
    const std::int64_t rightValue = right_->evaluate(symbols);

    switch (operator_) {
    case BinaryOperator::Add:
        return checkedInteger(leftValue + rightValue, location_);
    case BinaryOperator::Subtract:
        return checkedInteger(leftValue - rightValue, location_);
    case BinaryOperator::Multiply:
        return checkedInteger(leftValue * rightValue, location_);
    case BinaryOperator::Divide:
        if (rightValue == 0) {
            throw sourceError(location_, "Division by zero");
        }
        return checkedInteger(leftValue / rightValue, location_);
    }

    throw std::logic_error("Unknown binary operator");
}

void BinaryExpression::analyze(const DefinedNames& names) const
{
    left_->analyze(names);
    right_->analyze(names);
}

void LetStatement::execute(SymbolTable& symbols) const {
    if (symbols.find(name_) != symbols.end())
    {
        throw sourceError(location_, "Variable " + name_ + " already exists");
    }

    Integer value = initializer_->evaluate(symbols);
    symbols[name_] = value;

}

void LetStatement::analyze(DefinedNames& names) const
{
    const auto iterator = names.find(name_);
    if (iterator != names.end())
    {
        throw sourceError(location_, "Variable " + name_ + " already exists");
    }
    initializer_->analyze(names);
    names.insert(name_);
}

void PrintStatement::execute(SymbolTable& symbols) const {
    Integer value = expression_->evaluate(symbols);
    std::cout << value << std::endl;
}

void PrintStatement::analyze(DefinedNames& names) const
{
    expression_->analyze(names);
}
