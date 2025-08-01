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
        -DCMAKE_INSTALL_PREFIX="$(pwd)/build-install" \
        -DCMAKE_C_FLAGS="-Wno-unused-parameter" \
        -DLLVM_TARGETS_TO_BUILD=X86 \
        -DLLVM_ENABLE_PROJECTS="clang;lld" \
        -DLLVM_ENABLE_LIBCXX=On \
        -DLLVM_BUILD_LLVM_DYLIB=On \
        -DLLVM_LINK_LLVM_DYLIB=On \
        -DLLVM_USE_LINKER=lld \
        -DLLVM_EXTERNAL_PROJECTS=tpde-shlib \
        -DLLVM_EXTERNAL_TPDE_SHLIB_SOURCE_DIR=$(pwd)/tpde-shlib

> ninja -C build check-tpde-shlib
[0/1] Running the TPDE custom backend tests
-- Testing: 4 tests, 4 workers --
PASS: TPDE-SHLIB :: fallback.ll (1 of 4)
PASS: TPDE-SHLIB :: load.cpp (2 of 4)
PASS: TPDE-SHLIB :: run.cpp (3 of 4)
PASS: TPDE-SHLIB :: custom-llvm.cpp (4 of 4)

Testing Time: 0.24s

Total Discovered Tests: 4
  Passed: 4 (100.00%)
```

## Benchmark

```
> ninja -C build install-tpde-bench
> cd build-install
> ./bench-tpde.sh
Configuring.. bench_builtin1/cmake.log
Building.. bench_builtin1/ninja.log

real  15m54,819s
user 238m43,687s
sys    6m49,652s

Configuring.. bench_builtin2/cmake.log
Building.. bench_builtin2/ninja.log

real  15m50,084s
user 238m11,806s
sys    6m51,175s

Configuring.. bench_shlib1/cmake.log
Building.. bench_shlib1/ninja.log

real  16m12,641s
user 241m21,990s
sys    6m41,188s

Configuring.. bench_shlib2/cmake.log
Building.. bench_shlib2/ninja.log

real  16m13,498s
user 241m17,639s
sys    6m43,465s
```

On my system TPDE is **1.11% slower** via shlib adapter than built-in in clang:
```
Builtin average: 14717s
  14323user + 409sys = 14732
  14291user + 411sys = 14702

Shlib average: 14881s
  14481user + 401sys = 14882
  14477user + 403sys = 14880
```
