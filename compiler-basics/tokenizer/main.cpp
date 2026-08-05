#include "lexer.h"
#include "token.h"
#include <iostream>
#include <string>
#include <vector>

const char* tokenTypeName(TokenType type) {
     switch (type) {
     case TokenType::Let:
         return "LET";
     case TokenType::Print:
         return "PRINT";
     case TokenType::Identifier:
         return "IDENTIFIER";
     case TokenType::Number:
         return "NUMBER";
     case TokenType::Equal:
         return "EQUAL";
     case TokenType::Plus:
         return "PLUS";
     case TokenType::Semicolon:
         return "SEMICOLON";
     case TokenType::EndOfFile:
         return "END_OF_FILE";
     case TokenType::Unknown:
         return "UNKNOWN";
     }

     return "UNKNOWN";
 }

int main() {
    std::string source = "let user_name = 10; print user_name; @";
    Lexer lexer{source};
    std::vector<Token> tokens = lexer.tokenize();

    for (Token& token: tokens) {
        std::cout << tokenTypeName(token.type) << "(\"" << token.lexeme << "\")" << std::endl;
    }
}
