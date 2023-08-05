#ifndef __NSTD_BUILTIN_HPP__
#define __NSTD_BUILTIN_HPP__

#include <cstddef>

#ifdef __has_builtin
#if __has_builtin(__type_pack_element) && !(defined(__ICC))
#define NSTD_BUILTIN_HAS_TYPE_PACK_ELEMENT
#endif
#endif

#ifndef NSTD_BUILTIN_HAS_TYPE_PACK_ELEMENT
#include "utility.hpp"
#include "type_traits.hpp"
#endif

namespace nstd {
#ifdef NSTD_BUILTIN_HAS_TYPE_PACK_ELEMENT
template <std::size_t I, typename... Ts>
using type_pack_element_t = __type_pack_element<I, Ts...>;
#else
namespace _internal0_impl0_builtin {
    template <std::size_t I, typename T>
    struct indexed_type : std::integral_constant<std::size_t, I> {
        using type = T;
    };
    template <std::size_t I, typename... Ts>
    struct type_pack_element_impl {
    private:
        template <typename>
        struct set;
        template <std::size_t... Is>
        struct set<index_sequence<Is...>> : indexed_type<Is, Ts>... {};
        template <typename T>
        inline static std::enable_if<true, T> impl(indexed_type<I, T>);
        inline static std::enable_if<false> impl(...);

    public:
        using type = decltype(impl(set<nstd::make_index_sequence<sizeof...(Ts)>>{}));
    };
}  // namespace _internal0_impl0_builtin

template <std::size_t I, typename... Ts>
using type_pack_element = typename _internal0_impl0_builtin::type_pack_element_impl<I, Ts...>::type;
template <std::size_t I, typename... Ts>
using type_pack_element_t = typename type_pack_element<I, Ts...>::type;
#endif
}  // namespace nstd

#endif
