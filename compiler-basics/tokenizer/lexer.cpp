#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& source)
    : source_(source) {
}

bool Lexer::isAtEnd() const {
    return current_ >= source_.size();
}

char Lexer::peek() const {
    if(isAtEnd()) {
        return '\0';
    }

    return source_[current_];
}

char Lexer::advance() {
    char character = peek();
    current_++;
    return character;
}

void Lexer::skipWhitespace() {
      while (!isAtEnd()) {
          const char character = peek();

          if (
              character != ' ' &&
              character != '\t' &&
              character != '\n' &&
              character != '\r'
          ) {
              break;
          }

          advance();
      }
  }

Token Lexer::scanIdentifier() {

    while (!isAtEnd()) {
        char character = peek();
        bool isValidCharacter = std::isalnum(character) || character == '_';

        if(!isValidCharacter) {
            break;
        }
        advance();
      }

    std::string lexeme = source_.substr(start_, current_ - start_);

    if (lexeme == "let") {
        return Token{TokenType::Let, lexeme};
    }

    if (lexeme == "print") {
        return Token{TokenType::Print, lexeme};
    }

    return Token{TokenType::Identifier, lexeme};
}

Token Lexer::scanNumber() {
    while(!isAtEnd()) {
        char number = peek();
        bool isValidNumber = std::isdigit(number);

        if(!isValidNumber) {
            break;
        }
        advance();
    }
    std::string lexeme = source_.substr(start_, current_ - start_);

    return Token{TokenType::Number, lexeme};
}

Token Lexer::scanToken() {

    skipWhitespace();
    start_ = current_;

    if (isAtEnd()) {
        return Token{TokenType::EndOfFile, ""};
    }

    char character = advance();

    if (isalpha(character) || character == '_') {
        return scanIdentifier();
    }

    if (isdigit(character)) {
        return scanNumber();
    }

    switch (character) {
        case '=':
            return Token{TokenType::Equal, "="};
        case '+':
            return Token{TokenType::Plus, "+"};
        case ';':
            return Token{TokenType::Semicolon, ";"};
        default:
            return Token{TokenType::Unknown, std::string(1, character)};
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = scanToken();

    while (token.type != TokenType::EndOfFile) {
        tokens.push_back(token);
        token = scanToken();
    }

    tokens.push_back(token); // To add EndOfFile token to the vector
    return tokens;

}
