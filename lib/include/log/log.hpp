
#ifndef __NSTD_LOG_HPP__
#define __NSTD_LOG_HPP__

#include <iomanip>
#include <iostream>
#include <cstddef>
#include <chrono>
#include "marker.hpp"
#include "type_traits.hpp"
#include "source_location.hpp"
#include "self_ref.hpp"

namespace nstd {

trait ILogMask;
class LogType;
class LogMetaData;
trait Logger;

#define NSTD_NON LogType(LogType::LOG_NON)
#define NSTD_TRACE LogType(LogType::LOG_TRACE)
#define NSTD_DEBUG LogType(LogType::LOG_DEBUG)
#define NSTD_INFO LogType(LogType::LOG_INFO)
#define NSTD_WARN LogType(LogType::LOG_WARN)
#define NSTD_ERROR LogType(LogType::LOG_ERROR)
#define NSTD_FATAL LogType(LogType::LOG_FATAL)
#define NSTD_PERF LogType(LogType::LOG_PERF)
#define NSTD_FUNC LogType(LogType::LOG_FUNC)

#define NSTD_LOGGER_TRACE(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_TRACE, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__, ); \
        if(logger.enabled(md)) { logger.log(md, ); }                                             \
    } while(false)

#define __PRE_DEFINED_RESERVE_LOG_TYPE_BIT \
    0x000001FF  // We reserved 16 bits for pre defined log type, but currently, only 9bits are used.
#define __NSTD_WARNING(...) do{std::ostringstream buf;buf<<std::left<<std::setw(15)<<"[nstd(Warn)] " <<__NSTD_FILE__<<":"<<__NSTD_LINE__<<". " <<__VA_ARGS__<<"\n";std::cerr<<buf.str();}while(0)
#define __NSTD_ERROR(...) do{std::ostringstream buf;buf<<std::left<<std::setw(15)<<"[nstd(Error)] " <<__NSTD_FILE__<<":"<<__NSTD_LINE__<<". " <<__VA_ARGS__<<"\n";std::cerr<<buf.str();}while(0)

template<typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
T operator&(const LogType& t1, const T& t2)
{
    if(dynamic_cast<const T*>(&t1) != nullptr)
    {
        return T{t1.mask & t2.mask};
    }
    else
    {
        __NSTD_WARNING("There is conflict within log types. Will use the predefined log type only. Log type "<< typeid(t1).name() << " conflict with log type "<< typeid(t2).name() << ".");
        T{t1.mask & __PRE_DEFINED_RESERVE_LOG_TYPE_BIT & t2.mask};
    }
}

template<typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
T operator|(const LogType& t1, const T& t2)
{
    if(dynamic_cast<const T*>(&t1) != nullptr)
    {
        return T{t1.mask | t2.mask};
    }
    else
    {
        __NSTD_WARNING("There is conflict within log types. Will use the incoming log type only. Log type "<< typeid(t1).name() << " conflict with log type "<< typeid(t2).name() << ".");
        T{t1.mask & __PRE_DEFINED_RESERVE_LOG_TYPE_BIT | t2.mask};
    }
}

trait ILogMask
{
    public:
    template<typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool>>
    friend T operator&(const LogType& t1, const T& t2);
    template<typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool>>
    friend T operator|(const LogType& t1, const T& t2);
    constexpr ILogMask(unsigned int m): mask(m){};
    virtual ~ILogMask(){}
    protected:
    unsigned int mask;
};

class LogType: virtual public ILogMask
{
    public:
    enum
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
    constexpr LogType(): ILogMask(LogType::LOG_NON) {};
    LogType(unsigned int m): ILogMask(m) {};
    virtual const char* c_str()
    {
        switch (mask)
        {
        case LOG_NON:
            return "";
        case LOG_TRACE:
            return "Trace";
        case LOG_DEBUG:
            return "Debug";
        case LOG_INFO:
            return "Info";
        case LOG_WARN:
            return "Warn";
        case LOG_ERROR:
            return "Error";
        case LOG_FATAL:
            return "Fatal";
        case LOG_PERF:
            return "Perf";
        case LOG_FUNC:
            return "Func";
        default:
            __NSTD_ERROR("Must chose one type of logging. Current value: "<< std::hex << mask << ".");
            break;
        }
    }
};

class ProcStart {
    static const std::chrono::steady_clock proc_start;
};

struct LogMetaData {
    SelfRef<LogType> log_type;
    std::string log_mod;
    std::string file;
    std::string func;
    unsigned int line;
    private:
    unsigned int colum = 0; // Now we can't provide colum. So it' private.
    template<typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
    LogMetaData(T lt, std::string&& file_, unsigned int line_, std::string&& func_, std::string&& mod = "", unsigned int col = 0): log_type(nstd::make_self_ref<LogType, T>(lt)), file(std::move(file_)), line(line_), func(std::move(func_)), log_mod(std::move(mod)), colum(col){}
};

trait Logger
{
public:
    thread_local static std::ostringstream buf;
};

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
