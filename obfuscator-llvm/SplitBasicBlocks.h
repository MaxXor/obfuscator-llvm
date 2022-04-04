//===- FlatteningIncludes.h - Flattening Obfuscation pass------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains includes and defines for the split basicblock pass
//
//===----------------------------------------------------------------------===//
#ifndef _OBFUSCATION_SPLIT_H_
#define _OBFUSCATION_SPLIT_H_

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Transforms/IPO.h"
#include "CryptoUtils.h"
#include "Utils.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils/Local.h" // For DemoteRegToStack and DemotePHIToStack
#include "llvm/IR/PassManager.h" // New PassManager

namespace llvm {
Pass *createSplitBasicBlock();
class SplitBasicBlockPass : public PassInfoMixin<SplitBasicBlockPass>{ // New PassManager
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

  static bool isRequired() { return true; }
};
}

#endif
