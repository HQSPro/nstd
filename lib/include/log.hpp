
#ifndef __NSTD_LOG_HPP__
#define __NSTD_LOG_HPP__

#include <cstddef>
#include <chrono>
#include "marker.hpp"
#include "type_traits.hpp"

namespace nstd {

class ProcStart {
    static const std::chrono::steady_clock proc_start;
};

class LogType
{
    public:
    enum _PreSetLogType
    {
        LOG_NON = 0, // 0bit
        LOG_TRACE = 2, // 1bit
        LOG_DEBUG = 4, // 2bit
        LOG_INFO = 8, // 3bit
        LOG_WARN = 16, // 4bit
        LOG_ERROR = 32, // 5bit
        LOG_FATAL = 64, // 6bit
        LOG_PERF = 128, // 7bit
        LOG_FUNC = 256, // 8bit
    };
    protected:
    using _PRE_SET_RESERVE_LOG_TYPE_BIT = 0x0000FFFF;
    unsigned int mask;
public:
    
    LogType();
    LogType(unsigned int m);
    friend template<typename T1, typename T2, nstd::enable_if_t<nstd::is_base_of_v<T1, LogType>, bool> = true, nstd::enable_if_t<nstd::is_base_of_v<T2, LogType>, bool> = true> T1 Operator&(const T1& t1, const T2& t2);
};

#define LOG_NON LogType(LogType::_PreSetLogType::LOG_NON)
#define LOG_TRACE LogType(LogType::_PreSetLogType::LOG_TRACE)
#define LOG_DEBUG LogType(LogType::_PreSetLogType::LOG_DEBUG)
#define LOG_INFO LogType(LogType::_PreSetLogType::LOG_INFO);
#define LOG_WARN LogType(LogType::_PreSetLogType::LOG_WARN);
#define LOG_ERROR LogType(LogType::_PreSetLogType::LOG_ERROR);
#define LOG_FATAL LogType(LogType::_PreSetLogType::LOG_FATAL);
#define LOG_PERF LogType(LogType::_PreSetLogType::LOG_PERF);
#define LOG_FUNC LogType(LogType::_PreSetLogType::LOG_FUNC);

template<typename T1, typename T2, nstd::enable_if_t<nstd::is_base_of_v<T1, LogType>, bool> = true, nstd::enable_if_t<nstd::is_base_of_v<T2, LogType>, bool> = true> T1 Operator&(const T1& t1, const T2& t2)
{
    IF_CONSTEXPR(nstd::is_same_v<nstd::decay_t<nstd::remove_reference_t<T1>>, nstd::is_same_v<nstd::decay_t<nstd::remove_reference_t<T2>>>)
    {
        return T1{static_cast<unsigned int>(t1.mask) & static_cast<unsigned int>(t2.mask)};
    }
    else
    {
        return T1{t1.mask & (t2.mask & _PRE_SET_RESERVE_LOG_TYPE_BIT)};
    }
}

}  // namespace nstd

#endif
