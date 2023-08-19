
#include "log.hpp"

namespace nstd {
const std::chrono::steady_clock ProcStart::proc_start = std::chrono::steady_clock::now();
thread_local std::stringstream Logger::buf;
std::mutex GlobalLogger::mtx;
std::shared_ptr<Logger> GlobalLogger::glogger;
LogResult GlobalLogger::add_logger(std::shared_ptr<Logger> plogger) noexcept
{
    try
    {
        if(plogger.get())
        {
            std::unique_lock<std::mutex> lock{mtx, std::defer_lock};
            if(lock.try_lock_for(std::chrono::seconds{__NSTD_LOG_TIMEOUT}))
            {
                if(glogger.try_emplace(std::size_t(plogger.get()), plogger).second) { return; }
                else { return LogResult::err("Add the same address of logger to global logger."); }
            }
            else { return LogResult::err("Lock global logger failed."); }
        }
        else { return LogResult::ok(std::monostate{}); }
    }
    catch(const std::exception& e)
    {
        return LogResult::err(e.what());
    }
}

LogResult GlobalLogger::remove_logger(std::shared_ptr<Logger> plogger) noexcept
{
    try
    {
        std::unique_lock<std::mutex> lock{mtx, std::defer_lock};
        if(lock.try_lock_for(std::chrono::seconds{__NSTD_LOG_TIMEOUT}))
        {
            auto iter = std::find(std::size_t(plogger.get()));
            if(iter != glogger.end()) { glogger.erase(iter); }
            else { return LogResult::ok(std::monostate{}); }
        }
        else { return LogResult::err("Lock global logger failed."); }
    }
    catch(const std::exception& e)
    {
        return LogResult::err(e.what());
    }
}

std::mutex& GlobalLogger::global_logger_mutex() noexcept { return mtx; }

std::map<std::size_t, std::shared_ptr<Logger>>& GlobalLogger::global_logger() noexcept
{
    return glogger;
}

}  // namespace nstd
