
#ifndef __NSTD_MARKER_HPP__
#define __NSTD_MARKER_HPP__

#include <cstdlib>

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

#endif
