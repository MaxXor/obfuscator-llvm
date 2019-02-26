#!/bin/sh

cd test
echo "====> Generating LLVM IR..."
clang-7 -S -emit-llvm test.c
echo "====> Obfuscating LLVM IR..."
clang-7 -Xclang -load -Xclang ../build/obfuscator-llvm/ObfuscatorLLVM.so -mllvm -bcf -mllvm -bcf_loop=1 -mllvm -sub -mllvm -sub_loop=1 -mllvm -fla ./test.ll -S -emit-llvm -o test_obfuscated.ll
echo "====> Compiling LLVM IR..."
clang-7 -flto=full -march=native -O3 -Wl,-strip-all -o test test_obfuscated.ll
echo "====> Running final executable..."
./test
