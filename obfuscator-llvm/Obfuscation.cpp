#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "Substitution.h"
#include "Flattening.h"
#include "SplitBasicBlocks.h"
#include "BogusControlFlow.h"

// Flags for obfuscation
static cl::opt<bool> Flattening("fla", cl::init(false), cl::desc("Enable the flattening pass"));
static cl::opt<bool> BogusControlFlow("bcf", cl::init(false), cl::desc("Enable bogus control flow"));
static cl::opt<bool> Substitution("sub", cl::init(false), cl::desc("Enable instruction substitutions"));
static cl::opt<std::string> AesSeed("aesSeed", cl::init(""), cl::desc("seed for the AES-CTR PRNG"));
static cl::opt<bool> Split("spli", cl::init(false), cl::desc("Enable basic block splitting"));

static void loadPass(const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {
  // Initialization of the global cryptographically
  // secure pseudo-random generator
  if(!AesSeed.empty()) {
    if(!llvm::cryptoutils->prng_seed(AesSeed.c_str())) {
      exit(1);
    }
  }
  PM.add(createSplitBasicBlock(Split));
  PM.add(createBogus(BogusControlFlow));
  PM.add(createFlattening(Flattening));
  PM.add(createSubstitution(Substitution));
}

// These constructors add our pass to a list of global extensions.
static RegisterStandardPasses clangtoolLoader_Ox(PassManagerBuilder::EP_OptimizerLast, loadPass);
static RegisterStandardPasses clangtoolLoader_O0(PassManagerBuilder::EP_EnabledOnOptLevel0, loadPass);
// Note: The location EP_OptimizerLast places this pass at the end of the list
// of *optimizations*. That means on -O0, it does not get run.
//
// In general, adding your pass twice will cause it to run twice, but in this
// particular case, the two are disjoint (EP_EnabledOnOptLevel0 only runs if
// you're in -O0, and EP_OptimizerLast only runs if you're not). You can check
// include/llvm/Transforms/IPO/PassManagerBuilder.h header and
// lib/Transforms/IPO/PassManagerBuilder.cpp file for the exact behavior.
