#ifndef __NSTD_TEST_HPP__
#define __NSTD_TEST_HPP__

#include <string>
#include <vector>
#include "result.hpp"

namespace nstd {

enum TestKind
{
    UNIT_TEST,
    INTEGRATION_TEST,
    BENCHMARK,
    STRESS_TEST,
};

class TestCaseMeta
{
protected:
    TestKind kind;
    std::string name;
    std::string file;
};

class TestCaseImpl : public TestCaseMeta
{
    friend class TestCase;

private:
    std::string code;
    TestCaseImpl(TestKind k, std::string&& n, std::string&& f, std::string&& c);
};
class TestCase:
{
private:
    TestCaseImpl impl;

public:
    TestCase(TestKind kind, std::string&& name, std::string&& file, std::string&& code);
};

class TestCaseManager
{
private:
    std::vector<nstd::TestCase> cases;
    static std::mutex tcm_mutex;
    TestCaseManager() = default;

public:
    static TestCaseManager& get_obj();
    void add_test_case(TestKind kind, std::string&& name, std::string&& file, std::string&& code);
};

#ifdef NSTD_TEST
#ifdef private
#undef private
#define private public
#endif
#ifdef protected
#undef protected
#define protected public
#endif
#define UNIT_TEST(name, ...)                                                            \
    struct PhantomUnitTest##name                                                        \
    {                                                                                   \
        PhantomUnitTest##name()                                                         \
        {                                                                               \
            nstd::TestCaseManager& tcm = nstd::TestCaseManager::get_obj();              \
            tcm.add_test_case(nstd::TestKind::UNIT_TEST, #name, __FILE__, #__VA_ARGS__) \
        }                                                                               \
    };                                                                                  \
    [[maybe_unused]] static const PhantomUnitTest##name _unit_test_##name{};
#define INTEGRATION_TEST(name, ...)                                                            \
    struct PhantomIntegrationTest##name                                                        \
    {                                                                                          \
        PhantomIntegrationTest##name()                                                         \
        {                                                                                      \
            nstd::TestCaseManager& tcm = nstd::TestCaseManager::get_obj();                     \
            tcm.add_test_case(nstd::TestKind::INTEGRATION_TEST, #name, __FILE__, #__VA_ARGS__) \
        }                                                                                      \
    };                                                                                         \
    [[maybe_unused]] static const PhantomIntegrationTest##name _intergration_test_##name{};
#define BENCHMARK(name, ...)                                                            \
    struct PhantomBenchmark##name                                                       \
    {                                                                                   \
        PhantomBenchmark##name()                                                        \
        {                                                                               \
            nstd::TestCaseManager& tcm = nstd::TestCaseManager::get_obj();              \
            tcm.add_test_case(nstd::TestKind::BENCHMARK, #name, __FILE__, #__VA_ARGS__) \
        }                                                                               \
    };                                                                                  \
    [[maybe_unused]] static const PhantomBenchmark##name _benchmark_##name{};

#define STRESS_TEST(name, ...)                                                            \
    struct PhantomStressTest##name                                                        \
    {                                                                                     \
        PhantomStressTest##name()                                                         \
        {                                                                                 \
            nstd::TestCaseManager& tcm = nstd::TestCaseManager::get_obj();                \
            tcm.add_test_case(nstd::TestKind::STRESS_TEST, #name, __FILE__, #__VA_ARGS__) \
        }                                                                                 \
    };                                                                                    \
    [[maybe_unused]] static const PhantomStressTest##name _stress_test_##name{};
#endif
} // namespace nstd

#endif
