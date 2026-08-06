#include "parser.h"
#include "token.h"
#include <memory>
#include <stdexcept>
#include <utility>

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
    throw std::runtime_error(message + " Found: " + peek().lexeme);
}

std::unique_ptr<Expression> Parser::parsePrimary() {

    if (match(TokenType::LeftParen)) {
        auto expression = parseExpression();

        consume(TokenType::RightParen, "Expected ') after expression." );

        return expression;
    }
    if (match(TokenType::Number)) {
        const Token& numberToken = previous(); //Used previous() because match() consumes a token
        const int value = std::stoi(numberToken.lexeme);
        return std::make_unique<NumberExpression>(value);
    }
    if (match(TokenType::Identifier)) {
        const Token& identifierToken = previous();
        return std::make_unique<VariableExpression>(identifierToken.lexeme);
    }

    throw std::runtime_error("Expected expression. Found: " + peek().lexeme);
}

std::unique_ptr<Expression> Parser::parseTerm() {
    auto expression = parsePrimary();

    while (check(TokenType::Star) || check(TokenType::Slash)) {
        const Token& operatorToken = advance();
        auto right = parsePrimary();

        BinaryOperator binaryOperator = BinaryOperator::Multiply;

        if (operatorToken.type == TokenType::Slash) {
            binaryOperator = BinaryOperator::Divide;
        }

        expression = std::make_unique<BinaryExpression>(std::move(expression), binaryOperator, std::move(right));

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

        expression = std::make_unique<BinaryExpression>(std::move(expression),binaryOperator, std::move(right));
    }
    return expression;
}

std::unique_ptr<Statement> Parser::parseLetStatement() {
    const Token& name = consume(TokenType::Identifier, "Expected variable name after 'let'.");

    consume(TokenType::Equal, "Expected '=' after variable name.");

    auto initializer = parseExpression();

    consume(TokenType::Semicolon, "Expected ';' after variable declaration.");

    return std::make_unique<LetStatement>(name.lexeme, std::move(initializer));
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

    throw std::runtime_error("Expected statement. Found: " + peek().lexeme);
}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;

    while (!isAtEnd()) {
        statements.push_back(parseStatement());
    }

    return statements;
}
