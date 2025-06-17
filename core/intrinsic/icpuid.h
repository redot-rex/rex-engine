/**************************************************************************/
/*  icpuid.cpp                                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             REDOT ENGINE                               */
/*                        https://redotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2024-present Redot Engine contributors                   */
/*                                          (see REDOT_AUTHORS.md)        */
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef ICPUID_H
#define ICPUID_H

#include <cstdint>

#include "intrin_util.h"

// just a fair warning, this code is extremely architectually, compiler, and
// operating system dependent

#if defined(__x86_64__) || defined(_M_X64)

#define ARCH_TYPE ARCH_X64

#if defined(__GNUC__) || defined(__clang__)
#include <cpuid.h>
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#else
#error "compiler not supported"
#endif

#elif defined(__aarch64__) || defined(_M_ARM64)
// only using neon
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#define ARM_NEON 1
#else
#define ARM_NEON 0
#endif
#define ARCH_TYPE ARCH_ARM

#endif

namespace intrin {

enum arch_intrin {
  ARCH_X64 = 0,
  ARCH_ARM = 1,
};

enum x64_intrin {
  X64_NONE = 0,
  X64_MMX = 1 << 0,
  X64_SSE = 1 << 1,
  X64_SSE2 = 1 << 2,
  X64_SSE3 = 1 << 3,
  X64_SSSE3 = 1 << 4,
  X64_SSE41 = 1 << 5,
  X64_SSE42 = 1 << 6,
  X64_AVX = 1 << 7,
  X64_AVX2 = 1 << 8,
  X64_AVX512F = 1 << 9,       // Foundation
  X64_AVX512VL = 1 << 10,     // 128/256-bit ops
  X64_AVX512DQ = 1 << 11,     // Byte/Word ops
  X64_AVX512IFMA = 1 << 12,   // Fused Multiply Add
  X64_AVX512BITALG = 1 << 13, // Bitwise logic ops
  X64_FMA = 1 << 14,
  X64_BMI1 = 1 << 15,
  X64_BMI2 = 1 << 16,
  X64_SHA = 1 << 17,
  X64_AES = 1 << 18
};

enum arm_intrin {
  ARM_NONE = 0,
  ARM_NEON = 1,
};

// this data should always exist no matter what
struct intrin_check {
  arch_intrin architecture;
  uint32_t flags;
};

_FORCE_INLINE_ uint32_t check_mask(uint32_t value, uint32_t n) {
  return (value & (1 << n)) != 0;
}

#if defined(__x86_64__) || defined(_M_X64)

// to understand, the gnu version also checks if cpuid exists... since in msvc
// we cannot compile 32 bit it doesn't matter and we always will return 1
FORCE_INLINE int cpuid(int leaf, uint32_t *a, uint32_t *b, uint32_t *c,
                       uint32_t *d) {
#if defined(__GNUC__) || defined(__clang__)

  return __get_cpuid(leaf, a, b, c, d);

#elif defined(_MSC_VER)
  int cpu_info[4];
  __cpuid(cpu_info, leaf);

  *a = cpu_info[0];
  *b = cpu_info[1];
  *c = cpu_info[2];
  *d = cpu_info[3];

#endif
  return 1;
}

_FORCE_INLINE_ int cpuidex(int leaf, int subleaf, uint32_t *a, uint32_t *b,
                         uint32_t *c, uint32_t *d) {
#if defined(__GNUC__) || defined(__clang__)
  return __get_cpuid_count(leaf, subleaf, a, b, c, d);
#elif defined(_MSC_VER)
  int cpu_info[4];
  __cpuidex(cpu_info, leaf, subleaf);
  *a = cpu_info[0];
  *b = cpu_info[1];
  *c = cpu_info[2];
  *d = cpu_info[3];
#endif

  return 1;
}

// this should realistically only be ran once and only once
_FORCE_INLINE_ uint32_t intrin_support() {

  uint32_t intrin_support = 0;
  uint32_t a, b, c, d;

  // we first check for intrinsics support for avx instructions as these are the
  // most compiler dependent due to the xcr checks
  cpuid(1, &a, &b, &c, &d);

  // avx check
  uint32_t avxcheck = check_mask(c, 28);
  uint32_t osxsave = check_mask(c, 27);

  // a bit strangely defined but will describe how this works
  // first we check if avx exists, then we will set it to a new value
  // as opposed to changing teh value as to reuse variables to prevent
  // overbloated amount of variables

  [[likely]]
  if (avxcheck && osxsave) {
    uint64_t xcr0 = _xgetbv(0);
    avxcheck = (xcr0 & 0x6) == 0x6;
    intrin_support |= (check_mask(c, 28) && avxcheck) << 7; // avx1
  }

  [[likely]]
  if (cpuidex(7, 0, &a, &b, &c, &d) && avxcheck) {
    intrin_support |= check_mask(b, 5) << 8; // avx2
    intrin_support |=
        check_mask(b, 16)
        << 9; // avx512f all below will be represented by their suffix
    intrin_support |= check_mask(b, 31) << 10; // VL
    intrin_support |= check_mask(b, 17) << 11; // DQ
    intrin_support |= check_mask(b, 21) << 12; // IFMA
    intrin_support |= check_mask(c, 12) << 13; // BITALG

    intrin_support |= check_mask(b, 3) << 15;  // BMI
    intrin_support |= check_mask(b, 8) << 16;  // BMI2
    intrin_support |= check_mask(b, 29) << 17; // SHA
  }

  cpuid(1, &a, &b, &c, &d);

  // sse check
  intrin_support |= check_mask(d, 23);      // sse1
  intrin_support |= check_mask(d, 25) << 1; // ssse2
  intrin_support |= check_mask(d, 26) << 2; // nnx

  intrin_support |= check_mask(c, 0) << 3;  // sse3
  intrin_support |= check_mask(c, 9) << 4;  // ssse3
  intrin_support |= check_mask(c, 19) << 5; // sse41
  intrin_support |= check_mask(c, 20) << 6; // sse4.2

  intrin_support |= check_mask(c, 12) << 10; // fma
  intrin_support |= check_mask(c, 25) << 18; // aes
  return intrin_support;
} 

#elif defined(__aarch64__) || defined(_M_ARM64)


_FORCE_INLINE_ uint32_t intrin_support() {
  uint32_t intrin_support = ARM_NEON; // neon is always 100% confirmed so don't need anything else
  return intrin_support;
}
#endif

[[nodiscard]]
_FORCE_INLINE_ intrin_check intrin_compat() {
  intrin_check intrin_checker{};

  intrin_checker.architecture = ARCH_TYPE;
  intrin_checker.flags = intrin_support();
  return intrin_checker;
}

}; // namespace intrin

#endif
