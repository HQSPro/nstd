
#ifndef __NSTD_MATCH_HPP__
#define __NSTD_MATCH_HPP__

#include <assert.h>
#include <type_magic.hpp>

namespace nstd {

/* The first implementations of MATCH like rust.
 * This approach is implemented through macros which use switch actually.
 * Note: currently, only 30 branches are supported for matching and we don't support match guard.
 */

#define MATCH_PP_EMPTY
#define MATCH_CONCAT_IMPL(A, B) A##B
#define MATCH_CONCAT(A, B) MATCH_CONCAT_IMPL(A, B)

#define MATCH_PARENS_PROBE(...) _, 1
#define MATCH_PARENS_CONDITION(a, b, ...) b
#define MATCH_PARENS_CHECK(...) MATCH_PARENS_CONDITION(__VA_ARGS__, 0)
#define MATCH_PARENS_REMOVE_IMPL_0(...) __VA_ARGS__
#define MATCH_PARENS_REMOVE_IMPL_1_IMPL(...) __VA_ARGS__
#define MATCH_PARENS_REMOVE_IMPL_1(expr) MATCH_PARENS_REMOVE_IMPL_1_IMPL expr
#define MATCH_PARENS_REMOVE_IMPL(condition) MATCH_CONCAT(MATCH_PARENS_REMOVE_IMPL_, condition)
#define MATCH_PARENS_REMOVE(expr) \
    MATCH_PARENS_REMOVE_IMPL(MATCH_PARENS_CHECK(MATCH_PARENS_PROBE expr))(expr)

#define MATCH_DEFAULT_SYMBOL_ _, 1
#define MATCH_CASE_OR_DEFAULT_CONDITION(a, b, ...) b
#define MATCH_CASE_OR_DEFAULT_CHECK(...) MATCH_CASE_OR_DEFAULT_CONDITION(__VA_ARGS__, 0)
#define MATCH_CASE_GEN_IMPL_0(c) case c:
#define MATCH_CASE_GEN_IMPL_1(c) MATCH_PP_EMPTY
#define MATCH_CASE_GEN_IMPL(condition) MATCH_CONCAT(MATCH_CASE_GEN_IMPL_, condition)
#define MATCH_CASE_GEN(c) \
    MATCH_CASE_GEN_IMPL(MATCH_CASE_OR_DEFAULT_CHECK(MATCH_CONCAT(MATCH_DEFAULT_SYMBOL, c)))(c)
#define MATCH_CASE_EXPAND_IMPL(c0,  \
                               c1,  \
                               c2,  \
                               c3,  \
                               c4,  \
                               c5,  \
                               c6,  \
                               c7,  \
                               c8,  \
                               c9,  \
                               c10, \
                               c11, \
                               c12, \
                               c13, \
                               c14, \
                               c15, \
                               c16, \
                               c17, \
                               c18, \
                               c19, \
                               c20, \
                               c21, \
                               c22, \
                               c23, \
                               c24, \
                               c25, \
                               c26, \
                               c27, \
                               c28, \
                               c29, \
                               ...) \
    MATCH_CASE_GEN(c0)              \
    MATCH_CASE_GEN(c1)              \
    MATCH_CASE_GEN(c2)              \
    MATCH_CASE_GEN(c3)              \
    MATCH_CASE_GEN(c4)              \
    MATCH_CASE_GEN(c5)              \
    MATCH_CASE_GEN(c6)              \
    MATCH_CASE_GEN(c7)              \
    MATCH_CASE_GEN(c8)              \
    MATCH_CASE_GEN(c9)              \
    MATCH_CASE_GEN(c10)             \
    MATCH_CASE_GEN(c11)             \
    MATCH_CASE_GEN(c12)             \
    MATCH_CASE_GEN(c13)             \
    MATCH_CASE_GEN(c14)             \
    MATCH_CASE_GEN(c15)             \
    MATCH_CASE_GEN(c16)             \
    MATCH_CASE_GEN(c17)             \
    MATCH_CASE_GEN(c18)             \
    MATCH_CASE_GEN(c19)             \
    MATCH_CASE_GEN(c20)             \
    MATCH_CASE_GEN(c21)             \
    MATCH_CASE_GEN(c22)             \
    MATCH_CASE_GEN(c23)             \
    MATCH_CASE_GEN(c24)             \
    MATCH_CASE_GEN(c25)             \
    MATCH_CASE_GEN(c26)             \
    MATCH_CASE_GEN(c27)             \
    MATCH_CASE_GEN(c28)             \
    MATCH_CASE_GEN(c29)

#define MATCH_CASE_EXPAND(...)          \
    MATCH_CASE_EXPAND_IMPL(__VA_ARGS__, \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _,           \
                           _)

#define MATCH_CASE_OR_DEFAULT_IMPL_0(m, c, ...) \
    MATCH_CASE_EXPAND(MATCH_PARENS_REMOVE(c))   \
    return __VA_ARGS__();

#define MATCH_CASE_OR_DEFAULT_IMPL_1(m, c, ...) \
    default:                                    \
        return __VA_ARGS__();

#define MATCH_CASE_OR_DEFAULT_IMPL(condition) MATCH_CONCAT(MATCH_CASE_OR_DEFAULT_IMPL_, condition)

