#pragma once

#include "ast.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <vector>

class CodegenContext {
public:
    CodegenContext();
    llvm::Value* number(Integer value);
    void print(llvm::Value* value);
    std::string generate(const std::vector<std::unique_ptr<Statement>>& statements);
private:
    llvm::LLVMContext context_;
    llvm::Module module_;
    llvm::IRBuilder<> builder_;
};
