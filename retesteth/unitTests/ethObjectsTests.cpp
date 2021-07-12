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

#include <retesteth/TestOutputHelper.h>
#include <retesteth/dataObject/ConvertFile.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/testStructures/configs/ClientConfigFile.h>
#include <retesteth/testStructures/types/ethereum.h>
#include <retesteth/testSuites/Common.h>
#include <boost/test/unit_test.hpp>
#include <thread>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::teststruct;

BOOST_FIXTURE_TEST_SUITE(EthObjectsSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_lastToFirst)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
    data.setKeyPos("key3", 0);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data3");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data2");
    TestOutputHelper::registerTestRunSuccess();
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_lastToMid)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key3", 1);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data3");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data2");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_lastTolast)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key3", 2);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data2");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data3");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_midToFirst)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key2", 0);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data2");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data3");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_midTomid)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key2", 1);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data2");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data3");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_midToLast)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key2", 2);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data3");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data2");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_midToLast2)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data["key4"] = "data4";
	data.setKeyPos("key2", 3);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data3");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key4");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data4");
    BOOST_CHECK(data.getSubObjects().at(3)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(3)->asString() == "data2");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_midToMid2)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data["key4"] = "data4";
	data.setKeyPos("key2", 2);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data3");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data2");
    BOOST_CHECK(data.getSubObjects().at(3)->getKey() == "key4");
    BOOST_CHECK(data.getSubObjects().at(3)->asString() == "data4");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_firstTofirst)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key1", 0);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data2");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data3");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_firstTomid)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key1", 1);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data2");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data1");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data3");
}

BOOST_AUTO_TEST_CASE(dataobject_setKeyPos_firstToLast)
{
	DataObject data;
	data["key1"] = "data1";
	data["key2"] = "data2";
	data["key3"] = "data3";
	data.setKeyPos("key1", 2);
    BOOST_CHECK(data.getSubObjects().at(0)->getKey() == "key2");
    BOOST_CHECK(data.getSubObjects().at(0)->asString() == "data2");
    BOOST_CHECK(data.getSubObjects().at(1)->getKey() == "key3");
    BOOST_CHECK(data.getSubObjects().at(1)->asString() == "data3");
    BOOST_CHECK(data.getSubObjects().at(2)->getKey() == "key1");
    BOOST_CHECK(data.getSubObjects().at(2)->asString() == "data1");
}

BOOST_AUTO_TEST_CASE(object_stringIntegerType_correctHex)
{
    BOOST_CHECK(stringIntegerType("0x11223344") == DigitsType::HexPrefixed);
    BOOST_CHECK(stringIntegerType("0x1122334") == DigitsType::UnEvenHexPrefixed);
    BOOST_CHECK(stringIntegerType("0x01234567890") == DigitsType::UnEvenHexPrefixed);
    BOOST_CHECK(stringIntegerType("0x11223344abcdef") == DigitsType::HexPrefixed);
    BOOST_CHECK(stringIntegerType("0xabcdef") == DigitsType::HexPrefixed);
    BOOST_CHECK(stringIntegerType("0x11223344abcdeff") == DigitsType::UnEvenHexPrefixed);

    BOOST_CHECK(stringIntegerType("11223344abcdef") == DigitsType::Hex);
    BOOST_CHECK(stringIntegerType("abcdef") == DigitsType::Hex);
    BOOST_CHECK(stringIntegerType("11223344abcdeff") == DigitsType::UnEvenHex);
}

BOOST_AUTO_TEST_CASE(object_stringIntegerType_correctDecimal)
{
    BOOST_CHECK(stringIntegerType("11223344") == DigitsType::Decimal);
    BOOST_CHECK(stringIntegerType("1122334") == DigitsType::Decimal);
    BOOST_CHECK(stringIntegerType("01234567890") == DigitsType::Decimal);
    BOOST_CHECK(stringIntegerType("0000000000000000000000000000000000000000") == DigitsType::Decimal);
    BOOST_CHECK(stringIntegerType("3535353535353535353535353535353535353535") == DigitsType::Decimal);
}

BOOST_AUTO_TEST_CASE(object_stringIntegerType_otherTypes)
{
    BOOST_CHECK(stringIntegerType("0x11223344z") == DigitsType::String);
    BOOST_CHECK(stringIntegerType("0x1122334s") == DigitsType::String);
    BOOST_CHECK(stringIntegerType("0x01234567890r") == DigitsType::String);
    BOOST_CHECK(stringIntegerType("0x11223344abttcdef") == DigitsType::String);
    BOOST_CHECK(stringIntegerType("0xabcdefk") == DigitsType::String);
    BOOST_CHECK(stringIntegerType("0xll11223344abcdeff") == DigitsType::String);

    BOOST_CHECK(stringIntegerType("11223r344abcdef") == DigitsType::String);
    BOOST_CHECK(stringIntegerType("abcdefrr") == DigitsType::String);
    BOOST_CHECK(stringIntegerType("11223344abcdeffzz") == DigitsType::String);
}


