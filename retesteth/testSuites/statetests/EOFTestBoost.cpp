#include "retesteth/testSuites/TestFixtures.h"
#include <retesteth/testSuites/EOFTest.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace test;
using namespace boost::unit_test;
namespace fs = boost::filesystem;


TestSuite::TestPath EOFTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("EOFTests" + m_fillerPathAdd));
}

TestSuite::FillerPath EOFTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / string("EOFTestsFiller" + m_fillerPathAdd));
}

using EOFTestsFixture = TestFixture<EOFTestSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(EOFTestsFixture, "EOFTests")
BOOST_FIXTURE_TEST_SUITE(EOFTests, EOFTestsFixture)

BOOST_AUTO_TEST_CASE(efExample) {}

BOOST_AUTO_TEST_SUITE_END()
