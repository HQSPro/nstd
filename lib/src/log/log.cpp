
#include "log.hpp"

namespace nstd {
const std::chrono::steady_clock ProcStart::proc_start = std::chrono::steady_clock::now();
thread_local std::ostringstream Logger::buf           = std::ostringstream{};

std::unique_ptr<Logger>* GlobalLogger::get_global_logger(std::unique_ptr<Logger>&& init)
{
    static std::unique_ptr<Logger> glogger = init;
    if(glogger != nullptr)
    {
        return &glogger;
    }
}

}  // namespace nstd
