
#ifndef __NSTD_LOG_HPP__
#define __NSTD_LOG_HPP__

#include <iomanip>
#include <iostream>
#include <cstddef>
#include <chrono>
#include <memory>
#include <map>
#include "marker.hpp"
#include "type_traits.hpp"
#include "source_location.hpp"
#include "self_ref.hpp"
#include "result.hpp"

namespace nstd {

using LogResult = nstd::ResultOmitOk<std::string>;

trait ILogMask;
// The base class of your own LogType class
class LogType;
class LogMetaData;
// The base class of your own Logger class
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

#define __NSTD_LOG_TIMEOUT 30  // timeout: 30s
#define __NSTD_WARNING(...)                                                          \
    do {                                                                             \
        std::ostringstream buf;                                                      \
        buf << std::left << std::setw(15) << "[nstd(Warn)] " << __NSTD_FILE__ << ":" \
            << __NSTD_LINE__ << ". " << __VA_ARGS__ << std::endl;                    \
        std::cerr << buf.str();                                                      \
    } while(0)

#define __NSTD_ERROR(...)                                                             \
    do {                                                                              \
        std::ostringstream buf;                                                       \
        buf << std::left << std::setw(15) << "[nstd(Error)] " << __NSTD_FILE__ << ":" \
            << __NSTD_LINE__ << ". " << __VA_ARGS__ << std::endl;                     \
        std::cerr << buf.str();                                                       \
    } while(0)

#define NSTD_LOGGER(logger, type, ...)                                         \
    do {                                                                       \
        try                                                                    \
        {                                                                      \
            LogMetaData md(type, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__); \
            if(logger.enabled(md))                                             \
            {                                                                  \
                logger.get_buf() << __VA_ARGS__ << std::endl;                  \
                logger.log(std::move(md));                                     \
            }                                                                  \
        }                                                                      \
        catch(const std::exception& e)                                         \
        {                                                                      \
            __NSTD_ERROR(e.what());                                            \
        }                                                                      \
    } while(false)

#define NSTD_LOG(type, ...)                                                                     \
    do {                                                                                        \
        try                                                                                     \
        {                                                                                       \
            LogMetaData md(type, __NSTD_FILE__, __NSTD_LINE__, __NSTD_FUNC__);                  \
            std::unique_lock<std::mutex>(GlobalLogger::global_logger_mutex(), std::defer_lock); \
            if(lock.try_lock_for(std::chrono::seconds{__NSTD_LOG_TIMEOUT}))                     \
            {                                                                                   \
                std::map<std::size_t, std::shared_ptr<Logger>>& glogger =                       \
                    GlobalLogger::global_logger();                                              \
                for(auto iter = glogger.begin(); iter != glogger.end(); ++iter)                 \
                {                                                                               \
                    std::shared_ptr<Logger>& logger = iter->second;                             \
                    if(logger->enabled(md))                                                     \
                    {                                                                           \
                        logger.get_buf() << __VA_ARGS__ << std::endl;                           \
                        logger->log(std::move(md));                                             \
                    }                                                                           \
                }                                                                               \
            }                                                                                   \
            else { __NSTD_ERROR("Lock global logger failed."); }                                \
        }                                                                                       \
        catch(const std::exception& e)                                                          \
        {                                                                                       \
            __NSTD_ERROR(e.what());                                                             \
        }                                                                                       \
    } while(false)

#define NSTD_LOGGER_TRACE(logger, ...) NSTD_LOGGER(logger, NSTD_TRACE, __VA_ARGS__)
#define NSTD_LOGGER_DEBUG(logger, ...) NSTD_LOGGER(logger, NSTD_DEBUG, __VA_ARGS__)
#define NSTD_LOGGER_INFO(logger, ...) NSTD_LOGGER(logger, NSTD_INFO, __VA_ARGS__)
#define NSTD_LOGGER_WARN(logger, ...) NSTD_LOGGER(logger, NSTD_WARN, __VA_ARGS__)
#define NSTD_LOGGER_ERROR(logger, ...) NSTD_LOGGER(logger, NSTD_ERROR, __VA_ARGS__)
#define NSTD_LOGGER_FATAL(logger, ...) NSTD_LOGGER(logger, NSTD_FATAL, __VA_ARGS__)
#define NSTD_LOGGER_PERF(logger, ...) NSTD_LOGGER(logger, NSTD_PERF, __VA_ARGS__)
#define NSTD_LOGGER_FUNC(logger, ...) NSTD_LOGGER(logger, NSTD_FUNC, __VA_ARGS__)

#define NSTD_LOG_TRACE(...) NSTD_LOG(NSTD_TRACE, __VA_ARGS__)
#define NSTD_LOG_DEBUG(...) NSTD_LOG(NSTD_DEBUG, __VA_ARGS__)
#define NSTD_LOG_INFO(...) NSTD_LOG(NSTD_INFO, __VA_ARGS__)
#define NSTD_LOG_WARN(...) NSTD_LOG(NSTD_WARN, __VA_ARGS__)
#define NSTD_LOG_ERROR(...) NSTD_LOG(NSTD_ERROR, __VA_ARGS__)
#define NSTD_LOG_FATAL(...) NSTD_LOG(NSTD_FATAL, __VA_ARGS__)
#define NSTD_LOG_PERF(...) NSTD_LOG(NSTD_PERF, __VA_ARGS__)
#define NSTD_LOG_FUNC(...) NSTD_LOG(NSTD_FUNC, __VA_ARGS__)

// We reserved 16 bits for pre defined log type, but currently, only 8bits are used.
#define __PRE_DEFINED_RESERVE_LOG_TYPE_BIT 0x000001FF

template <typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
T operator&(const LogType& t1, const T& t2) noexcept
{
    if(dynamic_cast<const T*>(&t1) != nullptr) { return T{t1.mask & t2.mask}; }
    else
    {
        __NSTD_WARNING(
            "There is conflict within log types. Will use the predefined log type only. Log type "
            << typeid(t1).name() << " conflict with log type " << typeid(t2).name() << ".");
        T{t1.mask & __PRE_DEFINED_RESERVE_LOG_TYPE_BIT & t2.mask};
    }
}

template <typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
T operator|(const LogType& t1, const T& t2) noexcept
{
    if(dynamic_cast<const T*>(&t1) != nullptr) { return T{t1.mask | t2.mask}; }
    else
    {
        __NSTD_WARNING(
            "There is conflict within log types. Will use the incoming log type only. Log type "
            << typeid(t1).name() << " conflict with log type " << typeid(t2).name() << ".");
        T{t1.mask & __PRE_DEFINED_RESERVE_LOG_TYPE_BIT | t2.mask};
    }
}

trait ILogMask
{
public:
    template <typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool>>
    friend T operator&(const LogType& t1, const T& t2) noexcept;
    template <typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool>>
    friend T operator|(const LogType& t1, const T& t2) noexcept;
    constexpr ILogMask(unsigned int m) : mask(m){};
    constexpr ILogMask(ILogMask &&)                = default;
    constexpr ILogMask(const ILogMask&)            = default;
    constexpr ILogMask& operator=(ILogMask&&)      = default;
    constexpr ILogMask& operator=(const ILogMask&) = default;
    virtual ~ILogMask()                            = default;

protected:
    unsigned int mask;
};

class LogType : virtual public ILogMask {
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
    constexpr LogType() : ILogMask(LogType::LOG_NON){};
    constexpr LogType(unsigned int m) : ILogMask(m){};
    constexpr LogType(LogType&&)                  = default;
    constexpr LogType(const LogType&)             = default;
    constexpr LogType& operator=(LogType&&)       = default;
    constexpr LogType& operator=(const LogType&&) = default;
    virtual const char* c_str()
    {
        switch(mask)
        {
        case LOG_NON: return "";
        case LOG_TRACE: return "Trace";
        case LOG_DEBUG: return "Debug";
        case LOG_INFO: return "Info";
        case LOG_WARN: return "Warn";
        case LOG_ERROR: return "Error";
        case LOG_FATAL: return "Fatal";
        case LOG_PERF: return "Perf";
        case LOG_FUNC: return "Func";
        default:
            __NSTD_ERROR("Must chose one type of logging. Current value: " << std::hex << mask
                                                                           << ".");
            return "";
        }
    }
};

struct LogMetaData {
    template <typename T, nstd::enable_if_t<nstd::is_base_of_v<LogType, T>, bool> = true>
    constexpr LogMetaData(T lt,
                          std::string&& file_,
                          unsigned int line_,
                          std::string&& func_,
                          std::string&& mod = "",
                          unsigned int col  = 0)
        : log_type(nstd::make_self_ref<LogType, T>(lt)), file(std::move(file_)), line(line_),
          func(std::move(func_)), log_mod(std::move(mod)), colum(col)
    {
    }
    nstd::SelfRef<LogType> log_type;
    std::string log_mod;
    std::string file;
    std::string func;
    unsigned int line;

private:
    unsigned int colum = 0;  // Now we can't provide colum. So it' private.
};

trait Logger
{
protected:
    thread_local static std::ostringstream buf;

public:
    virtual std::ostringstream& get_buf() { return buf; }
    virtual bool enabled(const LogMetaData&) = 0;
    virtual LogResult log(LogMetaData &&)    = 0;
};

struct ProcStart {
    static const std::chrono::steady_clock proc_start;
};

class GlobalLogger {
    static std::mutex mtx;
    static std::map<std::size_t, std::shared_ptr<Logger>> glogger;

public:
    static LogResult add_logger(std::shared_ptr<Logger> plogger) noexcept;
    static LogResult remove_logger(std::shared_ptr<Logger> plogger) noexcept;
    static std::mutex& global_logger_mutex() noexcept;
    static std::map<std::size_t, std::shared_ptr<Logger>>& global_logger() noexcept;
};

}  // namespace nstd

#endif
