
#ifndef __NSTD_MATCH_HPP__
#define __NSTD_MATCH_HPP__

#include "type_traits"
#include "variant"

namespace nstd {

/* The first implementations of match like rust.
 * This approach is implemented through generic which can support a large number of branches.
 */

// helper type for the match
template <class... Ts>
struct MatchHelper : Ts... {
    using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
MatchHelper(Ts...) -> MatchHelper<Ts...>;

template <typename... Vs, typename... Funcs>
constexpr decltype(auto) match(nstd::variant<Vs...>&& v, Funcs&&... funcs)
{
    return nstd::visit(MatchHelper{funcs...}, std::forward<nstd::variant<Vs...>>(v));
}

template <typename... Vs, typename... Funcs>
constexpr decltype(auto) match(nstd::variant<Vs...>& v, Funcs&&... funcs)
{
    return nstd::visit(MatchHelper{funcs...}, std::forward<nstd::variant<Vs...>>(v));
}

template <typename... Vs, typename... Funcs>
constexpr decltype(auto) match(const nstd::variant<Vs...>& v, Funcs&&... funcs)
{
    return nstd::visit(MatchHelper{funcs...}, std::forward<nstd::variant<Vs...>>(v));
}

/* The second implementations of MATCH like rust.
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
#define MATCH_PARENS_REMOVE(expr) MATCH_PARENS_REMOVE_IMPL(MATCH_PARENS_CHECK(MATCH_PARENS_PROBE expr))(expr)

#define MATCH_DEFAULT_SYMBOL_ _, 1
#define MATCH_CASE_OR_DEFAULT_CONDITION(a, b, ...) b
#define MATCH_CASE_OR_DEFAULT_CHECK(...) MATCH_CASE_OR_DEFAULT_CONDITION(__VA_ARGS__, 0)
#define MATCH_CASE_GEN_IMPL_0(m, f, c) \
    case c:                            \
        return MATCH_PARENS_REMOVE(f)(std::get<static_cast<std::size_t>(c)>(m));
#define MATCH_CASE_GEN_IMPL_1(m, f, c) MATCH_PP_EMPTY
#define MATCH_CASE_GEN_IMPL(condition) MATCH_CONCAT(MATCH_CASE_GEN_IMPL_, condition)
#define MATCH_CASE_GEN(m, f, c) MATCH_CASE_GEN_IMPL(MATCH_CASE_OR_DEFAULT_CHECK(MATCH_CONCAT(MATCH_DEFAULT_SYMBOL, c)))(m, c)
#define MATCH_CASE_EXPAND_IMPL(                                                                                                                            \
    m, f, c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27, c28, c29, ...) \
    MATCH_CASE_GEN(m, f, c0)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c1)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c2)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c3)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c4)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c5)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c6)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c7)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c8)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c9)                                                                                                                               \
    MATCH_CASE_GEN(m, f, c10)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c11)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c12)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c13)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c14)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c15)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c16)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c17)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c18)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c19)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c20)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c21)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c22)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c23)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c24)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c25)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c26)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c27)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c28)                                                                                                                              \
    MATCH_CASE_GEN(m, f, c29)

#define MATCH_CASE_EXPAND(m, f, ...) \
    MATCH_CASE_EXPAND_IMPL(m, f, __VA_ARGS__, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _)

#define MATCH_CASE_OR_DEFAULT_IMPL_0(m, c, ...) MATCH_CASE_EXPAND(m, (__VA_ARGS__), MATCH_PARENS_REMOVE(c))

#define MATCH_CASE_OR_DEFAULT_IMPL_1(m, c, ...) \
    default:                                    \
        return __VA_ARGS__();

#define MATCH_CASE_OR_DEFAULT_IMPL(condition) MATCH_CONCAT(MATCH_CASE_OR_DEFAULT_IMPL_, condition)

#define MATCH_CASE_FIRST_IMPL(c, ...) c
#define MATCH_CASE_FIRST(...) __VA_ARGS__
#define MATCH_CASE_PARSE_IMPL_0(...) MATCH_PP_EMPTY
#define MATCH_CASE_PARSE_IMPL_1_IMPL(m, c, ...)                                                                                           \
    MATCH_CASE_OR_DEFAULT_IMPL(MATCH_CASE_OR_DEFAULT_CHECK(MATCH_CONCAT(MATCH_DEFAULT_SYMBOL, MATCH_CASE_FIRST(MATCH_PARENS_REMOVE(c))))) \
    (m, c, __VA_ARGS__)
#define MATCH_CASE_PARSE_IMPL_1(m, c) MATCH_CASE_PARSE_IMPL_1_IMPL(m, c)
#define MATCH_CASE_PARSE_IMPL(condition) MATCH_CONCAT(MATCH_CASE_PARSE_IMPL_, condition)
#define MATCH_CASE_PARSE(m, c) MATCH_CASE_PARSE_IMPL(MATCH_PARENS_CHECK(MATCH_PARENS_PROBE c))(m, MATCH_PARENS_REMOVE(c))

#define MATCH_CASES_PARSE_IMPL(                                                                                                                         \
    m, c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27, c28, c29, ...) \
    MATCH_CASE_PARSE(m, c0)                                                                                                                             \
    MATCH_CASE_PARSE(m, c1)                                                                                                                             \
    MATCH_CASE_PARSE(m, c2)                                                                                                                             \
    MATCH_CASE_PARSE(m, c3)                                                                                                                             \
    MATCH_CASE_PARSE(m, c4)                                                                                                                             \
    MATCH_CASE_PARSE(m, c5)                                                                                                                             \
    MATCH_CASE_PARSE(m, c6)                                                                                                                             \
    MATCH_CASE_PARSE(m, c7)                                                                                                                             \
    MATCH_CASE_PARSE(m, c8)                                                                                                                             \
    MATCH_CASE_PARSE(m, c9)                                                                                                                             \
    MATCH_CASE_PARSE(m, c10)                                                                                                                            \
    MATCH_CASE_PARSE(m, c11)                                                                                                                            \
    MATCH_CASE_PARSE(m, c12)                                                                                                                            \
    MATCH_CASE_PARSE(m, c13)                                                                                                                            \
    MATCH_CASE_PARSE(m, c14)                                                                                                                            \
    MATCH_CASE_PARSE(m, c15)                                                                                                                            \
    MATCH_CASE_PARSE(m, c16)                                                                                                                            \
    MATCH_CASE_PARSE(m, c17)                                                                                                                            \
    MATCH_CASE_PARSE(m, c18)                                                                                                                            \
    MATCH_CASE_PARSE(m, c19)                                                                                                                            \
    MATCH_CASE_PARSE(m, c20)                                                                                                                            \
    MATCH_CASE_PARSE(m, c21)                                                                                                                            \
    MATCH_CASE_PARSE(m, c22)                                                                                                                            \
    MATCH_CASE_PARSE(m, c23)                                                                                                                            \
    MATCH_CASE_PARSE(m, c24)                                                                                                                            \
    MATCH_CASE_PARSE(m, c25)                                                                                                                            \
    MATCH_CASE_PARSE(m, c26)                                                                                                                            \
    MATCH_CASE_PARSE(m, c27)                                                                                                                            \
    MATCH_CASE_PARSE(m, c28)                                                                                                                            \
    MATCH_CASE_PARSE(m, c29)

#define MATCH_CASES_PARSE(m, ...) \
    MATCH_CASES_PARSE_IMPL(m, ##__VA_ARGS__, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _)

#define MATCH_IMPL(matchable, ...)                                  \
    [matchable{std::move(matchable)}]() mutable -> decltype(auto) { \
        switch(matchable)                                           \
        {                                                           \
            MATCH_CASES_PARSE(matchable, __VA_ARGS__)               \
        }                                                           \
    }()

#define MATCH(matchable, ...) MATCH_IMPL(MATCH_PARENS_REMOVE(matchable), __VA_ARGS__)

}  // namespace nstd
#endif
