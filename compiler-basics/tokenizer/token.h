#pragma once
#include <string>

enum class TokenType {
    Let,
    Print,
    Identifier,
    Number,
    Equal,
    Plus,
    Minus,
    Star,
    Slash,
    LeftParen,
    RightParen,
    Semicolon,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type = TokenType::Unknown;
    std::string lexeme;
};
