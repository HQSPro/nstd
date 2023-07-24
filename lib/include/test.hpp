#ifndef __NSTD_TEST_HPP__
#define __NSTD_TEST_HPP__

namespace nstd {

#ifdef NSTD_TEST
#include <string>
#include <vector>
#include <unordered_map>
#include "result.hpp"

enum TestKind
{
    UNIT_TEST,
    INTEGRATION_TEST,
    DOC_TEST,
    BENCHMARK,
    STRESS_TEST,
};

class TestGroupMeta
{
protected:
    TestKind kind;
    std::string group;
    std::vector<std::string> cases;
    std::string file;
    std::string make_options;
};

class TestGroup : public TestGroupMeta
{
private:
    std::string code;
public:
    TestGroup(TestKind test_kind, std::string group_name, std::string&& file);
    TestGroup(TestKind test_kind, std::string group_name, std::string&& file, std::vector<std::string>&& case_names, std::string&& make_options, std::string&& tests_code);
    void add_case_name(std::string&& case_name);
    void set_tests_code(std::string&& tests_code);
    void set_make_options(std::string&& make_options);
    inline const std::string& get_group_name() const;
};

class TestGroupManager
{
private:
    std::unordered_map<std::string, nstd::TestGroup> groups;
    static std::mutex tgm_mutex;
    TestGroupManager() = default;

public:
    static TestGroupManager& get_obj();
    void add_test_group(TestGroup&& test_group);
    inline const std::unordered_map<std::string, nstd::TestGroup>& get_groups() const;
    inline std::unordered_map<std::string, nstd::TestGroup>& get_groups();
};

#ifdef private
#undef private
#endif
#ifdef protected
#undef protected
#endif
#define private public
#define protected public

#define TEST_PARENS_PROBE(...) _, 1
#define TEST_PARENS_CONDITION(a, b, ...) b
#define TEST_PARENS_CHECK(...) TEST_PARENS_CONDITION(__VA_ARGS__, 0)
#define TEST_PARENS_REMOVE_IMPL_0(...) #__VA_ARGS__
#define TEST_PARENS_REMOVE_IMPL_1_IMPL(...) #__VA_ARGS__
#define TEST_PARENS_REMOVE_IMPL_1(expr) TEST_PARENS_REMOVE_IMPL_1_IMPL expr
#define TEST_PARENS_REMOVE_IMPL(condition) TEST_CONCAT(TEST_PARENS_REMOVE_IMPL_, condition)
#define TEST_PARENS_REMOVE(expr) \
    TEST_PARENS_REMOVE_IMPL(TEST_PARENS_CHECK(TEST_PARENS_PROBE expr))(expr)

#define UNIT_TEST(name, ...)                                                            \
    struct PhantomUnitTest##name                                                        \
    {                                                                                   \
        PhantomUnitTest##name()                                                         \
        {                                                                               \
            std::string txt{#__VA_ARGS__}; \
            
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
#define DOC_TEST(name, inc, ...)                                                  \
    struct PhantomDocTest##name                                                   \
    {                                                                             \
        PhantomDocTest##name()                                                    \
        {                                                                         \
            std::string incs(TEST_PARENS_REMOVE(inc));                            \
            while(std::string::npos != incs.find(","))                            \
            {                                                                     \
                incs = incs.replace(incs.find(","), 1, "\n");                     \
            }                                                                     \
            std::string explain(#__VA_ARGS__);                                    \
            std::string nm = #name;                                               \
            int count      = 0;                                                   \
            std::string::size_type bpos, epos;                                    \
            bpos = explain.find("```");                                           \
            while(std::string::npos != bpos)                                      \
            {                                                                     \
                epos = explain.find("```", bpos + 3);                             \
                assert(std::string::npos != epos);                                \
                nstd::TestCaseManager& tcm = nstd::TestCaseManager::get_obj();    \
                tcm.add_test_case(nstd::TestKind::DOC_TEST,                       \
                                  nm + std::to_string(count),                     \
                                  __FILE__,                                       \
                                  incs + "\nint main() {\n" +                     \
                                      explain.substr(bpos + 3, epos - bpos - 3) + \
                                      "\nreturn 0;\n}");                          \
                ++count;                                                          \
                explain = explain.substr(epos + 3);                               \
                bpos    = explain.find("```");                                    \
            }                                                                     \
        }                                                                         \
    };                                                                            \
    [[maybe_unused]] static const PhantomDocTest##name _doc_test_##name{};        \
    __VA_ARGS__
#else
#define TEST_PP_EMPTY
#define UNIT_TEST(name, ...) TEST_PP_EMPTY
#define INTEGRATION_TEST(name, ...) TEST_PP_EMPTY
#define BENCHMARK(name, ...) TEST_PP_EMPTY
#define STRESS_TEST(name, ...) TEST_PP_EMPTY
#define DOC_TEST(name, inc, ...) __VA_ARGS__
#endif
} // namespace nstd

#endif
