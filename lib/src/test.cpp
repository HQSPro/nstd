#include "test.hpp"

namespace nstd {

#ifdef NSTD_TEST

TestGroup::TestGroup(TestKind test_kind, std::string group_name, std::string&& f)
    : kind(test_kind), group(std::move(group_name)), file(std::move(f))
{
}
TestGroup::TestGroup(TestKind test_kind,
                     std::string group_name,
                     std::string&& f,
                     std::vector<std::string>&& case_names,
                     std::string&& options,
                     std::string&& tests_code)
    : kind(test_kind),
      group(group_name),
      cases(std::move(case_names)),
      file(std::move(f)),
      make_options(std::move(options)),
      code(std::move(tests_code))
{
}

void TestGroup::add_case_name(std::string&& case_name)
{
    cases.emplace_back(std::forward<std::string>(case_name));
}

void TestGroup::set_tests_code(std::string&& tests_code)
{
    code = std::forward<std::string>(tests_code);
}
void TestGroup::set_make_options(std::string&& options)
{
    make_options = std::forward<std::string>(options);
}

const std::string& TestGroup::get_group_name() const { return group; }

TestGroupManager& TestGroupManager::get_obj()
{
    static TestGroupManager tgm{};
    return tgm;
}

const std::unordered_map<std::string, nstd::TestGroup>& TestGroupManager::get_groups() const
{
    return groups;
}
std::unordered_map<std::string, nstd::TestGroup>& TestGroupManager::get_groups() { return groups; }

void TestGroupManager::add_test_group(TestGroup&& test_group)
{
    try
    {
        std::lock_guard<std::mutex> guard(tgm_mutex);
        auto iter = groups.find(test_group.get_group_name());
        assert(groups.end() == iter);
        groups.emplace(
            std::make_pair(test_group.get_group_name(), std::forward<TestGroup>(test_group)));
    }
    catch(const std::exception& e)
    {
        // todo: Make the output error msg follow the log format.(Currently we don't have log
        // module.)
        std::cerr << e.what() + '\n';
        std::abort();
    }
}
#endif
} // namespace nstd
