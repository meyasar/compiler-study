#include "lexer.h"
#include "parser.h"
#include <string>
#include <utility>


int main() {
    std::string source = "let x = (10 + 4) * 3 - 8 / 2; print x;";

    Lexer lexer{source};
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser{std::move(tokens)};
    auto statements = parser.parse();

    SymbolTable symbols;

    for (auto& statement : statements) {
        statement->execute(symbols);
    }
}
