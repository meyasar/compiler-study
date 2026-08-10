#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class StaticAnalyzerPass : public PassInfoMixin<StaticAnalyzerPass> {
public:
  static bool isRequired() { return true; }

  PreservedAnalyses run(Function &function, FunctionAnalysisManager &) {
    unsigned basicBlockCount = 0;
    unsigned instructionCount = 0;

    for (BasicBlock &block : function) {
      basicBlockCount++;

      for (Instruction &instruction : block) {
        (void)instruction;
        instructionCount++;
      }
    }
    errs() << "Function: " << function.getName() << "\n";
    errs() << "Basic blocks: " << basicBlockCount << "\n";
    errs() << "Instructions: " << instructionCount << "\n";

    return PreservedAnalyses::all();
  }
};
} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "StaticAnalyzerPass", LLVM_VERSION_STRING,
          [](PassBuilder &passBuilder) {
            passBuilder.registerPipelineParsingCallback(
                [](StringRef passName, FunctionPassManager &functionPassManager,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (passName == "static-analyzer") {
                    functionPassManager.addPass(StaticAnalyzerPass());
                    return true;
                  }

                  return false;
                });
          }};
}
