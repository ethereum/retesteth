
#pragma once
#include <retesteth/TestSuite.h>
#include <retesteth/testSuites/Common.h>

namespace test
{
/// Generate blockchain test from filler
void FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
    TestSuite::TestSuiteOptions const&, DataObject& _testOut);

}  // namespace test
