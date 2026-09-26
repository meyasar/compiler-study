#include "parser.h"
#include "token.h"
#include <memory>
#include <stdexcept>
#include <utility>
#include <limits>

namespace {
SourceLocation locationOf(const Token& token) {
    return {token.line, token.column};
}

std::string foundText(const Token& token) {
    return token.type == TokenType::EndOfFile ? "<end of file>" : token.lexeme;
}
}

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

const Token& Parser::peek() const {
    return tokens_[current_];
}

const Token& Parser::previous() const {
    if (current_ == 0) {
        throw std::logic_error("No token has been consumed yet");
    }
    return tokens_[current_ -1];
}

bool Parser::isAtEnd() const {
    if (peek().type == TokenType::EndOfFile) {
        return true;
    }
    return false;
}

const Token& Parser::advance() {
    const Token& value = peek();
    if (!isAtEnd()) {
        current_++;
    }
    return value;
}

bool Parser::check(TokenType type) const {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (!check(type)) {
        return false;
    }

    advance();
    return true;
}

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        return advance();
    }
    throw sourceError(locationOf(peek()), message + " Found: " + foundText(peek()));
}

std::unique_ptr<Expression> Parser::parsePrimary() {

    if (match(TokenType::LeftParen)) {
        auto expression = parseExpression();

        consume(TokenType::RightParen, "Expected ')' after expression.");

        return expression;
    }
    if (match(TokenType::Number)) {
        const Token& numberToken = previous(); //Used previous() because match() consumes a token
        std::int64_t value;
        try {
            value = std::stoll(numberToken.lexeme);
        } catch (const std::out_of_range&) {
            throw sourceError(locationOf(numberToken), "Integer literal out of range: " + numberToken.lexeme);
        }
        if (value > std::numeric_limits<Integer>::max()) {
            throw sourceError(locationOf(numberToken), "Integer literal out of range: " + numberToken.lexeme);
        }
        return std::make_unique<NumberExpression>(static_cast<Integer>(value));
    }
    if (match(TokenType::Identifier)) {
        const Token& identifierToken = previous();
        return std::make_unique<VariableExpression>(identifierToken.lexeme, locationOf(identifierToken));
    }

    throw sourceError(locationOf(peek()), "Expected expression. Found: " + foundText(peek()));
}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (match(TokenType::Minus)) {
        const SourceLocation location = locationOf(previous());
        if (check(TokenType::Number)) {
            const auto& digits = peek().lexeme;
            const auto first = digits.find_first_not_of('0');
            if (first != std::string::npos && digits.substr(first) == "2147483648") {
                advance();
                return std::make_unique<NumberExpression>(std::numeric_limits<Integer>::min());
            }
        }
        auto operand = parseUnary();
        return std::make_unique<UnaryExpression>(std::move(operand), location);
    }
    return parsePrimary();
}

std::unique_ptr<Expression> Parser::parseTerm() {
    auto expression = parseUnary();

    while (check(TokenType::Star) || check(TokenType::Slash)) {
        const Token& operatorToken = advance();
        auto right = parseUnary();

        BinaryOperator binaryOperator = BinaryOperator::Multiply;

        if (operatorToken.type == TokenType::Slash) {
            binaryOperator = BinaryOperator::Divide;
        }

        expression = std::make_unique<BinaryExpression>(std::move(expression), binaryOperator, std::move(right), locationOf(operatorToken));

    }
    return expression;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    auto expression = parseTerm();

    while (check(TokenType::Plus) || check(TokenType::Minus)) {
        const Token& operatorToken = advance();
        auto right = parseTerm();

        BinaryOperator binaryOperator = BinaryOperator::Add;

        if (operatorToken.type == TokenType::Minus) {
            binaryOperator = BinaryOperator::Subtract;
        }

        expression = std::make_unique<BinaryExpression>(std::move(expression), binaryOperator, std::move(right), locationOf(operatorToken));
    }
    return expression;
}

std::unique_ptr<Statement> Parser::parseLetStatement() {
    const Token& name = consume(TokenType::Identifier, "Expected variable name after 'let'.");

    consume(TokenType::Equal, "Expected '=' after variable name.");

    auto initializer = parseExpression();

    consume(TokenType::Semicolon, "Expected ';' after variable declaration.");

    return std::make_unique<LetStatement>(name.lexeme, std::move(initializer), locationOf(name));
}

std::unique_ptr<Statement> Parser::parsePrintStatement() {
    auto expression = parseExpression();

    consume(TokenType::Semicolon, "Expected ';' after print expression.");

    return std::make_unique<PrintStatement>(std::move(expression));
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (match(TokenType::Let)) {
        return parseLetStatement();
    }
    if (match(TokenType::Print)) {
        return parsePrintStatement();
    }

    throw sourceError(locationOf(peek()), "Expected statement. Found: " + foundText(peek()));
}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;

    while (!isAtEnd()) {
        statements.push_back(parseStatement());
    }

    return statements;
}
