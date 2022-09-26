/** @file DifficultyTest.h
 * Difficulty formula functions.
 */

#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <boost/filesystem/path.hpp>

namespace test
{
class DifficultyTestSuite : public TestSuite
{
public:
    DifficultyTestSuite(){};
    dataobject::spDataObject doTests(dataobject::spDataObject& _input, TestSuiteOptions& _opt) const override;
    TestSuite::TestPath suiteFolder() const override;
    TestSuite::FillerPath suiteFillerFolder() const override;
};

}  // namespace test
