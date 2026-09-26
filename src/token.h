#pragma once
#include <string>
#include <cstddef>

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
    std::size_t line = 1;
    std::size_t column = 1;
};
