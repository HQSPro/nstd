#include "test.hpp"

namespace nstd {

TestGroup::TestGroup(std::string&& n, std::string&& f) : group(std::forward<std::string>(n)), file(std::forward<std::string>(f)) {}
TestGroup::TestGroup(const std::string& n, const std::string& f) : group(n), file(f) {}

void TestGroup::add_case(TestKind kind, std::string&& case_name) noexcept;
{
    cases.emplace_back(kind, std::forward<std::string>(case_name));
}

void TestGroup::set_code(std::string&& tests_code) noexcept { code = std::forward<std::string>(tests_code); }

const std::string& TestGroup::get_group_name() const noexcept { return group; }

const std::vector<TestCaseInfo>& TestGroup::get_cases() const noexcept { return cases; }

TestGroupManager& TestGroupManager::get_obj()
{
    static TestGroupManager tgm{};
    return tgm;
}

void TestGroupManager::add_test_group(TestGroup&& test_group)
{
    try
    {
        std::lock_guard<std::mutex> guard(tgm_mutex);
        groups.emplace_back(std::forward<TestGroup>(test_group));
    }
    catch(const std::exception& e)
    {
        // todo: Make the output error msg follow the log format.(Currently we don't have log
        // module.)
        std::cerr << e.what() + '\n';
        std::abort();
    }
}

}  // namespace nstd

// clang-format off
TEST(test_framwork_test,
#include <string>
#include <test.hpp>
[[utest(test_utest)]]
void test_utest()
{
    nstd::TestGroup tg0("test_framwork_test", __FILE__);
    assert("test_framwork_test" == tg0.get_group_name().c_str());
    nstd::TestGroup tg1(std::string("test_framwork_test"), std::string(__FILE__));
    assert("test_framwork_test" == tg1.get_group_name().c_str());
    tg0.add_case(nstd::TestKind::UNIT_TEST, std::string("test_utest"));
    assert("test_utest" == tg0.get_cases().front().c_str());
}
[[itest(test_itest)]]
void test_itest()
{
    nstd::TestGroup tg0("test_framwork_test", __FILE__);
    assert("test_framwork_test" == tg0.get_group_name().c_str());
    nstd::TestGroup tg1(std::string("test_framwork_test"), std::string(__FILE__));
    assert("test_framwork_test" == tg1.get_group_name().c_str());
    tg0.add_case(nstd::TestKind::INTEGRATION_TEST, std::string("test_itest"));
    assert("test_itest" == tg0.get_cases().front().c_str());
}
[[bench(test_bench)]]
void test_bench(nstd::BenchFunc& b)
{
    b = []{
        nstd::TestGroup tg0("test_framwork_test", __FILE__);
        assert("test_framwork_test" == tg0.get_group_name().c_str());
        nstd::TestGroup tg1(std::string("test_framwork_test"), std::string(__FILE__));
        assert("test_framwork_test" == tg1.get_group_name().c_str());
        tg0.add_case(nstd::TestKind::BENCHMARK, std::string("test_bench"));
        assert("test_bench" == tg0.get_cases().front().c_str());
    };
}
[[stress(test_stress)]]
void test_stress(nstd::StressFunc& s)
{
    s = []{
        nstd::TestGroup tg0("test_framwork_test", __FILE__);
        assert("test_framwork_test" == tg0.get_group_name().c_str());
        nstd::TestGroup tg1(std::string("test_framwork_test"), std::string(__FILE__));
        assert("test_framwork_test" == tg1.get_group_name().c_str());
        tg0.add_case(nstd::TestKind::STRESS_TEST, std::string("test_stress"));
        assert("test_stress" == tg0.get_cases().front().c_str());
    };
}
)
// clang-format on
