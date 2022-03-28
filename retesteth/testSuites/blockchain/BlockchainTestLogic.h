/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <retesteth/testStructures/types/BlockchainTests/BlockchainTest.h>
#include <retesteth/testStructures/types/BlockchainTests/BlockchainTestFiller.h>
#include <string>

using namespace std;
namespace test
{
/// Generate blockchain test from filler
spDataObject FillTest(BlockchainTestInFiller const& _testObject, TestSuite::TestSuiteOptions const& _opt);

/// Read and execute the test from the file
void RunTest(BlockchainTestInFilled const& _testObject, TestSuite::TestSuiteOptions const& _opt);

/// Parse blockchain test fillers and Run/Fill tests
spDataObject DoTests(spDataObject& _input, TestSuite::TestSuiteOptions& _opt);
}
