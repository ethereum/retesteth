/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/TestSuite.h>
#include <retesteth/ethObjects/common.h>
#include <string>

using namespace std;
namespace test
{
/// Generate blockchain test from filler
void FillTest(scheme_blockchainTestFiller const& _testObject, string const& _network,
    TestSuite::TestSuiteOptions const& _opt, DataObject& _testOut);

/// Read and execute the test from the file
void RunTest(DataObject const& _testObject, TestSuite::TestSuiteOptions const& _opt);

/// Parse blockchain test fillers and Run/Fill tests
DataObject DoTests(DataObject const& _input, TestSuite::TestSuiteOptions& _opt);
}
