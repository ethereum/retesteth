/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/types/BlockchainTests/BlockchainTest.h>
#include <retesteth/testStructures/types/BlockchainTests/BlockchainTestFiller.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <string>

namespace test
{
/// Generate blockchain test from filler
dataobject::spDataObject FillTest(
    teststruct::BlockchainTestInFiller const& _testObject, TestSuite::TestSuiteOptions const& _opt);

/// Read and execute the test from the file
void RunTest(teststruct::BlockchainTestInFilled const& _testObject, TestSuite::TestSuiteOptions const& _opt);

/// Parse blockchain test fillers and Run/Fill tests
dataobject::spDataObject DoTests(dataobject::spDataObject& _input, TestSuite::TestSuiteOptions& _opt);
}
