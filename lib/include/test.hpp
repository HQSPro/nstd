#ifndef __NSTD_TEST_HPP__
#define __NSTD_TEST_HPP__

#include <string>
#include <vector>
#ifdef NSTD_TEST
#include <sstream>
#include <regex>
#ifdef private
#undef private
#endif
#ifdef protected
#undef protected
#endif
#define private public
#define protected public
#endif

namespace nstd {

typedef void (*BenchFunc)();
typedef void (*StressFunc)();

enum TestKind
{
    UNIT_TEST,
    INTEGRATION_TEST,
    DOC_TEST,
    BENCHMARK,
    STRESS_TEST,
};

struct TestCaseInfo {
    TestKind kind;
    std::string name;
}

class TestGroup {
private:
    std::string group;
    std::string file;
    std::string code;
    std::vector<TestCaseInfo> cases;

public:
    TestGroup(std::string&& group_name, std::string&& file);
    TestGroup(const std::string& group_name, const std::string& file);
    void add_case(TestKind kind, std::string&& case_name) noexcept;
    void set_code(std::string&& tests_code) noexcept;
    inline const std::string& get_group_name() const noexcept;
    inline const std::vector<TestCaseInfo>& get_cases() const noexcept;
};

class TestGroupManager {
private:
    std::vector<nstd::TestGroup> groups;
    static std::mutex tgm_mutex;
    TestGroupManager() = default;

public:
    static TestGroupManager& get_obj();
    void add_test_group(TestGroup&& test_group);
};

#ifdef NSTD_TEST

template <typename T>
T& make_test_group()
{
    static const T test_group{};
    return test_group;
}

#define TEST_PARENS_PROBE(...) _, 1
#define TEST_PARENS_CONDITION(a, b, ...) b
#define TEST_PARENS_CHECK(...) TEST_PARENS_CONDITION(__VA_ARGS__, 0)
#define TEST_PARENS_REMOVE_IMPL_0(...) #__VA_ARGS__
#define TEST_PARENS_REMOVE_IMPL_1_IMPL(...) #__VA_ARGS__
#define TEST_PARENS_REMOVE_IMPL_1(expr) TEST_PARENS_REMOVE_IMPL_1_IMPL expr
#define TEST_PARENS_REMOVE_IMPL(condition) TEST_CONCAT(TEST_PARENS_REMOVE_IMPL_, condition)
#define TEST_PARENS_REMOVE(expr) \
    TEST_PARENS_REMOVE_IMPL(TEST_PARENS_CHECK(TEST_PARENS_PROBE expr))(expr)

#define TEST(name, ...)                                                                            \
    class PhantomGroupTest##name {                                                                 \
    public:                                                                                        \
        PhantomGroupTest##name(I)                                                                  \
        {                                                                                          \
            std::istringstream txt{#__VA_ARGS__};                                                  \
            std::ostringstream code;                                                               \
            nstd::TestGroup tg(#name, __FILE__);                                                   \
            std::regex battr_pattern("^(.+\\};?|\\s*)(\\[\\[.*)");                                 \
            std::regex eattr_pattern("^(.*\\]\\])(.*)");                                           \
            std::regex utest_pattern("(utest\\()(\\w+)\\),?");                                     \
            std::regex itest_pattern("(itest\\()(\\w+)\\),?");                                     \
            std::regex bench_pattern("(bench\\()(\\w+)\\),?");                                     \
            std::regex stress_pattern("(stress\\()(\\w+)\\),?");                                   \
            bool bmatch = false;                                                                   \
            bool ematch = false;                                                                   \
            std::ostringstream attr;                                                               \
            for(std::string line; std::getline(txt, line);)                                        \
            {                                                                                      \
                if(!bmatch)                                                                        \
                {                                                                                  \
                    std::smatch r;                                                                 \
                    bmatch = std::regex_match(line, r, battr_pattern);                             \
                    if(bmatch)                                                                     \
                    {                                                                              \
                        attr << r[2];                                                              \
                        code << r[1];                                                              \
                    }                                                                              \
                }                                                                                  \
                std::smatch r;                                                                     \
                ematch = std::regex_match(line, r, eattr_pattern);                                 \
                if(bmatch && ematch)                                                               \
                {                                                                                  \
                    attr << r[1];                                                                  \
                    std::string attr_str = attr.str();                                             \
                    auto find_test       = [&r, &tg, &attr_str, &code, &bmatch, &ematch](          \
                                         nstd::TestKind kind, std::regex& pattern) -> bool { \
                        std::smatch tr;                                                            \
                        if(std::regex_search(attr_str, tr, pattern))                               \
                        {                                                                          \
                            std::string name = tr[2];                                              \
                            code << std::regex_replace(attr_str, pattern, "");                     \
                            << r[2];                                                               \
                            tg.add_case(kind, std::move(name));                                    \
                            bmatch = false;                                                        \
                            ematch = false;                                                        \
                            return true;                                                           \
                        }                                                                          \
                        else { return false; }                                                     \
                    };                                                                             \
                    if(find_test(nstd::TestKind::UNIT_TEST, utest_pattern)) { continue; }          \
                    if(find_test(nstd::TestKind::INTEGRATION_TEST, itest_pattern)) { continue; }   \
                    if(find_test(nstd::TestKind::BENCHMARK, bench_pattern)) { continue; }          \
                    if(find_test(nstd::TestKind::STRESS_TEST, stress_pattern)) { continue; }       \
                }                                                                                  \
                else if(bmatch) { attr << line; }                                                  \
                else                                                                               \
                {                                                                                  \
                    ematch = std::string::npos;                                                    \
                    code << line << "\n";                                                          \
                }                                                                                  \
            }                                                                                      \
            assert(!bmatch && !ematch);                                                            \
            tg.set_code(code.str());                                                               \
            nstd::TestCaseManager& tgm = nstd::TestCaseManager::get_obj();                         \
            tgm.add_test_group(std::move(tg));                                                     \
        }                                                                                          \
    };                                                                                             \
    [[maybe_unused]] static const PhantomGroupTest##name& _group_test_##name =                     \
        make_test_group<PhantomGroupTest##name>();
#define DOC_TEST(name, incs, ...)                                                        \
    class PhantomDocTest##name {                                                         \
    public:                                                                              \
        PhantomDocTest##name()                                                           \
        {                                                                                \
            std::string incs(TEST_PARENS_REMOVE(inc));                                   \
            while(std::string::npos != incs.find(","))                                   \
            {                                                                            \
                incs = incs.replace(incs.find(","), 1, "\n");                            \
            }                                                                            \
            std::istringstream explain(#__VA_ARGS__);                                    \
            std::ostringstream code;                                                     \
            code << incs << "\n";                                                        \
            bool bfind = false;                                                          \
            std::regex example_pattern("^(\\s*)(///|//!|\\*)(\\s*)(```)(\\s*)");         \
            std::regex code_pattern("^(\\s*)(///|//!|\\*)(\\s*)(.*)");                   \
            std::string nm = #name;                                                      \
            int count      = 0;                                                          \
            nstd::TestGroup tg(#name, __FILE__);                                         \
            for(std::string line; std::getline(explain, line);)                          \
            {                                                                            \
                if(!bfind)                                                               \
                {                                                                        \
                    bfind = std::regex_match(line, example_pattern);                     \
                    if(bfind)                                                            \
                    {                                                                    \
                        tg.add_case(nstd::TestKind::DOC_TEST,                            \
                                    std::move(nm + "_" + std::to_string(count)));        \
                        code << "void " << nm + "_" + std::to_string(count) << "() {\n"; \
                    }                                                                    \
                }                                                                        \
                else                                                                     \
                {                                                                        \
                    if(std::regex_match(line, example_pattern))                          \
                    {                                                                    \
                        code << "\n}\n";                                                 \
                        bfind = false;                                                   \
                        ++count;                                                         \
                    }                                                                    \
                    else                                                                 \
                    {                                                                    \
                        std::smatch r;                                                   \
                        std::regex_match(line, r, code_pattern);                         \
                        code << r[4] << "\n";                                            \
                    }                                                                    \
                }                                                                        \
            }                                                                            \
            assert(!bfind);                                                              \
            tg.set_code(code.str());                                                     \
            nstd::TestCaseManager& tgm = nstd::TestCaseManager::get_obj();               \
            tgm.add_test_group(std::move(tg));                                           \
        }                                                                                \
    };                                                                                   \
    [[maybe_unused]] inline static const PhantomDocTest##name& _doc_test_##name =        \
        make_test_group<PhantomDocTest##name>();                                         \
    __VA_ARGS__
#else
#define TEST_PP_EMPTY
#define TEST(name, ...) TEST_PP_EMPTY
#define DOC_TEST(name, incs, ...) __VA_ARGS__
#endif
}  // namespace nstd

#endif
