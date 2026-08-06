#pragma once
#include "ast.h"
#include "token.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class Parser {
public:
    Parser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<Statement>> parse();
private:
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();

    bool check(TokenType type) const;
    bool match(TokenType type);

    const Token& consume(TokenType type, const std::string& message);

    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseLetStatement();
    std::unique_ptr<Statement> parsePrintStatement();

    std::unique_ptr<Expression> parseExpression(); // + and -
    std::unique_ptr<Expression> parseTerm();       // * and /
    std::unique_ptr<Expression> parsePrimary();    // number, variable and paranthesis

    std::vector<Token> tokens_;
    std::size_t current_ = 0;
};
