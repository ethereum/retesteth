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
/** @file ethObjectsTests.cpp
 * Unit tests for ethObjects functions.
 */

#include <testeth/ethObjects/object.h>
#include <testeth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace test;

BOOST_FIXTURE_TEST_SUITE(EthObjectsSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(object_stringIntegerType_correctHex)
{
	BOOST_CHECK(object::stringIntegerType("0x11223344") == object::DigitsType::HexPrefixed);
	BOOST_CHECK(object::stringIntegerType("0x1122334") == object::DigitsType::HexPrefixed);
	BOOST_CHECK(object::stringIntegerType("0x01234567890") == object::DigitsType::HexPrefixed);
	BOOST_CHECK(object::stringIntegerType("0x11223344abcdef") == object::DigitsType::HexPrefixed);
	BOOST_CHECK(object::stringIntegerType("0xabcdef") == object::DigitsType::HexPrefixed);
	BOOST_CHECK(object::stringIntegerType("0x11223344abcdeff") == object::DigitsType::HexPrefixed);

	BOOST_CHECK(object::stringIntegerType("11223344abcdef") == object::DigitsType::Hex);
	BOOST_CHECK(object::stringIntegerType("abcdef") == object::DigitsType::Hex);
	BOOST_CHECK(object::stringIntegerType("11223344abcdeff") == object::DigitsType::Hex);
}

BOOST_AUTO_TEST_CASE(object_stringIntegerType_correctDecimal)
{
	BOOST_CHECK(object::stringIntegerType("11223344") == object::DigitsType::Decimal);
	BOOST_CHECK(object::stringIntegerType("1122334") == object::DigitsType::Decimal);
	BOOST_CHECK(object::stringIntegerType("01234567890") == object::DigitsType::Decimal);
}

BOOST_AUTO_TEST_CASE(object_stringIntegerType_otherTypes)
{
	BOOST_CHECK(object::stringIntegerType("0x11223344z") == object::DigitsType::String);
	BOOST_CHECK(object::stringIntegerType("0x1122334s") == object::DigitsType::String);
	BOOST_CHECK(object::stringIntegerType("0x01234567890r") == object::DigitsType::String);
	BOOST_CHECK(object::stringIntegerType("0x11223344abttcdef") == object::DigitsType::String);
	BOOST_CHECK(object::stringIntegerType("0xabcdefk") == object::DigitsType::String);
	BOOST_CHECK(object::stringIntegerType("0xll11223344abcdeff") == object::DigitsType::String);

	BOOST_CHECK(object::stringIntegerType("11223r344abcdef") == object::DigitsType::String);
	BOOST_CHECK(object::stringIntegerType("abcdefrr") == object::DigitsType::String);
	BOOST_CHECK(object::stringIntegerType("11223344abcdeffzz") == object::DigitsType::String);
}

BOOST_AUTO_TEST_SUITE_END()
