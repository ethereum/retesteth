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
#include <retesteth/session/RPCSession.h>
#include <boost/filesystem/path.hpp>

#include <retesteth/testStructures/types/ethereum.h>

namespace test
{

// Get Remote State From Client
struct StateTooBig : BaseEthException
{
    StateTooBig(){}
};
State getRemoteState(SessionInterface& _session);

// Check that test has data object
void checkDataObject(DataObject const& _input);

// Check that test file has only one test
void checkOnlyOneTest(DataObject const& _input);

// Check that test file has at least one test
void checkAtLeastOneTest(DataObject const& _input);

// Check test name in the file is equal to the test name of the file
void checkTestNameIsEqualToFileName(DataObject const& _input);
void checkTestNameIsEqualToFileName(string const& _testName);


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
string CompareResultToString(CompareResult res);

// Compafre expected StateIncomplete against remote client (get post state data on the fly with session)
void compareStates(StateBase const& _stateExpect, SessionInterface& _session);

// Compare expected StateIncomplete against post state State
void compareStates(StateBase const& _stateExpect, State const& _statePost);

// json trace vm
void printVmTrace(SessionInterface& _session, FH32 const& _trHash, FH32 const& _stateRoot);
}
