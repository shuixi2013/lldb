// Test that we return complete results when only a part of the binary is built
// with an index.

// REQUIRES: lld

// RUN: clang %s -g -c -emit-llvm -o - --target=x86_64-pc-linux -DONE | \
// RUN:   llc -accel-tables=Dwarf -filetype=obj -o %t-1.o
// RUN: clang %s -g -c -emit-llvm -o - --target=x86_64-pc-linux -DTWO | \
// RUN:   llc -accel-tables=Disable -filetype=obj -o %t-2.o
// RUN: ld.lld %t-1.o %t-2.o -o %t
// RUN: lldb-test symbols --find=variable --name=foo  %t | FileCheck %s

// CHECK: Found 2 variables:
#ifdef ONE
namespace one {
int foo;
// CHECK-DAG: name = "foo", {{.*}} decl = dwarf5-partial-index.cpp:[[@LINE-1]]
} // namespace one
extern "C" void _start() {}
#else
namespace two {
int foo;
// CHECK-DAG: name = "foo", {{.*}} decl = dwarf5-partial-index.cpp:[[@LINE-1]]
} // namespace two
#endif
