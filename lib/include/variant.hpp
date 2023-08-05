
#ifndef __NSTD_VARIANT_HPP__
#define __NSTD_VARIANT_HPP__

#include "version.hpp"
#ifdef __cpp_lib_variant
#define NSTD_LIB_HAS_VARIANT __cpp_lib_variant
#endif

#ifdef NSTD_LIB_HAS_VARIANT
#include <variant>
#endif

namespace nstd {
#ifdef NSTD_LIB_HAS_VARIANT
using std::variant;
#else
// todo: define the variant class.
#endif

#if(defined(NSTD_LIB_HAS_VARIANT) && (NSTD_LIB_HAS_VARIANT < 202102L))
// todo: define the std::visit for variant.
#endif
} // namespace nstd

#endif
