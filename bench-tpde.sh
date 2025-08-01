#!/bin/bash
set -e

export CC="$(pwd)/bin/clang"
export CXX="$(pwd)/bin/clang++"
export CFLAGS="-Xclang -load -Xclang $(pwd)/lib/tpde.so"
export LDFLAGS="-fuse-ld=lld -B$(pwd)/bin"

export CCACHE_DISABLE=1

rm -rf bench_builtin*
for i in {1..2}; do
  out_dir="bench_builtin${i}"
  mkdir $out_dir
  echo "Configuring.. $out_dir/cmake.log"
  cmake -GNinja \
        -S "$(pwd)/../llvm" \
        -B $out_dir \
        -DCMAKE_EXE_LINKER_FLAGS="$LDFLAGS" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DLLVM_ENABLE_LIBCXX=On > $out_dir/cmake.log 2>&1
  echo "Building.. $out_dir/ninja.log"
  time ninja -C $out_dir llc > $out_dir/ninja.log
  echo ""
done

rm -rf bench_shlib*
for i in {1..2}; do
  out_dir="bench_shlib${i}"
  mkdir $out_dir
  echo "Configuring.. $out_dir/cmake.log"
  cmake -GNinja \
        -S "$(pwd)/../llvm" \
        -B $out_dir \
        -DCMAKE_C_FLAGS="$CFLAGS" \
        -DCMAKE_CXX_FLAGS="$CFLAGS" \
        -DCMAKE_EXE_LINKER_FLAGS="$LDFLAGS" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DLLVM_ENABLE_LIBCXX=On > $out_dir/cmake.log 2>&1
  echo "Building.. $out_dir/ninja.log"
  time ninja -C $out_dir llc > $out_dir/ninja.log
  echo ""
done
