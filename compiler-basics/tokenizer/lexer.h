#pragma once
#include "token.h"
#include <cstddef>
#include <string>
#include <vector>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();
private:
    bool isAtEnd() const;
    char advance();
    char peek() const;
    void skipWhitespace();
    Token scanToken();
    Token scanIdentifier();
    Token scanNumber();

    std::string source_;
    std::size_t start_ = 0;
    std::size_t current_ = 0;
};