void testCompareResult(DataObject const& _exp, DataObject const& _post, CompareResult _expResult, size_t _errCount = 2)
{
    try
    {
        test::compareStates(StateIncomplete(_exp), State(_post));
        ETH_FAIL_REQUIRE(_expResult == CompareResult::Success);
    }
    catch (test::EthError const& _ex)
    {
        ETH_FAIL_REQUIRE(string(_ex.what()).rfind(CompareResultToString(_expResult)) != string::npos);
        ETH_FAIL_REQUIRE(TestOutputHelper::get().getErrors().size() == _errCount);
    }
    TestOutputHelper::get().resetErrors();
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
    testCompareResult(expectData, postData, CompareResult::Success);
}

BOOST_AUTO_TEST_CASE(compareStates_missingAccount)
{
    DataObject expectData;
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::MissingExpectedAccount);
}

BOOST_AUTO_TEST_CASE(compareStates_shouldnoexist)
{
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["shouldnotexist"] = "1";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::AccountShouldNotExist);
}

BOOST_AUTO_TEST_CASE(compareStates_wrongBalance)
{
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
    testCompareResult(expectData, postData, CompareResult::IncorrectBalance);
}

BOOST_AUTO_TEST_CASE(compareStates_wrongNonce)
{
    DataObject expectData;
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082123";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x02";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82123";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::IncorrectNonce);
}

BOOST_AUTO_TEST_CASE(compareStates_multipleError)
{
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082123";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x02";
    expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x02";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082123";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::IncorrectCode, 3);
}

BOOST_AUTO_TEST_CASE(compareStates_accountShouldNotExistAndItsNot)
{
	DataObject expectData;
	expectData["0xb94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["shouldnotexist"] = "1";
	DataObject postData;
	postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
	postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
	postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
	postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::Success);
}

BOOST_AUTO_TEST_CASE(compareStates_prefixedZeros)
{
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] =
        DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::Success);
}

BOOST_AUTO_TEST_CASE(compareStates_prefixedZeros2)
{
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x82124";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] =
        DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::Success);
}

BOOST_AUTO_TEST_CASE(compareStates_storageEmpty)
{
    DataObject expectStorage;
    expectStorage["0x01"] = "0x00";
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = expectStorage;
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] =
        DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::Success);
}

BOOST_AUTO_TEST_CASE(compareStates_storageIncorrect)
{
    DataObject expectStorage;
    expectStorage["0x01"] = "0x01";
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = expectStorage;
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] =
        DataObject(DataType::Object);
    testCompareResult(expectData, postData, CompareResult::IncorrectStorage);
}

BOOST_AUTO_TEST_CASE(compareStates_storageCorrect)
{
    DataObject expectStorage;
    expectStorage["0x01"] = "0x01";
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = expectStorage;
    DataObject postStorage;
    postStorage["0x01"] = "0x01";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = postStorage;
    testCompareResult(expectData, postData, CompareResult::Success);
}

BOOST_AUTO_TEST_CASE(compareStates_storageMissingOnPost)
{
    DataObject expectStorage;
    expectStorage["0x01"] = "0x01";
    expectStorage["0x02"] = "0x01";
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = expectStorage;
    DataObject postStorage;
    postStorage["0x01"] = "0x01";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = postStorage;
    testCompareResult(expectData, postData, CompareResult::IncorrectStorage);
}

BOOST_AUTO_TEST_CASE(compareStates_storageMissingOnExpect)
{
    DataObject expectStorage;
    expectStorage["0x01"] = "0x01";
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = expectStorage;
    DataObject postStorage;
    postStorage["0x01"] = "0x01";
    postStorage["0x02"] = "0x01";
    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = postStorage;
    testCompareResult(expectData, postData, CompareResult::IncorrectStorage, 2);
}

void ExpectVsPost(string const& _expectKey, string const& _expectVal, string const& _postKey, string const& _postVal,
    CompareResult _res, string const& _doubleVal = "0x02", size_t _errCount = 2)
{
    ETH_LOG("Exp(" + _expectKey + ":" + _expectVal + ") vs Post(" + _postKey + "," + _postVal + ") dd: " + _doubleVal, 0);
    DataObject expectStorage(DataType::Object);
    if (_expectKey != "--")
        expectStorage[_expectKey] = _expectVal;
    expectStorage["0x02"] = "0x02";
    DataObject expectData;
    expectData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = expectStorage;
    DataObject postStorage(DataType::Object);
    if (_postKey != "--")
        postStorage[_postKey] = _postVal;
    postStorage["0x02"] = _doubleVal;

    DataObject postData;
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["balance"] = "0x082124";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["code"] = "0x1234";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["nonce"] = "0x01";
    postData["0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"]["storage"] = postStorage;
    testCompareResult(expectData, postData, _res, _errCount);
}

