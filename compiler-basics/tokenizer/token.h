#pragma once
#include <string>

enum class TokenType {
    Let,
    Print,
    Identifier,
    Number,
    Equal,
    Plus,
    Semicolon,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type = TokenType::Unknown;
    std::string lexeme;
};
