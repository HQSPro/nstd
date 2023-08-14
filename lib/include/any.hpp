#ifndef __NSTD_ANY_HPP__
#define __NSTD_ANY_HPP__

#include "version.hpp"

#ifdef __cpp_lib_any
#define __NSTD_LIB_HAS_ANY
#endif

#ifdef __NSTD_LIB_HAS_ANY
#include <any>    // require c++17
#endif

namespace nstd
{
    #ifdef __NSTD_LIB_HAS_ANY
    using std::any;
    using std::any_cast;
    #else
    // todo: Impl any and any_cast
    #endif
}


#endif
