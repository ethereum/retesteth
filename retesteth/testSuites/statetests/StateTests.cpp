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

/** @file StateTests.cpp
 * @author Dimitry Khokhlov <dimitry@ethereum.org>
 * @date 2016
 * General State Tests parser.
 */

#include "StateTests.h"
#include "StateTestsHelper.h"
#include <retesteth/Options.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testStructures/structures.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace test::teststruct;
using namespace test::session;
using namespace test::statetests;
namespace fs = boost::filesystem;

namespace test
{
spDataObject StateTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("StateTestSuite::doTests init"));
    if (_opt.doFilling)
    {
        spDataObject filledTest;
        GeneralStateTestFiller filler(_input);
        StateTestInFiller const& test = filler.tests().at(0);
        checkTestNameIsEqualToFileName(test.testName());
        if (Options::get().fillchain)
            filledTest = FillTestAsBlockchain(test);
        else
        {
            auto const filled = FillTest(test);
            if (filled->type() != DataType::Null)
                (*filledTest).addSubObject(test.testName(), filled);
            else
                return filled;
        }

        TestOutputHelper::get().registerTestRunSuccess();
        return filledTest;
    }
    else
    {
        if (Options::get().fillchain)
        {
            BlockchainTestInvalidSuite bcTestSuite;
            bcTestSuite.doTests(_input, _opt);
        }
        else
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("Parsing generalstate test"));
            TestOutputHelper::get().setCurrentTestName("----");
            GeneralStateTest filledTest(_input);

            // Just check the test structure if running with --checkhash
            if (Options::get().checkhash)
                return spDataObject();
            if (Options::get().getvectors)
            {
                filledTest.registerAllVectors();
                return spDataObject();
            }

            for (auto const& test : filledTest.tests())
            {
                RunTest(test);
                TestOutputHelper::get().registerTestRunSuccess();
            }
        }
    }
    return spDataObject();
}
}  // namespace test
