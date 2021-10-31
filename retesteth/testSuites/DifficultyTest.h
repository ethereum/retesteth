/** @file DifficultyTest.h
 * Difficulty formula functions.
 */

#pragma once
#include <dataObject/DataObject.h>
#include <retesteth/TestSuite.h>
#include <boost/filesystem/path.hpp>

namespace test
{
class DifficultyTestSuite : public TestSuite
{
public:
    DifficultyTestSuite(){};
    spDataObject doTests(spDataObject& _input, TestSuiteOptions& _opt) const override;
    TestSuite::TestPath suiteFolder() const override;
    TestSuite::FillerPath suiteFillerFolder() const override;
};

}  // namespace test
