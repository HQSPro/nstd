
#include "log.hpp"

namespace nstd {
const std::chrono::steady_clock ProcStart::proc_start = std::chrono::steady_clock::now();
thread_local std::ostringstream Logger::buf           = std::ostringstream{};
ILogMask::ILogMask(unsigned int m) : mask(m) {}

}  // namespace nstd
