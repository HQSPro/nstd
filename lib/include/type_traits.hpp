
#ifndef __NSTD_TYPE_TRAITS_HPP__
#define __NSTD_TYPE_TRAITS_HPP__

#include <type_traits>

#if __cplusplus >= 201703L  // c++17
#define NSTD_VALUE_HAS_IS_INTEGRAL_V
#endif

#if __cplusplus >= 201402L  // c++14
#define NSTD_TYPE_HAS_ENABLE_IF_T
#endif

namespace nstd {

#ifdef NSTD_VALUE_HAS_IS_INTEGRAL_V
using std::is_integral_v;
#else
template <typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;
#endif

#ifdef NSTD_TYPE_HAS_ENABLE_IF_T
using std::is_enable_t;
#else
template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
#endif

}  // namespace nstd

#endif
