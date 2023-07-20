
#ifndef __NSTD_TYPE_MAGIC_HPP__
#define __NSTD_TYPE_MAGIC_HPP__

#include <type_traits>

namespace nstd {

template <int N>
struct Int : Int<N - 1>
{
    static constexpr int value = N;
};

template <>
struct Int<0>
{
    static constexpr int value = 0;
};

template <typename T>
static constexpr bool is_pure_type = !(std::is_pointer<T>::value || std::is_reference<T>::value ||
                                       std::is_const<T>::value || std::is_volatile<T>::value);

template <typename T>
using pure_type_t = typename std::remove_reference<
    typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::type;

} // namespace nstd

#endif
