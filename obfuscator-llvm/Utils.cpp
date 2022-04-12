#include "Utils.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include <sstream>

using namespace llvm;

// Shamefully borrowed from ../Scalar/RegToMem.cpp :(
bool valueEscapes(Instruction *Inst) {
  BasicBlock *BB = Inst->getParent();
  for (Value::use_iterator UI = Inst->use_begin(), E = Inst->use_end(); UI != E;
       ++UI) {
    Instruction *I = cast<Instruction>(*UI);
    if (I->getParent() != BB || isa<PHINode>(I)) {
      return true;
    }
  }
  return false;
}

void fixStack(Function *f) {
  // Try to remove phi node and demote reg to stack
  std::vector<PHINode *> tmpPhi;
  std::vector<Instruction *> tmpReg;
  BasicBlock *bbEntry = &*f->begin();

  do {
    tmpPhi.clear();
    tmpReg.clear();

    for (Function::iterator i = f->begin(); i != f->end(); ++i) {

      for (BasicBlock::iterator j = i->begin(); j != i->end(); ++j) {

        if (isa<PHINode>(j)) {
          PHINode *phi = cast<PHINode>(j);
          tmpPhi.push_back(phi);
          continue;
        }
        if (!(isa<AllocaInst>(j) && j->getParent() == bbEntry) &&
            (valueEscapes(&*j) || j->isUsedOutsideOfBlock(&*i))) {
          tmpReg.push_back(&*j);
          continue;
        }
      }
    }
    for (unsigned int i = 0; i != tmpReg.size(); ++i) {
      DemoteRegToStack(*tmpReg.at(i), f->begin()->getTerminator());
    }

    for (unsigned int i = 0; i != tmpPhi.size(); ++i) {
      DemotePHIToStack(tmpPhi.at(i), f->begin()->getTerminator());
    }

  } while (tmpReg.size() != 0 || tmpPhi.size() != 0);
}

StringRef readAnnotate(Function *f, StringRef attr) {
  // Get annotation variable
  auto *glob = f->getParent()->getGlobalVariable("llvm.global.annotations");

  if (glob == NULL) return "";

  // Get the array
  auto *ca = dyn_cast<ConstantArray>(glob->getInitializer());
  if (!ca) return "";

  for (unsigned i = 0; i < ca->getNumOperands(); ++i) {
    // Get the struct
    auto *structAn = dyn_cast<ConstantStruct>(ca->getOperand(i));
    if (!structAn) continue;

    auto *expr = dyn_cast<ConstantExpr>(structAn->getOperand(0));
    if (!expr) continue;

    // If it's a bitcast we can check if the annotation is concerning
    // the current function
    if (expr->getOpcode() != Instruction::BitCast ||
        expr->getOperand(0) != f) continue;

    auto *note = cast<ConstantExpr>(structAn->getOperand(1));
    // If it's a GetElementPtr, that means we found the variable
    // containing the annotations
    if (note->getOpcode() != Instruction::GetElementPtr) continue;

    auto *annoteStr = dyn_cast<GlobalVariable>(note->getOperand(0));
    if (!annoteStr) continue;

    auto *data = dyn_cast<ConstantDataSequential>(annoteStr->getInitializer());
    if (!data) continue;

    if (!data->isString()) continue;

    auto attr_pos = static_cast<std::size_t>(-1);
    auto attribute = data->getAsString();
    while (true){
      if (attr_pos != std::string::npos)
        attr_pos = attribute.find(attr, attr_pos + attr.size());
      else
        attr_pos = attribute.find(attr);
      if (attr_pos == std::string::npos) break;
      if (attr_pos != 0 && !std::isspace(attribute[attr_pos - 1])) continue;
      if (attr_pos + attr.size() < attribute.size() &&
          !std::isspace(attribute[attr_pos + attr.size()]) &&
          attribute[attr_pos + attr.size()] != '=' &&
          attribute[attr_pos + attr.size()] != '\0')
        continue;

      auto assign = attribute.find('=');
      if (assign != std::string::npos){
        auto value = assign + 1;
        while (std::isspace(attribute[value]))
          ++value;
        auto end = value + 1;
        while (end < attribute.size() && !std::isspace(attribute[end]) && attribute[end] != '\0')
          ++end;
        return attribute.substr(value, end - value);
     } else
        return "1";
    }
  }

  return "";
}

bool toObfuscate(bool flag, Function *f, StringRef attribute) {
  auto attrNo = "no" + attribute.str();

  // Check if declaration
  if (f->isDeclaration()) {
    return false;
  }

  // Check external linkage
  if (f->hasAvailableExternallyLinkage() != 0) {
    return false;
  }

  // We have to check the nofla flag first
  // Because .find("fla") is true for a string like "fla" or
  // "nofla"
  if (!readAnnotate(f, attrNo).empty()) {
    return false;
  }

  // If fla annotations
  if (!readAnnotate(f, attribute).empty()) {
    return true;
  }

  // If fla flag is set
  if (flag == true) {
    /* Check if the number of applications is correct
    if (!((Percentage > 0) && (Percentage <= 100))) {
      LLVMContext &ctx = llvm::getGlobalContext();
      ctx.emitError(Twine("Flattening application function\
              percentage -perFLA=x must be 0 < x <= 100"));
    }
    // Check name
    else if (func.size() != 0 && func.find(f->getName()) != std::string::npos) {
      return true;
    }

    if ((((int)llvm::cryptoutils->get_range(100))) < Percentage) {
      return true;
    }
    */
    return true;
  }

  return false;
}
