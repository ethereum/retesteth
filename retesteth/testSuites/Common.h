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
#include <retesteth/RPCSession.h>
#include <boost/filesystem/path.hpp>

namespace test
{
struct ExpectInfo
{
    ExpectInfo()
      : postHash(string()), logHash(string()), trHash(string()), expectState(DataObject())
    {}
    ExpectInfo(scheme_expectState const& _expectState)
      : postHash(string()), logHash(string()), trHash(string()), expectState(_expectState)
    {}
    string postHash;
    string logHash;
    string trHash;
    scheme_expectState expectState;
};

// Check post condition on a client
bool checkExpectSection(
    RPCSession& _session, ExpectInfo const& _expectedInfo, scheme_remoteState& _remoteState);

enum StateRequest
{
    AttemptFullPost,
    NoPost
};

// Get Remote State From Client
scheme_remoteState getRemoteState(RPCSession& _session, StateRequest _stateRequest);

// Check that test has data object
void checkDataObject(DataObject const& _input);

// Check that test file has only one test
void checkOnlyOneTest(DataObject const& _input);

// Check that test file has at least one test
void checkAtLeastOneTest(DataObject const& _input);

// Check test name in the file is equal to the test name of the file
void checkTestNameIsEqualToFileName(DataObject const& _input);

// Compare states with session asking post state data on the fly
CompareResult compareStates(scheme_expectState const& _stateExpect, RPCSession& _session);
CompareResult compareStates(scheme_state const& _stateExpect, scheme_state const& _statePost);

// Get account from remote state. inline function
scheme_account remoteGetAccount(RPCSession& _session, string const& _account,
    string const& _latestBlockNumber, size_t& _totalSize);
}
