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
/** @file dataObjectTests.cpp
 * Unit tests for TestHelper functions.
 */

#include <dataObject/DataObject.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace test;
using namespace dataobject;

BOOST_FIXTURE_TEST_SUITE(DataObjectTestSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before1_of3)
{
    string const key = "aab0";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 0);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before2_of3)
{
    string const key = "abb0";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 1);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before3_of3)
{
    string const key = "acb4";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 2);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_after3_of3)
{
    string const key = "adb4";
    DataObject data(DataType::Object);
    data["aab1"] = "1";
    data["abb2"] = "2";
    data["adb3"] = "3";
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 3);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_empty)
{
    string const key = "adb4";
    DataObject data(DataType::Object);
    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 0);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before1_of8)
{
    string const key = "aa0";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 0);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before2_of8)
{
    string const key = "aa1b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 1);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before3_of8)
{
    string const key = "aa2b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 2);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before4_of8)
{
    string const key = "aa3b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 3);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before5_of8)
{
    string const key = "aa4b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 4);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before6_of8)
{
    string const key = "aa5b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 5);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before7_of8)
{
    string const key = "aa6b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 6);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_before8_of8)
{
    string const key = "aa7b";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 7);
}

BOOST_AUTO_TEST_CASE(dataobject_findOrderedKeyPosition_after8_of8)
{
    string const key = "aa9";
    DataObject data(DataType::Object);
    data["aa1"] = "1";
    data["aa2"] = "2";
    data["aa3"] = "3";
    data["aa4"] = "4";
    data["aa5"] = "5";
    data["aa6"] = "6";
    data["aa7"] = "7";
    data["aa8"] = "8";

    size_t pos = dataobject::findOrderedKeyPosition(key, data.getSubObjects());
    BOOST_CHECK(pos == 8);
}

BOOST_AUTO_TEST_CASE(dataobject_jsonOrder)
{
    DataObject data(DataType::Object);
    data.setAutosort(true);
    data["aa1"] = "1";
    data["aa31"] = "3";
    data["aa3"] = "3";
    data["aa7"] = "7";
    data["aa5"] = "5";
    data["aa2"] = "2";
    data["aa70"] = "7";
    data["aa8"] = "8";
    BOOST_CHECK(data.asJson(0, false) ==
                "{\"aa1\":\"1\",\"aa2\":\"2\",\"aa3\":\"3\",\"aa31\":\"3\",\"aa5\":\"5\",\"aa7\":"
                "\"7\",\"aa8\":\"8\",\"aa70\":\"7\"}");
}

BOOST_AUTO_TEST_SUITE_END()
