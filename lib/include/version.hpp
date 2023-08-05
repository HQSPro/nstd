
#ifndef __NSTD_REQUIRETS_HPP__
#define __NSTD_REQUIRETS_HPP__

#if __cplusplus >= 202002L // c++ 202002 certainly has <version>
#define NSTD_INCLUDE_HAS_VERSION
#elif(defined(__has_include) && __has_include(<version>))
#define NSTD_INCLUDE_HAS_VERSION
#endif

#ifdef NSTD_INCLUDE_HAS_VERSION
#include <version>
#endif

namespace nstd {
}

#endif
