
#ifndef __NSTD_UTILITY_HPP__
#define __NSTD_UTILITY_HPP__

#include <utility>

#ifdef __cpp_lib_integer_sequence
#define NSTD_LIB_HAS_INTERGER_SEQUENCE
#else
#include <cstddef>
#include "type_traits.hpp"
#endif

#if __cplusplus >= 201703L  // c++17
#define NSTD_TYPE_HAS_IN_PLACE_T
#endif

namespace nstd {

#ifdef NSTD_TYPE_HAS_IN_PLACE_T
using std::in_place;
using std::in_place_index;
using std::in_place_index_t;
using std::in_place_t;
using std::in_place_type;
using std::in_place_type_t;
#else
struct in_place_t {
    explicit in_place_t() = default;
};
inline constexpr in_place_t in_place {}
template <class T>
struct in_place_type_t {
    explicit in_place_type_t() = default;
};
template <class T>
inline constexpr in_place_type_t<T> in_place_type{};
template <std::size_t I>
struct in_place_index_t {
    explicit in_place_index_t() = default;
};
template <std::size_t I>
inline constexpr in_place_index_t<I> in_place_index{};
#endif

#ifdef NSTD_LIB_HAS_INTERGER_SEQUENCE
using std::index_sequence;
using std::index_sequence_for;
using std::integer_sequence;
using std::make_index_sequence;
using std::make_integer_sequence;
#else
namespace _internal0_impl0_utility {
    template <typename T>
    struct identity {
        using type = T;
    };

    template <typename T, T... Ints>
    struct integer_sequence {
        static_assert(nstd::is_integral_v<T>);
        using value_type = T;
        static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
    };
    template <std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;
    template <typename T, std::size_t N>
    struct make_integer_sequence_impl;
    template <typename T, T N>
    using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

    template <typename T, typename Lhs, typename Rhs>
    struct make_integer_sequence_concat;

    template <typename T, T... Lhs, T... Rhs>
    struct make_integer_sequence_concat<T, integer_sequence<T, Lhs...>, integer_sequence<T, Rhs...>>
        : identity<integer_sequence<T, Lhs..., (sizeof...(Lhs) + Rhs)...>> {};

    template <typename T, std::size_t N>
    struct make_integer_sequence_impl : make_integer_sequence_concat<T, make_integer_sequence<T, N / 2>, make_integer_sequence<T, N - (N / 2)>> {};
    template <typename T>
    struct make_integer_sequence_impl<T, 0> : identity<integer_sequence<T>> {};
    template <typename T>
    struct make_integer_sequence_impl<T, 1> : identity<integer_sequence<T, 0>> {};

    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template <typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;
}  // namespace _internal0_impl0_utility
namespace _internal0_impl1_utility {

    template <typename T, T... Ints>
    struct integer_sequence {
        static_assert(nstd::is_integral_v<T>);
        using value_type = T;
        static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
        template <T Next>
        using append = integer_sequence<T, Ints..., Next>;
    };
    template <std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;
    namespace _internal1_impl0_utility {

        template <typename T, std::size_t N>
        struct make_integer_sequence {
            using type = typename make_integer_sequence<T, N - 1>::type::template append<N>;
        };

        template <typename T>
        struct make_integer_sequence<T, 0> {
            using type = integer_sequence<T, 0>;
        };

    }  // namespace _internal1_impl0_utility

    template <typename T, std::size_t N>
    using make_integer_sequence = typename _internal1_impl0_utility::make_integer_sequence<T, N - 1>::type;

    template <std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template <typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;
}  // namespace _internal0_impl1_utility
using _internal0_impl0_utility::index_sequence;
using _internal0_impl0_utility::index_sequence_for;
using _internal0_impl0_utility::integer_sequence;
using _internal0_impl0_utility::make_index_sequence;
using _internal0_impl0_utility::make_integer_sequence;
#endif
}  // namespace nstd

#endif
