
#ifndef __NSTD_VARIANT_HPP__
#define __NSTD_VARIANT_HPP__

#include "version.hpp"
#ifdef __cpp_lib_variant
#define __NSTD_LIB_HAS_VARIANT
#endif
#if(defined(__cpp_lib_variant) && (__cpp_lib_variant >= 202102L))
#define __NSTD_LIB_HAS_VARIANT_VISIT
#endif

#ifdef __NSTD_LIB_HAS_VARIANT
#include <variant>
#endif

namespace nstd {
#ifdef __NSTD_LIB_HAS_VARIANT
using std::get;
using std::monostate;
using std::variant;
#else
// todo: define the variant class.
// todo: define the monostate class.
// todo: define the get algorithm.
#endif

#ifdef __NSTD_LIB_HAS_VARIANT_VISIT
using std::visit;
#else
// todo: define the std::visit for variant.
#endif

}  // namespace nstd

#endif
