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
    if (character == '\n')
    {
        line_++;
        column_ = 1;
    } else
    {
        column_++;
    }

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
        return Token{TokenType::Let, lexeme, startLine_, startColumn_};
    }

    if (lexeme == "print") {
        return Token{TokenType::Print, lexeme, startLine_, startColumn_};
    }

    return Token{TokenType::Identifier, lexeme, startLine_, startColumn_};
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

    return Token{TokenType::Number, lexeme, startLine_, startColumn_};
}

Token Lexer::scanToken() {

    skipWhitespace();
    start_ = current_;

    startLine_ = line_;
    startColumn_ = column_;

    if (isAtEnd()) {
        return Token{TokenType::EndOfFile, "", startLine_, startColumn_};
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
            return Token{TokenType::Equal, "=", startLine_, startColumn_};
        case '+':
            return Token{TokenType::Plus, "+", startLine_, startColumn_};
        case '-':
            return Token{TokenType::Minus, "-", startLine_, startColumn_};
        case '*':
            return Token{TokenType::Star, "*", startLine_, startColumn_};
        case '/':
            return Token{TokenType::Slash, "/", startLine_, startColumn_};
        case '(':
            return Token{TokenType::LeftParen, "(", startLine_, startColumn_};
        case ')':
            return Token{TokenType::RightParen, ")", startLine_, startColumn_};
        case ';':
            return Token{TokenType::Semicolon, ";", startLine_, startColumn_};
        default:
            return Token{TokenType::Unknown, std::string(1, character), startLine_, startColumn_};
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
