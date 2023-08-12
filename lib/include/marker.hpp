
#ifndef __NSTD_MARKER_HPP__
#define __NSTD_MARKER_HPP__

#include <cstdlib>

/* if constexpr is a c++17 feature, before c++17, we can't use it.*/
#if __cplusplus >= 201703L  // c++17
#define IF_CONSTEXPR if constexpr
#else
#define IF_CONSTEXPR if
#endif

#define TODO(...)                                            \
    do {                                                     \
        fprintf(stderr, "Not impl yet. " #__VA_ARGS__ "\n"); \
        std::abort()                                         \
    } while(0)
#define UNIMPL(...)                                      \
    do {                                                 \
        fprintf(stderr, "Not impl. " #__VA_ARGS__ "\n"); \
        std::abort()                                     \
    } while(0)
#define UNREACHABLE(...)                                                    \
    do {                                                                    \
        fprintf(stderr, "This logic can't be reached. " #__VA_ARGS__ "\n"); \
        std::abort()                                                        \
    } while(0)

#define trait class

#endif
