
#ifndef __NSTD_TYPE_MAGIC_HPP__
#define __NSTD_TYPE_MAGIC_HPP__

namespace nstd{

template<typename T>
constexpr bool is_pure_type = !(std::is_pointer<T>::value || std::is_reference<T>::value || std::is_const<T>::value || std::is_volatile<T>::value);

template<typename T>
using pure_type_t = typename std::remove_reference<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::type;

}

#endif
