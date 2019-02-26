# Obfuscator-LLVM

Obfuscator-LLVM is a project initiated in June 2010 by the information security group of the University of Applied Sciences and Arts Western Switzerland of Yverdon-les-Bains ([HEIG-VD](http://www.heig-vd.ch/international)). The aim of this project is to provide increased software security through code obfuscation and tamper-proofing. As the obfuscator currently mostly works at the Intermediate Representation (IR) level, the tool is compatible with all programming languages (C, C++, Objective-C, Ada and Fortran) and target platforms (x86, x86-64, PowerPC, PowerPC-64, ARM, Thumb, SPARC, Alpha, CellSPU, MIPS, MSP430, SystemZ, and XCore) currently supported by LLVM. The initially published source code and research paper can be found [here](https://github.com/obfuscator-llvm/obfuscator). This fork makes the obfuscator available as an extension to most available LLVM installations. It doesn't require any changes to existing LLVM installations, unlike the project published by the authors of Obfuscator-LLVM.

## Features

Here is a list of the different features that are currently available:

* [Instructions Substitution](https://github.com/obfuscator-llvm/obfuscator/wiki/Instructions-Substitution) `-mllvm -sub`
* [Bogus Control Flow](https://github.com/obfuscator-llvm/obfuscator/wiki/Bogus-Control-Flow) `-mllvm -bcf`
* [Control Flow Flattening](https://github.com/obfuscator-llvm/obfuscator/wiki/Control-Flow-Flattening) `-mllvm -fla`
* [Functions annotations](https://github.com/obfuscator-llvm/obfuscator/wiki/Functions-annotations)

## Building

To build this project it's required to have Clang 3.3+, LLVM 3.3+ and CMake 3.4+ installed. On Ubuntu 18.04+ you can use `sudo apt install clang-7 llvm-7-dev cmake` to install the required softwre. To compile the obfuscator simply run `./build.sh` or execute the following commands in this directory:

```bash
mkdir -p build
cd build
cmake ../
cmake --build .
```

## Usage

Adding the obfuscator to existing projects which use an LLVM compiler is very easy. For the Make (or rather Makefile) buildsystem simply adjust the small example below to fit your needs:

```makefile
OBFUSCATOR_FLAGS=-Xclang -load -Xclang /path/to/ObfuscatorLLVM.so

# ... your targets

tool: tool.c utils.c helper.c clangtool.so
    $(CC) $(CXXFLAGS) $(OBFUSCATOR_FLAGS) -O0 -o test test.c
```

You can also integrate the obfuscator to your existing CMake projects for any project that uses LLVM:

```cmake
target_compile_options(<name> PRIVATE -Xclang -load -Xclang /path/to/ObfuscatorLLVM.so)
```

## Testing

The [Himeno benchmark](http://accc.riken.jp/en/supercom/documents/himenobmt/) is used to test the effectiveness of the obfuscation techniques. To execute the obfuscator test run `./test.sh`. This will generate LLVM IR from `./test/test.c`, apply obfuscations on it and run the obfuscated program. The obfuscated LLVM IR can be found in `./test/test_obfuscated.ll` and the non-obfuscated LLVM IR in `./test/test.ll`.
