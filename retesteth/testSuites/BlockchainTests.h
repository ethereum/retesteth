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
/** @file BlockchainTests.h
 * BlockchainTests functions.
 */

#pragma once
#include <retesteth/DataObject.h>
#include <retesteth/TestSuite.h>
#include <boost/filesystem/path.hpp>

namespace test
{
class BlockchainTestSuite : public TestSuite
{
public:
    DataObject doTests(DataObject const& _input, TestSuiteOptions& _opt) const override;
    boost::filesystem::path suiteFolder() const override;
    boost::filesystem::path suiteFillerFolder() const override;
};
}
