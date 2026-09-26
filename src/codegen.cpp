#include "codegen.h"
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <stdexcept>

CodegenContext::CodegenContext()
    : module_("toy", context_), builder_(context_) {}

llvm::Value* CodegenContext::number(Integer value) {
    return llvm::ConstantInt::getSigned(builder_.getInt32Ty(), value);
}

void CodegenContext::print(llvm::Value* value) {
    auto* type = llvm::FunctionType::get(builder_.getInt32Ty(), {builder_.getPtrTy()}, true);
    auto printfFunction = module_.getOrInsertFunction("printf", type);
    auto* format = builder_.CreateGlobalString("%d\n");
    builder_.CreateCall(printfFunction, {format, value});
}

std::string CodegenContext::generate(const std::vector<std::unique_ptr<Statement>>& statements) {
    if (!module_.empty()) {
        throw std::logic_error("Code generation context has already been used");
    }
    auto* type = llvm::FunctionType::get(builder_.getInt32Ty(), false);
    auto* main = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", module_);
    builder_.SetInsertPoint(llvm::BasicBlock::Create(context_, "entry", main));
    for (const auto& statement : statements) {
        statement->codegen(*this);
    }
    builder_.CreateRet(builder_.getInt32(0));

    std::string errors;
    llvm::raw_string_ostream errorStream(errors);
    if (llvm::verifyModule(module_, &errorStream)) {
        throw std::runtime_error("Invalid generated LLVM IR: " + errors);
    }
    std::string ir;
    llvm::raw_string_ostream output(ir);
    module_.print(output, nullptr);
    return ir;
}

llvm::Value* Expression::codegen(CodegenContext&) const {
    throw std::runtime_error("LLVM IR generation does not yet support this expression");
}

void Statement::codegen(CodegenContext&) const {
    throw std::runtime_error("LLVM IR generation does not yet support this statement");
}

llvm::Value* NumberExpression::codegen(CodegenContext& context) const {
    return context.number(value_);
}

void PrintStatement::codegen(CodegenContext& context) const {
    context.print(expression_->codegen(context));
}
