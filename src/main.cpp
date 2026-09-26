#include "lexer.h"
#include "parser.h"
#include "codegen.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>



int main(int argc, char* argv[]) {
    const bool emitIR = argc == 3 && std::string(argv[1]) == "--emit-ir";
    if ((!emitIR && argc != 2) || (argc == 2 && std::string(argv[1]) == "--emit-ir")) {
        std::cerr << "Usage: tokenizer [--emit-ir] <source-file>\n";
        return 1;
    }

    const char* sourcePath = argv[emitIR ? 2 : 1];
    std::ifstream input{sourcePath};

    if(!input.is_open()) {
        std::cerr << "Error could not open " << sourcePath << "\n";
        return 1;
    }

    std::string source{ std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};

    try {
        Lexer lexer{source};
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser{std::move(tokens)};

        auto statements = parser.parse();

        DefinedNames names;
        for (const auto& statement : statements)
        {
            statement -> analyze(names);
        }

        if (emitIR) {
            CodegenContext context;
            std::cout << context.generate(statements);
            return 0;
        }

        SymbolTable symbols;

        for(auto& statement : statements) {
            statement -> execute(symbols);
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
