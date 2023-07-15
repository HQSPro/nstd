
#ifndef __NSTD_MATCH_HPP__
#define __NSTD_MATCH_HPP__

#include<type_traits>
#include<type_magic.hpp>

namespace nstd {

#define PP_COMMA() ,
#define PP_LPAREN() (
#define PP_RPAREN() )
#define PP_EMPTY()

#define PP_REMOVE_PARENS(T) PP_REMOVE_PARENS_IMPL T
#define PP_REMOVE_PARENS_IMPL(...) __VA_ARGS__

#define PP_CONCAT(A, B) PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B) A##B

#define PP_INC(N) PP_CONCAT(PP_INC_, N)
#define PP_INC_0 1
#define PP_INC_1 2
// ...
#define PP_INC_254 255
#define PP_INC_255 256

#define PP_DEC(N) PP_CONCAT(PP_DEC_, N)
#define PP_DEC_256 255
#define PP_DEC_255 254
// ...
#define PP_DEC_2 1
#define PP_DEC_1 0

#define PP_NOT(N) PP_CONCAT(PP_NOT_, N)
#define PP_NOT_0 1
#define PP_NOT_1 0

#define PP_AND(A, B) PP_CONCAT(PP_AND_, PP_CONCAT(A, B))
#define PP_AND_00 0
#define PP_AND_01 0
#define PP_AND_10 0
#define PP_AND_11 1

#define PP_BOOL(N) PP_CONCAT(PP_BOOL_, N)
#define PP_BOOL_0 0
#define PP_BOOL_1 1
#define PP_BOOL_2 1
// ...

#define PP_IF(PRED, THEN, ELSE) PP_CONCAT(PP_IF_, PP_BOOL(PRED))(THEN, ELSE)
#define PP_IF_1(THEN, ELSE) THEN
#define PP_IF_0(THEN, ELSE) ELSE

#define PP_COMMA_IF(N) PP_IF(N, PP_COMMA, PP_EMPTY)()

#define PP_GET_N(N, ...) PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)
#define PP_GET_N_0(_0, ...) _0
#define PP_GET_N_1(_0, _1, ...) _1
#define PP_GET_N_2(_0, _1, _2, ...) _2
// ...
#define PP_GET_N_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8

#define PP_GET_TUPLE(N, T) PP_GET_N(N, PP_REMOVE_PARENS(T))

#define PP_IS_EMPTY(...)                                      \
  PP_AND(PP_AND(PP_NOT(PP_HAS_COMMA(__VA_ARGS__)),            \
                PP_NOT(PP_HAS_COMMA(__VA_ARGS__()))),         \
         PP_AND(PP_NOT(PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__)), \
                PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__())))
#define PP_HAS_COMMA(...) PP_GET_N_8(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 0, 0)
#define PP_COMMA_V(...) ,

#define PP_VA_OPT_COMMA(...) PP_COMMA_IF(PP_NOT(PP_IS_EMPTY(__VA_ARGS__)))


#define DO_EACH(VAR, IDX, CTX) PP_COMMA_IF(IDX) CTX VAR

#define PP_NARG(...)                                                           \
  PP_GET_N(8, __VA_ARGS__ PP_VA_OPT_COMMA(__VA_ARGS__) 8, 7, 6, 5, 4, 3, 2, 1, \
           0)

#define PP_FOR_EACH(DO, CTX, ...) \
  PP_CONCAT(PP_FOR_EACH_, PP_NARG(__VA_ARGS__))(DO, CTX, 0, __VA_ARGS__)
#define PP_FOR_EACH_0(DO, CTX, IDX, ...)
#define PP_FOR_EACH_1(DO, CTX, IDX, VAR, ...) DO(VAR, IDX, CTX)
#define PP_FOR_EACH_2(DO, CTX, IDX, VAR, ...) \
  DO(VAR, IDX, CTX)                           \
  PP_FOR_EACH_1(DO, CTX, PP_INC(IDX), __VA_ARGS__)
#define PP_FOR_EACH_3(DO, CTX, IDX, VAR, ...) \
  DO(VAR, IDX, CTX)                           \
  PP_FOR_EACH_2(DO, CTX, PP_INC(IDX), __VA_ARGS__)
// ...



PP_FOR_EACH(DO_EACH, void, )        // (empty)
PP_FOR_EACH(DO_EACH, int, a, b, c)  // -> int a, int b, int c
PP_FOR_EACH(DO_EACH, bool, x)       // -> bool x


#define MATCH_CASE(I, F) case I: F()

#define MATCH_PARSE_CASE_IMPL1(predict, ...)
#define MATCH_PARSE_CASE_IMPL(...) MATCH_PARSE_CASE_IMPL1(__VA_ARGS__)
#define MATCH_PARSE_CASE(T) MATCH_PARSE_CASE_IMPL T


#define MATCH_FOR_EACH(c0, ...) MATCH_PARSE_CASE(c0)

#define MATCH_IMPL(matchable, c0, ...) [&matchable]() -> decaltype(auto) {switch (matchable)
{
MATCH_FOR_EACH(c0, __VA_ARGS__)
default:
    return {};
}}

#define MATCH(matchable, ...) MATCH_IMPL(matchable, __VA_ARGS__)

MATCH(result, (OK, ||{}), (ERR, ||{}));

template<typename Derived, typename = typename std::enable_if<nstd::is_pure_type<Derived>>::type>
class ApplyMatchTrait
{
    public:
    using type = Derived;
    operator int() const
    {
        return static_cast<int>(*static_cast<Derived*>(this));
    }
};

template<int I, typename OneType, typename Func>
decltype(auto) match_single_branch(OneType&& v, Func&& func)
{
    
}



template <typename MatchableType, typename ItemFunc, typename Func>
decltype(auto) match_impl(MatchableType&& v, ItemFunc&& item_func)
{
    
}

template <typename MatchableType, typename ItemFunc, typename... ItemFuncs>
decltype(auto) match_impl(MatchableType&& v, ItemFunc&& item_func, ItemFuncs&&... item_funcs)
{
    static_cast<int>(v)
}

template <typename MatchableType, typename... ItemFuncs>
decltype(auto) match(MatchableType&& v, ItemFuncs&&... item_funcs)
{
    return match_impl(v, item_funcs...);
}
template<typename MatchableType, typename... Funcs>
decltype(auto) match(MatchableType* v, Funcs&&... funcs)
{
    if constexpr (
        std::is_same<typename std::remove_cvref<MatchableType>::type, typename ApplyMatchTrait<std::remove_cvref<MatchableType>::type>>
    )
}
}
#endif
