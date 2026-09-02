#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <iterator>



int main(int argc, char* argv[]) {
    if (argc != 2 ) {
        std::cerr << "Usage: tokenizer <source-file>\n";
        return 1;
    }

    std::ifstream input{argv[1]};

    if(!input.is_open()) {
        std::cerr << "Error could not open " << argv[1] << "\n";
        return 1;
    }

    std::string source{ std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};

    Lexer lexer{source};
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser{std::move(tokens)};
    auto statements = parser.parse();

    SymbolTable symbols;

    for(auto& statement : statements) {
        statement -> execute(symbols);
    }
    return 0;
}
