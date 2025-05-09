/*
This file is part of cpp-ethereum.

cpp-ethereum is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cpp-ethereum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file StateTests.h
 * StateTests functions.
 */

#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <boost/filesystem/path.hpp>

namespace test
{

class StateTestSuite : public TestSuite
{
public:
    dataobject::spDataObject doTests(dataobject::spDataObject& _input, TestSuiteOptions& _opt) const override;
    TestSuite::TestPath suiteFolder() const override;
    TestSuite::FillerPath suiteFillerFolder() const override;
};

class LegacyConstantinopleStateTestSuite : public StateTestSuite
{
protected:
    bool legacyTestSuiteFlag() const override { return true; }

public:
    TestSuite::TestPath suiteFolder() const override;
    TestSuite::FillerPath suiteFillerFolder() const override;
};

class LegacyCancunStateTestSuite : public StateTestSuite
{
protected:
    bool legacyTestSuiteFlag() const override { return true; }

public:
    TestSuite::TestPath suiteFolder() const override;
    TestSuite::FillerPath suiteFillerFolder() const override;
};


#define REGISTER_STATESUITE(NAME)           \
    class NAME : public StateTestSuite      \
    {                                       \
    public:                                 \
        TestSuite::TestPath suiteFolder() const override;        \
        TestSuite::FillerPath suiteFillerFolder() const override;\
    };

REGISTER_STATESUITE(StateTestVMSuite)
REGISTER_STATESUITE(StateTestShanghaiSuite)
}  // namespace test
