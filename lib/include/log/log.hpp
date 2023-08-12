
#ifndef __NSTD_LOG_HPP__
#define __NSTD_LOG_HPP__

#include <cstddef>
#include <chrono>
#include "marker.hpp"
#include "type_traits.hpp"
#include "source_location.hpp"

namespace nstd {

class LogType;
class LogMetaData;
trait Logger;

#define NSTD_NON LogType(LogType::_PreSetLogType::LOG_NON)
#define NSTD_TRACE LogType(LogType::_PreSetLogType::LOG_TRACE)
#define NSTD_DEBUG LogType(LogType::_PreSetLogType::LOG_DEBUG)
#define NSTD_INFO LogType(LogType::_PreSetLogType::LOG_INFO)
#define NSTD_WARN LogType(LogType::_PreSetLogType::LOG_WARN)
#define NSTD_ERROR LogType(LogType::_PreSetLogType::LOG_ERROR)
#define NSTD_FATAL LogType(LogType::_PreSetLogType::LOG_FATAL)
#define NSTD_PERF LogType(LogType::_PreSetLogType::LOG_PERF)
#define NSTD_FUNC LogType(LogType::_PreSetLogType::LOG_FUNC)
#define __PRE_SET_RESERVE_LOG_TYPE_BIT \
    0x000001FF  // We reserved 16 bits for _PreSetLogType, but urrently, only 9bits are used.

template <typename T1,
          typename T2,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool> = true,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool> = true>
T1 operator&(const T1& t1, const T2& t2)
{
    IF_CONSTEXPR(nstd::is_base_of_v<T2, T1>)
    return T1{t1.mask & (t2.mask & __PRE_SET_RESERVE_LOG_TYPE_BIT)};
}
template <typename T1,
          typename T2,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool> = true,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool> = true,
          nstd::enable_if_t<nstd::is_base_of_v<T2, T1>, bool> = true>
T1 operator&(const T1& t1, const T2& t2)
{
    return T1{t1.mask & t2.mask};
}

template <typename T1,
          typename T2,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool> = true,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool> = true
          nstd::enable_if_t<!nstd::is_base_of_v<T2, T1>, bool> = true>
T1 operator|(const T1& t1, const T2& t2)
{
    return T1{t1.mask | (t2.mask & __PRE_SET_RESERVE_LOG_TYPE_BIT)};
}
template <typename T1,
          typename T2,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool> = true,
          nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool> = true
          nstd::enable_if_t<nstd::is_base_of_v<T2, T1>, bool> = true>
T1 operator|(const T1& t1, const T2& t2)
{
    
    return T1{t1.mask | t2.mask};
}

class ProcStart {
    static const std::chrono::steady_clock proc_start;
};

class LogType {
public:
    enum _PreSetLogType
    {
        LOG_NON   = 0,      // 0bit
        LOG_TRACE = 1,      // 1bit
        LOG_DEBUG = 2,      // 2bit
        LOG_INFO  = 4,      // 3bit
        LOG_WARN  = 8,      // 4bit
        LOG_ERROR = 16,     // 5bit
        LOG_FATAL = 32,     // 6bit
        LOG_PERF  = 64,     // 7bit
        LOG_FUNC  = 128,    // 8bit
        LOG_RESV  = 32768,  // _PreSetLogType reserve 16 bits.
    };

protected:
    unsigned int mask;

public:
    LogType() = default;
    LogType(unsigned int m);
    template <typename T1,
              typename T2,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool>,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool>,
              nstd::enable_if_t<!nstd::is_base_of_v<T2, T1>, bool> = true>
    friend T1 operator&(const T1& t1, const T2& t2);
    template <typename T1,
              typename T2,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool>,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool>,
              nstd::enable_if_t<nstd::is_base_of_v<T2, T1>, bool> = true>
    friend T1 operator&(const T1& t1, const T2& t2);
    template <typename T1,
              typename T2,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool>,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool>,
              nstd::enable_if_t<!nstd::is_base_of_v<T2, T1>, bool> = true>
    friend T1 operator|(const T1& t1, const T2& t2);
    template <typename T1,
              typename T2,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T1>, bool>,
              nstd::enable_if_t<nstd::is_base_of_v<LogType, T2>, bool>,
              nstd::enable_if_t<nstd::is_base_of_v<T2, T1>, bool> = true>
    friend T1 operator|(const T1& t1, const T2& t2);
};

template <typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
struct LogMetaData {
    std::string log_type;
    std::string log_mod;
    std::string file;
    std::string func;
    unsigned int line;
    private:
    unsigned int colum = 0; // Now we can't provide colum. So it' private.
};

template <typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
LogMetaData<T> make_log_meta_data(
    T&& t, std::string&& file, std::string&& func, unsigned int line, std::string&& m = "")
{
    return {t, m, file, func, line};
}

template<typename T2, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
trait Logger
{
    protected:
    T2 log_type_permit;
public:
    thread_local static std::ostringstream buf;
    template <typename T1, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
    bool enabled(const LogMetaData<T1>& md)
    {

    }
};


#define NSTD_LOG_TRACE(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData<LogType> md = make_log_meta_data(NSTD_TRACE, __NSTD_FILE__, __NSTD_FUNC__, __NSTD_LINE__); \
        if(logger.enabled(md)) { logger.log(md, ); }                                             \
    } while(false)

#define NSTD_LOG_DEBUG(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData<LogType> md = make_log_meta_data(NSTD_DEBUG, __FILE__, __func__, __LINE__); \
        if(logger.enable(md))                                                                   \
        {                                                                                       \
            logger.buf << __VA_ARGS__;                                                          \
            logger.log(md);                                                                     \
        }                                                                                       \
    } while(false)

}  // namespace nstd

#endif
