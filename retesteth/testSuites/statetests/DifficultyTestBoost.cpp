#include "retesteth/testSuites/TestFixtures.h"
#include <retesteth/testSuites/DifficultyTest.h>
#include <boost/test/unit_test.hpp>

using namespace test;
using namespace boost::unit_test;
namespace fs = boost::filesystem;

TestSuite::TestPath DifficultyTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("DifficultyTests" + m_fillerPathAdd));
}

TestSuite::FillerPath DifficultyTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / std::string("DifficultyTestsFiller" + m_fillerPathAdd));
}

using DifficultyTestsFixture = TestFixture<DifficultyTestSuite, DefaultFlags>;

// For some weird reason this function only works if file is placed inside statetests folder
ETH_REGISTER_DYNAMIC_TEST_SEARCH(DifficultyTestsFixture, "DifficultyTests")

BOOST_FIXTURE_TEST_SUITE(DifficultyTests, DifficultyTestsFixture)
BOOST_AUTO_TEST_CASE(dfArrowGlacier) {}
BOOST_AUTO_TEST_CASE(dfByzantium) {}
BOOST_AUTO_TEST_CASE(dfConstantinople) {}
BOOST_AUTO_TEST_CASE(dfEIP2384) {}
BOOST_AUTO_TEST_CASE(dfExample) {}
BOOST_AUTO_TEST_CASE(dfFrontier) {}
BOOST_AUTO_TEST_CASE(dfGrayGlacier) {}
BOOST_AUTO_TEST_CASE(dfHomestead) {}

BOOST_AUTO_TEST_SUITE_END()
