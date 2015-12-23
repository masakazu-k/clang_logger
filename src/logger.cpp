#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;


class LoggerRegister:public ModulePass{
public:
    static char ID;
    
    LoggerRegister():ModulePass(ID){}
    
    bool runOnModule(Module &m){
	for (auto &f : m) {
	for (auto &B : f) {
        	errs() << "TARGET: " << f.getName() << "!\n";
		IRBuilder<> builder(&*B.begin());

		// putsの引数に渡す定数
		Value *func_header = builder.CreateGlobalStringPtr("[callfunc] ");
		Value *func_name = builder.CreateGlobalStringPtr(f.getName());
		
		// putsの引数定義
		std::vector<llvm::Type *> putsArgs;
		putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
		llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
		
		FunctionType *putsType = FunctionType::get(builder.getInt32Ty(), argsRef, false);

		// putsを定義
		llvm::Constant *putsFunc = m.getOrInsertFunction("puts", putsType);

		// putsを呼び出す
		builder.CreateCall(putsFunc, func_header);
		builder.CreateCall(putsFunc, func_name);
		
		errs() << "END: " << f.getName() << "!\n";
		break;
	}
	}
	return true;
    }
};

char LoggerRegister::ID = 0;

static void registerLoggerPass(const PassManagerBuilder &,legacy::PassManagerBase &PM) {
    PM.add(new LoggerRegister());
}
static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible, registerLoggerPass);


