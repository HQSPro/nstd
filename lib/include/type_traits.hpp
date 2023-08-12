
#ifndef ____NSTD_TYPE_TRAITS_HPP__
#define ____NSTD_TYPE_TRAITS_HPP__

#include <type_traits>

#if __cplusplus >= 201703L  // c++17
#define __NSTD_VALUE_HAS_IS_INTEGRAL_V
#define __NSTD_VALUE_HAS_IS_POINTER_V
#define __NSTD_VALUE_HAS_IS_BASE_OF_V
#define __NSTD_VALUE_HAS_IS_SAME_V
#endif

#if __cplusplus >= 201402L  // c++14
#define __NSTD_TYPE_HAS_ENABLE_IF_T
#define __NSTD_TYPE_HAS_ADD_LVALUE_REFERENCE_T
#define __NSTD_TYPE_HAS_ADD_RVALUE_REFERENCE_T
#define __NSTD_TYPE_HAS_ADD_CONST_T
#define __NSTD_TYPE_HAS_ADD_POINTER_T
#define __NSTD_TYPE_HAS_REMOVE_POINTER_T
#define __NSTD_TYPE_HAS_DECAY_T
#define __NSTD_TYPE_HAS_REMOVE_REFERENCE_T
#endif

namespace nstd {

#ifdef __NSTD_VALUE_HAS_IS_INTEGRAL_V
using std::is_integral_v;
#else
template <typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;
#endif

#ifdef __NSTD_TYPE_HAS_ENABLE_IF_T
using std::is_enable_t;
#else
template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
#endif

#ifdef __NSTD_TYPE_HAS_ADD_LVALUE_REFERENCE_T
using std::add_lvalue_reference_t;
#else
template <class T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;
#endif

#ifdef __NSTD_TYPE_HAS_ADD_LVALUE_REFERENCE_T
using std::add_rvalue_reference_t;
#else
template <class T>
using add_rvalue_reference_t = typename std::add_lvalue_reference<T>::type;
#endif

#ifdef __NSTD_TYPE_HAS_ADD_CONST_T
using std::add_const_t;
#else
template <class T>
using add_const_t = typename std::add_const<T>::type;
#endif

#ifdef __NSTD_TYPE_HAS_ADD_POINTER_T
using std::add_pointer_t;
#else
template <class T>
using add_pointer_t = typename std::add_pointer<T>::type;
#endif

#ifdef __NSTD_TYPE_HAS_REMOVE_POINTER_T
using std::remove_pointer_t;
#else
template <class T>
using remove_pointer_t = typename remove_pointer<T>::type;
#endif

#ifdef __NSTD_VALUE_HAS_IS_POINTER_V
using std::is_pointer_v;
#else
template <class T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;
#endif

#ifdef __NSTD_VALUE_HAS_IS_BASE_OF_V
using std::is_base_of_v;
#else
template <class Base, class Derived>
inline constexpr bool is_base_of_v = std::is_base_of<Base, Derived>::value;
#endif

#ifdef __NSTD_VALUE_HAS_IS_SAME_V
using std::is_same_v;
#else
template <class T, class U>
inline constexpr bool is_same_v = is_same<T, U>::value;
#endif

#ifdef __NSTD_TYPE_HAS_DECAY_T
using std::decay_t;
#else
template <class T>
using decay_t = typename decay<T>::type;
#endif

#ifdef __NSTD_TYPE_HAS_REMOVE_REFERENCE_T
using std::remove_reference_t;
#else
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
#endif

}  // namespace nstd

#endif
