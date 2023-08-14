
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
#include "result.hpp"

namespace nstd {

trait ILogMask;
class LogType;
class LogMetaData;
trait Logger;
class GlobalLogger;

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
        LogMetaData md(NSTD_TRACE, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)

#define NSTD_LOGGER_DEBUG(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_DEBUG, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)

#define NSTD_LOGGER_INFO(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_INFO, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)

#define NSTD_LOGGER_WARN(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_WARN, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)

#define NSTD_LOGGER_ERROR(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_ERROR, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)

#define NSTD_LOGGER_FATAL(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_FATAL, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)

#define NSTD_LOGGER_PERF(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_PERF, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)

#define NSTD_LOGGER_FUNC(logger, ...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_FUNC, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        if(logger.enabled(md)) { logger.log(std::move(md));}                                             \
    } while(false)


#define NSTD_LOG_TRACE(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_TRACE, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define NSTD_LOG_DEBUG(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_DEBUG, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define NSTD_LOG_INFO(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_INFO, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define NSTD_LOG_WARN(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_WARN, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define NSTD_LOG_ERROR(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_ERROR, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define NSTD_LOG_FATAL(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_FATAL, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define NSTD_LOG_PERF(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_PERF, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define NSTD_LOG_FUNC(...)                                                             \
    do {                                                                                        \
        LogMetaData md(NSTD_FUNC, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
        auto logger = GlobalLogger::get_global_logger(); \
        if(logger != nullptr) \
        { \
            if((*logger)->enabled(md)) { (*logger)->log(std::move(md));}                                             \
        } \
        else{ \
            __NSTD_ERROR("No global logget set before using it."); \
        } \
    } while(false)

#define __PRE_DEFINED_RESERVE_LOG_TYPE_BIT \
    0x000001FF  // We reserved 16 bits for pre defined log type, but currently, only 8bits are used.
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
    constexpr ILogMask(ILogMask&&) = default;
    constexpr ILogMask(const ILogMask&) = default;
    constexpr ILogMask& operator=(ILogMask&&) = default;
    constexpr ILogMask& operator=(const ILogMask&) = default;
    virtual ~ILogMask() = default;
    protected:
    unsigned int mask;
};

class LogType: virtual public ILogMask
{
    public:
    enum
    {
        LOG_NON   = 0,      // 0bit
        LOG_TRACE = 1,      // 0bit
        LOG_DEBUG = 2,      // 1bit
        LOG_INFO  = 4,      // 2bit
        LOG_WARN  = 8,      // 3bit
        LOG_ERROR = 16,     // 4bit
        LOG_FATAL = 32,     // 5bit
        LOG_PERF  = 64,     // 6bit
        LOG_FUNC  = 128,    // 7bit
        LOG_RESV  = 32768,  // _PreSetLogType reserve 16 bits.
    };
    constexpr LogType(): ILogMask(LogType::LOG_NON) {};
    constexpr LogType(unsigned int m): ILogMask(m) {};
    constexpr LogType(LogType&&) = default;
    constexpr LogType(const LogType&) = default;
    constexpr LogType& operator = (LogType&&) = default;
    constexpr LogType& operator = (const LogType&&) = default;
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
            return "";
        }
    }
};

struct LogMetaData {
    template<typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
    constexpr LogMetaData(T lt, std::string&& file_, unsigned int line_, std::string&& func_, std::string&& mod = "", unsigned int col = 0): log_type(nstd::make_self_ref<LogType, T>(lt)), file(std::move(file_)), line(line_), func(std::move(func_)), log_mod(std::move(mod)), colum(col){}
    nstd::SelfRef<LogType> log_type;
    std::string log_mod;
    std::string file;
    std::string func;
    unsigned int line;
    private:
    unsigned int colum = 0; // Now we can't provide colum. So it' private.
};

trait Logger
{
public:
    thread_local static std::ostringstream buf;
    virtual bool enabled(const LogMetaData&) = 0;
    virtual nstd::ResultOmitOk<std::string> log(LogMetaData&&) = 0;
};

class ProcStart {
    static const std::chrono::steady_clock proc_start;
};

class GlobalLogger
{
    public:
    static std::unique_ptr<Logger>* get_global_logger(std::unique_ptr<Logger>&& init = nullptr);
};

}  // namespace nstd

#endif