#define MATCH_CASE_FIRST_IMPL(c, ...) c
#define MATCH_CASE_FIRST(...) __VA_ARGS__
#define MATCH_CASE_PARSE_IMPL_0(...) MATCH_PP_EMPTY
#define MATCH_CASE_PARSE_IMPL_1_IMPL(m, c, ...)                                        \
    MATCH_CASE_OR_DEFAULT_IMPL(MATCH_CASE_OR_DEFAULT_CHECK(                            \
        MATCH_CONCAT(MATCH_DEFAULT_SYMBOL, MATCH_CASE_FIRST(MATCH_PARENS_REMOVE(c))))) \
    (m, c, __VA_ARGS__)
#define MATCH_CASE_PARSE_IMPL_1(m, c) MATCH_CASE_PARSE_IMPL_1_IMPL(m, c)
#define MATCH_CASE_PARSE_IMPL(condition) MATCH_CONCAT(MATCH_CASE_PARSE_IMPL_, condition)
#define MATCH_CASE_PARSE(m, c) \
    MATCH_CASE_PARSE_IMPL(MATCH_PARENS_CHECK(MATCH_PARENS_PROBE c))(m, MATCH_PARENS_REMOVE(c))

#define MATCH_CASES_PARSE_IMPL(m,   \
                               c0,  \
                               c1,  \
                               c2,  \
                               c3,  \
                               c4,  \
                               c5,  \
                               c6,  \
                               c7,  \
                               c8,  \
                               c9,  \
                               c10, \
                               c11, \
                               c12, \
                               c13, \
                               c14, \
                               c15, \
                               c16, \
                               c17, \
                               c18, \
                               c19, \
                               c20, \
                               c21, \
                               c22, \
                               c23, \
                               c24, \
                               c25, \
                               c26, \
                               c27, \
                               c28, \
                               c29, \
                               ...) \
    MATCH_CASE_PARSE(m, c0)         \
    MATCH_CASE_PARSE(m, c1)         \
    MATCH_CASE_PARSE(m, c2)         \
    MATCH_CASE_PARSE(m, c3)         \
    MATCH_CASE_PARSE(m, c4)         \
    MATCH_CASE_PARSE(m, c5)         \
    MATCH_CASE_PARSE(m, c6)         \
    MATCH_CASE_PARSE(m, c7)         \
    MATCH_CASE_PARSE(m, c8)         \
    MATCH_CASE_PARSE(m, c9)         \
    MATCH_CASE_PARSE(m, c10)        \
    MATCH_CASE_PARSE(m, c11)        \
    MATCH_CASE_PARSE(m, c12)        \
    MATCH_CASE_PARSE(m, c13)        \
    MATCH_CASE_PARSE(m, c14)        \
    MATCH_CASE_PARSE(m, c15)        \
    MATCH_CASE_PARSE(m, c16)        \
    MATCH_CASE_PARSE(m, c17)        \
    MATCH_CASE_PARSE(m, c18)        \
    MATCH_CASE_PARSE(m, c19)        \
    MATCH_CASE_PARSE(m, c20)        \
    MATCH_CASE_PARSE(m, c21)        \
    MATCH_CASE_PARSE(m, c22)        \
    MATCH_CASE_PARSE(m, c23)        \
    MATCH_CASE_PARSE(m, c24)        \
    MATCH_CASE_PARSE(m, c25)        \
    MATCH_CASE_PARSE(m, c26)        \
    MATCH_CASE_PARSE(m, c27)        \
    MATCH_CASE_PARSE(m, c28)        \
    MATCH_CASE_PARSE(m, c29)

#define MATCH_CASES_PARSE(m, ...)         \
    MATCH_CASES_PARSE_IMPL(m,             \
                           ##__VA_ARGS__, \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _,             \
                           _)

#define MATCH_IMPL(matchable, ...)                    \
    [&matchable]() -> decltype(auto) {                \
        switch(matchable)                             \
        {                                             \
            MATCH_CASES_PARSE(matchable, __VA_ARGS__) \
        }                                             \
    }()

#define MATCH(matchable, ...) MATCH_IMPL(MATCH_PARENS_REMOVE(matchable), __VA_ARGS__)

/* The second implementations of match like rust.
 * This approach is implemented through generic which can support a large number of branches.
 * Note: this approach must check every branch, then the execution efficiency is not as good as the
 * first approach.
 */
// template<typename Derived, typename = typename std::enable_if<nstd::is_pure_type<Derived>>::type>
// class ApplyMatchTrait
// {
//     public:
//     using type = Derived;
//     operator int() const
//     {
//         return static_cast<int>(*static_cast<Derived*>(this));
//     }
// };

// template<int I, typename OneType, typename Func>
// decltype(auto) match_single_branch(OneType&& v, Func&& func)
// {

// }

// template <typename MatchableType, typename ItemFunc, typename Func>
// decltype(auto) match_impl(MatchableType&& v, ItemFunc&& item_func)
// {

// }

// template <typename MatchableType, typename ItemFunc, typename... ItemFuncs>
// decltype(auto) match_impl(MatchableType&& v, ItemFunc&& item_func, ItemFuncs&&... item_funcs)
// {
//     static_cast<int>(v)
// }

// template <typename MatchableType, typename... ItemFuncs>
// decltype(auto) match(MatchableType&& v, ItemFuncs&&... item_funcs)
// {
//     return match_impl(v, item_funcs...);
// }
// template<typename MatchableType, typename... Funcs>
// decltype(auto) match(MatchableType* v, Funcs&&... funcs)
// {
//     if constexpr (
//         std::is_same<typename std::remove_cvref<MatchableType>::type, typename
//         ApplyMatchTrait<std::remove_cvref<MatchableType>::type>>
//     )
// }

} // namespace nstd
#endif
