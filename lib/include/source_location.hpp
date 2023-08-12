#ifndef __NSTD_SOURCE_LOCATION_HPP__
#define __NSTD_SOURCE_LOCATION_HPP__

#include "version.hpp"
#include <string>

#if(__cplusplus >= 202002L || defined(__cpp_lib_source_location))  // c++20
#define __NSTD_LIB_HAS_SOURCE_LOCATION
#endif

#ifdef __NSTD_LIB_HAS_SOURCE_LOCATION
#include <source_location>
#endif

namespace nstd {
#ifdef __NSTD_LIB_HAS_SOURCE_LOCATION
#define __NSTD_FILE__ std::source_location::current().file_name()
#define __NSTD_LINE__ std::source_location::current().line()
#define __NSTD_FUNC__ std::source_location::current().function_name()
#else
#define __NSTD_FILE__ __FILE__
#define __NSTD_LINE__ __LINE__
#ifdef _MSC_VER
#define __NSTD_FUNC__ __FUNCSIG__
#elif defined(__GNUC__)
#define __NSTD_FUNC__ __PRETTY_FUNCTION__
#else
#define __NSTD_FUNC__ __func__
#endif
#endif
}  // namespace nstd

#endif
