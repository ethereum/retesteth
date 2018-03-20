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
#include <testeth/ethObjects/state.h>
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

BOOST_AUTO_TEST_CASE(compareStates_noError)
{
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    CompareResult res = test::compareStates(expectState(expectData), state(postData));
    BOOST_REQUIRE(res == CompareResult::Success);
}

BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(compareStates_missingAccount, 1)
BOOST_AUTO_TEST_CASE(compareStates_missingAccount)
{
    std::cerr << "Expected 1 error: " << std::endl;
    DataObject expectData;
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    CompareResult res = test::compareStates(expectState(expectData), state(postData));
    BOOST_REQUIRE(res == CompareResult::MissingExpectedAccount);
}

BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(compareStates_shouldnoexist, 1)
BOOST_AUTO_TEST_CASE(compareStates_shouldnoexist)
{
    std::cerr << "Expected 1 error: " << std::endl;
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["shouldnotexist"] = "1";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    CompareResult res = test::compareStates(expectState(expectData), state(postData));
    BOOST_REQUIRE(res == CompareResult::AccountShouldNotExist);
}

BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(compareStates_wrongBalance, 1)
BOOST_AUTO_TEST_CASE(compareStates_wrongBalance)
{
    std::cerr << "Expected 1 error: " << std::endl;
    DataObject expectData;
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    CompareResult res = test::compareStates(expectState(expectData), state(postData));
    BOOST_REQUIRE(res == CompareResult::IncorrectBalance);
}

BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(compareStates_wrongNonce, 1)
BOOST_AUTO_TEST_CASE(compareStates_wrongNonce)
{
    std::cerr << "Expected 1 error: " << std::endl;
    DataObject expectData;
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x02";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    CompareResult res = test::compareStates(expectState(expectData), state(postData));
    BOOST_REQUIRE(res == CompareResult::IncorrectNonce);
}

BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(compareStates_multipleError, 2)
BOOST_AUTO_TEST_CASE(compareStates_multipleError)
{
    std::cerr << "Expected 2 errors: " << std::endl;
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x02";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x02";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    CompareResult res = test::compareStates(expectState(expectData), state(postData));
    BOOST_REQUIRE(res == CompareResult::IncorrectCode);
}

BOOST_AUTO_TEST_SUITE_END()
