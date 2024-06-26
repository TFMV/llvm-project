//===--- clock_gettime linux implementation ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/__support/time/linux/clock_gettime.h"
#include "src/__support/OSUtil/syscall.h"
#include <sys/syscall.h>
namespace LIBC_NAMESPACE {
namespace internal {
ErrorOr<int> clock_gettime(clockid_t clockid, timespec *ts) {
#if SYS_clock_gettime
  int ret = LIBC_NAMESPACE::syscall_impl<int>(SYS_clock_gettime,
                                              static_cast<long>(clockid),
                                              reinterpret_cast<long>(ts));
#elif defined(SYS_clock_gettime64)
  static_assert(
      sizeof(time_t) == sizeof(int64_t),
      "SYS_clock_gettime64 requires struct timespec with 64-bit members.");
  int ret = LIBC_NAMESPACE::syscall_impl<int>(SYS_clock_gettime64,
                                              static_cast<long>(clockid),
                                              reinterpret_cast<long>(ts));
#else
#error "SYS_clock_gettime and SYS_clock_gettime64 syscalls not available."
#endif
  if (ret < 0)
    return Error(-ret);
  return ret;
}

} // namespace internal
} // namespace LIBC_NAMESPACE
