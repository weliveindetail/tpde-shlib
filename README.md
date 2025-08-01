# tpde-shlib

Experimental custom backend adapter for TPDE in llvm-project. Current state works with this patch on top of release/20.x: https://github.com/weliveindetail/llvm-project/commit/custom-backend-shlib-dev

On the clang command-line, the custom backend is injected via `-Xclang -load -Xclang tpde.so`. It creates a codegen pipeline with a single pass. No other MIR passes are running. Comment out the following line in TPDEPass.cpp to fail the `run.cpp` test:

```cpp
    // Compilation successful: write out buffer
    OS.pwrite(reinterpret_cast<char *>(Elf.data()), Elf.size(), 0);
```

## Build and test

```
> git clone https://github.com/weliveindetail/llvm-project --branch custom-backend-dev --single-branch
> cd llvm-project
> git clone https://github.com/weliveindetail/tpde-shlib
> git -C tpde-shlib submodule update --init --recursive
> CC=clang-20 CXX=clang++-20 cmake -Bbuild -Sllvm  -GNinja \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_C_FLAGS="-Wno-unused-parameter" \
        -DLLVM_TARGETS_TO_BUILD=X86 \
        -DLLVM_ENABLE_PROJECTS=clang \
        -DLLVM_ENABLE_LIBCXX=On \
        -DLLVM_BUILD_LLVM_DYLIB=On \
        -DLLVM_LINK_LLVM_DYLIB=On \
        -DLLVM_USE_LINKER=lld \
        -DLLVM_EXTERNAL_PROJECTS=tpde-shlib \
        -DLLVM_EXTERNAL_TPDE_SHLIB_SOURCE_DIR=$(pwd)/tpde-shlib

> ninja -C build check-tpde-shlib
[0/1] Running the TPDE custom backend tests
-- Testing: 2 tests, 2 workers --
PASS: TPDE-SHLIB :: load.cpp (1 of 2)
PASS: TPDE-SHLIB :: run.cpp (2 of 2)

Testing Time: 0.15s

Total Discovered Tests: 2
  Passed: 2 (100.00%)
```
