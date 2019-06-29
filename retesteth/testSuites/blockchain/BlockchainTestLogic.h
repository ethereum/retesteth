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
bool FillTest(
    scheme_blockchainTestFiller const& _testObject, string const& _network, DataObject& _testOut);

/// Read and execute the test from the file
bool RunTest(DataObject const& _testObject);

/// Parse blockchain test fillers and Run/Fill tests
DataObject DoTests(DataObject const& _input, TestSuite::TestSuiteOptions& _opt);
}
