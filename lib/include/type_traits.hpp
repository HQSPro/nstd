
#ifndef __NSTD_TYPE_TRAITS_HPP__
#define __NSTD_TYPE_TRAITS_HPP__

#include <type_traits>

#if __cplusplus >= 201703L  // c++17
#define NSTD_VALUE_HAS_IS_INTEGRAL_V
#define NSTD_VALUE_HAS_IS_POINTER_V
#endif

#if __cplusplus >= 201402L  // c++14
#define NSTD_TYPE_HAS_ENABLE_IF_T
#define NSTD_TYPE_HAS_ADD_LVALUE_REFERENCE_T
#define NSTD_TYPE_HAS_ADD_RVALUE_REFERENCE_T
#define NSTD_TYPE_HAS_ADD_CONST_T
#define NSTD_TYPE_HAS_ADD_POINTER_T
#define NSTD_TYPE_HAS_REMOVE_POINTER_T
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

#ifdef NSTD_TYPE_HAS_ADD_LVALUE_REFERENCE_T
using std::add_lvalue_reference_t;
#else
template <class T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;
#endif

#ifdef NSTD_TYPE_HAS_ADD_LVALUE_REFERENCE_T
using std::add_rvalue_reference_t;
#else
template <class T>
using add_rvalue_reference_t = typename std::add_lvalue_reference<T>::type;
#endif

#ifdef NSTD_TYPE_HAS_ADD_CONST_T
using std::add_const_t;
#else
template <class T>
using add_const_t = typename std::add_const<T>::type;
#endif

#ifdef NSTD_TYPE_HAS_ADD_POINTER_T
using std::add_pointer_t;
#else
template <class T>
using add_pointer_t = typename std::add_pointer<T>::type;
#endif

#ifdef NSTD_TYPE_HAS_REMOVE_POINTER_T
using std::remove_pointer_t;
#else
template <class T>
using remove_pointer_t = typename remove_pointer<T>::type;
#endif

#ifdef NSTD_VALUE_HAS_IS_POINTER_V
using std::is_pointer_v;
#else
template <class T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;
#endif

}  // namespace nstd

#endif
