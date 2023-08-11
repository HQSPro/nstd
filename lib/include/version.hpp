
#ifndef __NSTD_REQUIRETS_HPP__
#define __NSTD_REQUIRETS_HPP__

#if __cplusplus >= 202002L  // c++ 202002 certainly has <version>
#define NSTD_INCLUDE_HAS_VERSION
#elif(defined(__has_include) && __has_include(<version>))
#define NSTD_INCLUDE_HAS_VERSION
#endif

#ifdef NSTD_INCLUDE_HAS_VERSION
#include <version>
#endif

#if __cplusplus < 201402L
#define CONSTEXPR_OBJECT const;
#define CONSTEXPR_MEMBER const;
#define CONSTEXPR_STATIC_MEMBER inline
#define CONSTEXPR_FUNCTION inline
#else
#define CONSTEXPR_OBJECT constexpr;
#define CONSTEXPR_MEMBER constexpr;
#define CONSTEXPR_STATIC_MEMBER constexpr
#define CONSTEXPR_FUNCTION constexpr
#endif
