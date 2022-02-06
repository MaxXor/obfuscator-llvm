#!/bin/sh

cd test
echo "====> Generating LLVM IR..."
clang -S -emit-llvm test.c
if [ $? != 0 ]; then
    printf "\033[0;31mFAILED!\n"
    exit 1
else
    printf "\033[0;32mPASS\033[0m\n"
fi
echo "====> Obfuscating LLVM IR..."
clang -Xclang -load -Xclang ../build/obfuscator-llvm/ObfuscatorLLVM.so -mllvm -bcf -mllvm -bcf_loop=1 -mllvm -sub -mllvm -sub_loop=1 -mllvm -fla ./test.ll -S -emit-llvm -o test_obfuscated.ll
if [ $? != 0 ]; then
    printf "\033[0;31mFAILED!\n"
    exit 1
else
    printf "\033[0;32mPASS\033[0m\n"
fi
echo "====> Compiling LLVM IR..."
clang -flto=full -march=native -O3 -Wl,-strip-all -o test test_obfuscated.ll
if [ $? != 0 ]; then
    printf "\033[0;31mFAILED!\n"
    exit 1
else
    printf "\033[0;32mPASS\033[0m\n"
fi
echo "====> Running final executable..."
./test
if [ $? != 0 ]; then
    printf "\033[0;31mFAILED!\n"
    exit 1
else
    printf "\033[0;32mPASS\033[0m\n"
fi
