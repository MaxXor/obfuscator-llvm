#ifndef _OBFUSCATION_UTILS_H_
#define _OBFUSCATION_UTILS_H_

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Local.h" // For DemoteRegToStack and DemotePHIToStack
#include <string>

void fixStack(llvm::Function *f);
llvm::StringRef readAnnotate(llvm::Function* f, llvm::StringRef attr);
bool toObfuscate(bool flag, llvm::Function *f, llvm::StringRef attribute);

#endif