BOOST_AUTO_TEST_CASE(compareStates_storageCombinations)
{
    // Do not allow 0x  as VALUE cases are commented
    // ExpectVsPost("0x", "0x", "--", "--", CompareResult::Success);
    // ExpectVsPost("0x", "0x00", "--", "--", CompareResult::Success);
    // ExpectVsPost("0x00", "0x", "--", "--", CompareResult::Success);
    ExpectVsPost("0x00", "0x00", "--", "--", CompareResult::Success);
    // ExpectVsPost("0x01", "0x", "--", "--", CompareResult::Success);
    ExpectVsPost("0x01", "0x00", "--", "--", CompareResult::Success);
    // ExpectVsPost("0x", "0x01", "--", "--", CompareResult::IncorrectStorage);
    ExpectVsPost("0x00", "0x01", "--", "--", CompareResult::IncorrectStorage);
    ExpectVsPost("0x01", "0x01", "--", "--", CompareResult::IncorrectStorage);
    // ExpectVsPost("0x", "0x01", "0x", "0x01", CompareResult::Success);
    // ExpectVsPost("0x00", "0x01", "0x", "0x01", CompareResult::Success);
    // ExpectVsPost("0x", "0x01", "0x00", "0x01", CompareResult::Success);
    ExpectVsPost("0x00", "0x01", "0x00", "0x01", CompareResult::Success);

    // Double layer
    // ExpectVsPost("0x", "0x", "--", "--", CompareResult::IncorrectStorage, "0x03");
    // ExpectVsPost("0x", "0x00", "--", "--", CompareResult::IncorrectStorage, "0x03");
    // ExpectVsPost("0x00", "0x", "--", "--", CompareResult::IncorrectStorage, "0x03");
    ExpectVsPost("0x00", "0x00", "--", "--", CompareResult::IncorrectStorage, "0x03");
    // ExpectVsPost("0x01", "0x", "--", "--", CompareResult::IncorrectStorage, "0x03");
    ExpectVsPost("0x01", "0x00", "--", "--", CompareResult::IncorrectStorage, "0x03");
    // ExpectVsPost("0x", "0x01", "--", "--", CompareResult::IncorrectStorage, "0x03");
    ExpectVsPost("0x00", "0x01", "--", "--", CompareResult::IncorrectStorage, "0x03", 3);
    ExpectVsPost("0x01", "0x01", "--", "--", CompareResult::IncorrectStorage, "0x03", 3);

    // ExpectVsPost("0x", "0x01", "0x", "0x01", CompareResult::IncorrectStorage, "0x03");
    // ExpectVsPost("0x00", "0x01", "0x", "0x01", CompareResult::IncorrectStorage, "0x03");
    // ExpectVsPost("0x", "0x01", "0x00", "0x01", CompareResult::IncorrectStorage, "0x03");
    ExpectVsPost("0x00", "0x01", "0x00", "0x01", CompareResult::IncorrectStorage, "0x03");
}

BOOST_AUTO_TEST_CASE(clientconfigTest)
{
    string data = R"(
    {
        "name" : "Ethereum GO on TCP",
        "socketType" : "tcp",
        "socketAddress" : [
            "127.0.0.1:8545",
            "127.0.0.1:8546"
        ],
        "forks" : [
          "Frontier",
          "Homestead",
          "EIP150"
        ],
        "additionalForks" : [
          "FrontierToHomesteadAt5",
          "HomesteadToEIP150At5"
        ],
        "exceptions" : {
            "ExtraDataTooBig" : "extra-data too long",
            "InvalidDifficulty" : "invalid difficulty"
        }
    })";

    test::teststruct::ClientConfigFile cfg(ConvertJsoncppStringToData(data));

    // Check Fork Order
    BOOST_CHECK(cfg.forks().at(0).asString() == "Frontier");
    BOOST_CHECK(cfg.forks().at(1).asString() == "Homestead");
    BOOST_CHECK(cfg.forks().at(2).asString() == "EIP150");

    // Check address Order
    BOOST_CHECK(cfg.socketAdresses().at(0).asString() == "127.0.0.1:8545");
    BOOST_CHECK(cfg.socketAdresses().at(1).asString() == "127.0.0.1:8546");
}

BOOST_AUTO_TEST_SUITE_END()
