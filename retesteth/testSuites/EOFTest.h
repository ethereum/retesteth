#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <retesteth/testSuites/statetests/StateTests.h>
#include <boost/filesystem/path.hpp>

namespace test
{
class EOFTestSuite : public TestSuite
{
public:
    dataobject::spDataObject doTests(dataobject::spDataObject& _input, TestSuiteOptions& _opt) const override;
    TestSuite::TestPath suiteFolder() const override;
    TestSuite::FillerPath suiteFillerFolder() const override;
};

}  // namespace test
