
#ifndef __NSTD_TYPE_MAGIC_HPP__
#define __NSTD_TYPE_MAGIC_HPP__

#include <utility>
#include <type_traits>

#if __has_builtin(__type_pack_element) && !(defined(__ICC))
#define NSTD_TYPE_PACK_ELEMENT
#endif

namespace nstd {

template <std::size_t... Is>
using index_sequence = std::integer_sequence<std::size_t, Is...>;
template <std::size_t N>
using size_constant = std::integral_constant<std::size_t, N>;
template <std::size_t I, typename T>
struct indexed_type : size_constant<I>
{
    using type = T;
};

#ifdef NSTD_TYPE_PACK_ELEMENT
template <std::size_t I, typename... Ts>
using type_pack_element_t = __type_pack_element<I, Ts...>;
#else
template <std::size_t I, typename... Ts>
struct type_pack_element_impl
{
private:
    template <typename>
    struct set;
    template <std::size_t... Is>
    struct set<index_sequence<Is...>> : indexed_type<Is, Ts>...
    {
    };
    template <typename T>
    inline static std::enable_if<true, T> impl(indexed_type<I, T>);
    inline static std::enable_if<false> impl(...);

public:
    using type = decltype(impl(set<std::make_index_sequence<sizeof...(Ts)>>{}));
};
template <std::size_t I, typename... Ts>
using type_pack_element = typename type_pack_element_impl<I, Ts...>::type;
template <std::size_t I, typename... Ts>
using type_pack_element_t = typename type_pack_element<I, Ts...>::type;
#endif

template <int N>
struct Int : Int<N - 1>
{
    constexpr static int value = N;
};

template <>
struct Int<0>
{
    constexpr static int value = 0;
};

template <typename T>
constexpr static bool is_pure_type = !(std::is_pointer<T>::value || std::is_reference<T>::value ||
                                       std::is_const<T>::value || std::is_volatile<T>::value);

template <typename T>
using pure_type_t = typename std::remove_reference<
    typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::type;

} // namespace nstd

#endif
