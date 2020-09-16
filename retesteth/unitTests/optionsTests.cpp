#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace test;

namespace test
{
class TestOptions
{
public:
    TestOptions(int argc = 0, const char** argv = 0) : m_opt(argc, argv) {}
    Options const& get() { return m_opt; }

private:
    Options m_opt;
};
}  // namespace test

BOOST_FIXTURE_TEST_SUITE(OptionsSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(options_singletestA)
{
    const char* argv[] = {"./retesteth", "--", "--singletest", "filename"};
    TestOptions opt(4, argv);
    BOOST_CHECK(opt.get().singleTestName == "filename");
}

BOOST_AUTO_TEST_CASE(options_singletestAsB)
{
    const char* argv[] = {"./retesteth", "--", "--singletest", "filename/testname"};
    TestOptions opt(4, argv);
    BOOST_CHECK(opt.get().singleTestName == "filename");
    BOOST_CHECK(opt.get().singleSubTestName == "testname");
}

BOOST_AUTO_TEST_SUITE_END()
