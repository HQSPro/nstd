#include "test.hpp"

namespace nstd {
TestCaseImpl::TestCaseImpl(TestKind k, std::string&& n, std::string&& f, std::string&& c)
    : kind(k), name(std::move(n)), file(std::move(f)), code(std::move(c))
{
}
TestCase::TestCase(TestKind kind, std::string&& name, std::string&& file, std::string&& code)
    : impl(kind, std::move(name), std::move(file), std::move(code))
{
}
TestCaseManager& TestCaseManager::get_obj()
{
    static TestCaseManager tcm{};
    return tcm;
}
void TestCaseManager::add_test_case(TestKind kind,
                                    std::string&& name,
                                    std::string&& file,
                                    std::string&& code)
{
    try
    {
        std::lock_guard<std::mutex> guard(tcm_mutex);
        cases.emplace_back(kind, std::move(name), std::move(file), std::move(code));
    }
    catch(const std::exception& e)
    {
        // todo: Make the output error msg follow the log format.(Currently we don't have log
        // module.)
        std::cerr << e.what() + '\n';
        std::abort();
    }
}
} // namespace nstd
