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
#include <retesteth/session/Session.h>
#include <boost/filesystem/path.hpp>
#include <retesteth/testStructures/types/ethereum.h>

namespace test
{

enum CompareResult
{
    Success,
    AccountShouldNotExist,
    MissingExpectedAccount,
    IncorrectBalance,
    IncorrectNonce,
    IncorrectCode,
    IncorrectStorage,
    None
};
std::string CompareResultToString(CompareResult res);


// Get Remote State From Client
struct StateTooBig : UpwardsException
{
    StateTooBig() : UpwardsException("StateTooBig") {}
};
State getRemoteState(test::session::SessionInterface& _session);

// Check that test has data object
void checkDataObject(DataObject const& _input);

// Check that test file has only one test
void checkOnlyOneTest(DataObject const& _input);

// Check that test file has at least one test
void checkAtLeastOneTest(DataObject const& _input);

// Check test name in the file is equal to the test name of the file
void checkTestNameIsEqualToFileName(DataObject const& _input);
void checkTestNameIsEqualToFileName(std::string const& _testName);


// Compafre expected StateIncomplete against remote client (get post state data on the fly with session)
void compareStates(StateBase const& _stateExpect, test::session::SessionInterface& _session);

// Compare expected StateIncomplete against post state State
void compareStates(StateBase const& _stateExpect, State const& _statePost);

// json trace vm
struct VMtraceinfo
{
    VMtraceinfo(test::session::SessionInterface& _info, FH32 const& _trHash, FH32 const& _stateRoot, std::string const& _trName)
      : session(_info), trHash(_trHash), stateRoot(_stateRoot), trName(_trName)
    {}
    test::session::SessionInterface& session;
    FH32 const& trHash;
    FH32 const& stateRoot;
    std::string const& trName;
};
void printVmTrace(VMtraceinfo const& _info);

// Validate transaction exception
void compareTransactionException(spTransaction const& _tr, MineBlocksResult const& _mRes, std::string const& _testException);

// Verify filled json structures
void verifyFilledTest(DataObject const& _want, DataObject const& _have);
void verifyFilledTest(DataObject const& _want, DataObject const& _have, FORK const& _net);
void verifyFilledTestRecursive(DataObject const& _want, DataObject const& _have, std::string& _debug);

// Configure transaction chainID by current client config
void modifyTransactionChainIDByNetwork(test::Transaction const& _tr, FORK const& _fork);

}
