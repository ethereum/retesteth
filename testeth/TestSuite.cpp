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
/** @file
 * Base functions for all test suites
 */

#include <boost/test/unit_test.hpp>
#include <json/value.h>
#include <json/reader.h>
#include <string>
#include <devcore/Log.h>
#include <devcore/CommonIO.h>
#include <devcore/SHA3.h>
#include <testeth/TestSuite.h>
#include <testeth/DataObject.h>
#include <testeth/TestOutputHelper.h>
#include <testeth/TestHelper.h>
#include <testeth/Options.h>
#include <testeth/RPCSession.h>

using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

//Helper functions for test proccessing
namespace {

void removeComments(test::DataObject& _obj)
{
    if (_obj.type() == test::DataType::Object)
	{
		list<string> removeList;
        for (auto& i: _obj.getSubObjectsUnsafe())
		{
            if (i.getKey().substr(0, 2) == "//")
			{
                removeList.push_back(i.getKey());
				continue;
			}
            removeComments(i);
		}
        for (auto const& i: removeList)
            _obj.removeKey(i);
	}
    else if (_obj.type() == test::DataType::Array)
	{
        for (auto& i: _obj.getSubObjectsUnsafe())
			removeComments(i);
    }
}

void addClientInfo(test::DataObject& _v, fs::path const& _testSource, h256 const& _testSourceHash)
{
    RPCSession& session = RPCSession::instance("/home/wins/.ethereum/geth.ipc");
    for (auto& o: _v.getSubObjectsUnsafe())
	{
        string comment;
        test::DataObject clientinfo;
		if (o.count("_info"))
		{
            test::DataObject const& existingInfo = o["_info"];
            if (existingInfo.count("comment"))
                comment = existingInfo.at("comment").asString();
		}

		clientinfo["comment"] = comment;
        clientinfo["filledwith"] = session.test_getClientInfo();
        clientinfo["retesteth"] = test::prepareVersionString();
		clientinfo["lllcversion"] = test::prepareLLLCVersionString();
		clientinfo["source"] = _testSource.string();
		clientinfo["sourceHash"] = toString(_testSourceHash);
		o["_info"] = clientinfo;
		o.setKeyPos("_info", 0);
    }
}

void checkFillerHash(fs::path const& _compiledTest, fs::path const& _sourceTest)
{
	string const s = asString(dev::contents(_compiledTest));
	BOOST_REQUIRE_MESSAGE(s.length() > 0, "Contents of " + _compiledTest.string() + " is empty.");
    test::DataObject v = test::convertJsonCPPtoData(test::readJson(s));
	h256 const fillerHash = sha3(dev::contents(_sourceTest));

    for (auto const& i: v.getSubObjects())
	{
        // use eth object _info section class here !!!!!
        BOOST_REQUIRE_MESSAGE(i.type() == test::DataType::Object, i.getKey() + " should contain an object under a test name.");
        BOOST_REQUIRE_MESSAGE(i.count("_info") > 0, "_info section not set! " + _compiledTest.string());
        test::DataObject const& info = i.at("_info");
        BOOST_REQUIRE_MESSAGE(info.count("sourceHash") > 0, "sourceHash not found in " + _compiledTest.string() + " in " + i.getKey());
        h256 const sourceHash = h256(info.at("sourceHash").asString());
        BOOST_CHECK_MESSAGE(sourceHash == fillerHash, "Test " + _compiledTest.string() + " in " + i.getKey() + " is outdated. Filler hash is different!");
    }
}

}

namespace test
{

string const c_fillerPostf = "Filler";
string const c_copierPostf = "Copier";

void TestSuite::runTestWithoutFiller(boost::filesystem::path const& _file) const
{
	// Allow to execute a custom test .json file on any test suite
	auto& testOutput = test::TestOutputHelper::get();
	testOutput.initTest(1);
	executeFile(_file);
	testOutput.finishTest();
}

void TestSuite::runAllTestsInFolder(string const& _testFolder) const
{
	// check that destination folder test files has according Filler file in src folder
	string const filter = test::Options::get().singleTestName.empty() ? string() : test::Options::get().singleTestName;
	vector<fs::path> const compiledFiles = test::getFiles(getFullPath(_testFolder), {".json", ".yml"} ,filter);
	for (auto const& file: compiledFiles)
	{
		fs::path const expectedFillerName = getFullPathFiller(_testFolder) / fs::path(file.stem().string() + c_fillerPostf + ".json");
		fs::path const expectedFillerName2 = getFullPathFiller(_testFolder) / fs::path(file.stem().string() + c_fillerPostf + ".yml");
		fs::path const expectedCopierName = getFullPathFiller(_testFolder) / fs::path(file.stem().string() + c_copierPostf + ".json");
		BOOST_REQUIRE_MESSAGE(fs::exists(expectedFillerName) || fs::exists(expectedFillerName2) || fs::exists(expectedCopierName), "Compiled test folder contains test without Filler: " + file.filename().string());
		BOOST_REQUIRE_MESSAGE(!(fs::exists(expectedFillerName) && fs::exists(expectedFillerName2) && fs::exists(expectedCopierName)), "Src test could either be Filler.json, Filler.yml or Copier.json: " + file.filename().string());

		// Check that filled tests created from actual fillers
		if (Options::get().filltests == false)
		{
			if (fs::exists(expectedFillerName))
				checkFillerHash(file, expectedFillerName);
			if (fs::exists(expectedFillerName2))
				checkFillerHash(file, expectedFillerName2);
			if (fs::exists(expectedCopierName))
				checkFillerHash(file, expectedCopierName);
		}
	}

	// run all tests
	vector<fs::path> const files = test::getFiles(getFullPathFiller(_testFolder), {".json", ".yml"}, filter.empty() ? filter : filter + "Filler");

	auto& testOutput = test::TestOutputHelper::get();
	testOutput.initTest(files.size());
	for (auto const& file: files)
	{
		testOutput.showProgress();
		testOutput.setCurrentTestFile(file);
		executeTest(_testFolder, file);
	}
	testOutput.finishTest();
}

fs::path TestSuite::getFullPathFiller(string const& _testFolder) const
{
	return test::getTestPath() / "src" / suiteFillerFolder() / _testFolder;
}

fs::path TestSuite::getFullPath(string const& _testFolder) const
{
	return test::getTestPath() / suiteFolder() / _testFolder;
}

void TestSuite::executeTest(string const& _testFolder, fs::path const& _testFileName) const
{
	fs::path const boostRelativeTestPath = fs::relative(_testFileName, getTestPath());
	string testname = _testFileName.stem().string();
	bool isCopySource = false;
	if (testname.rfind(c_fillerPostf) != string::npos)
		testname = testname.substr(0, testname.rfind("Filler"));
	else if (testname.rfind(c_copierPostf) != string::npos)
	{
		testname = testname.substr(0, testname.rfind(c_copierPostf));
		isCopySource = true;
	}
	else
		BOOST_REQUIRE_MESSAGE(false, "Incorrect file suffix in the filler folder! " + _testFileName.string());

	// Filename of the test that would be generated
	fs::path const boostTestPath = getFullPath(_testFolder) / fs::path(testname + ".json");

    TestSuiteOptions opt;
	if (Options::get().filltests)
	{
		if (isCopySource)
		{
			clog << "Copying " << _testFileName.string() << "\n";
			clog << " TO " << boostTestPath.string() << "\n";
			assert(_testFileName.string() != boostTestPath.string());
			TestOutputHelper::get().showProgress();
			test::copyFile(_testFileName, boostTestPath);
			BOOST_REQUIRE_MESSAGE(boost::filesystem::exists(boostTestPath.string()), "Error when copying the test file!");

			// Update _info and build information of the copied test
			/*Json::Value v;
			string const s = asString(dev::contents(boostTestPath));
			json_spirit::read_string(s, v);
			addClientInfo(v, boostRelativeTestPath, sha3(dev::contents(_testFileName)));
			writeFile(boostTestPath, asBytes(json_spirit::write_string(v, true)));*/
		}
		else
		{
			if (!Options::get().singleTest)
                std::cout << "Populating tests..." << std::endl;

            DataObject v;
			bytes const byteContents = dev::contents(_testFileName);
			string const s = asString(byteContents);
			BOOST_REQUIRE_MESSAGE(s.length() > 0, "Contents of " + _testFileName.string() + " is empty.");

			if (_testFileName.extension() == ".json")
                v = test::convertJsonCPPtoData(readJson(s));
            //else if (_testFileName.extension() == ".yml")
            //	v = test::parseYamlToJson(s);
			else
                BOOST_ERROR("Unknown test format!" + TestOutputHelper::get().testFile().string());

			removeComments(v);
            opt.doFilling = true;
            DataObject output = doTests(v, opt);
			if (!opt.wasErrors)
			{
				addClientInfo(output, boostRelativeTestPath, sha3(byteContents));
				writeFile(boostTestPath, asBytes(output.asJson()));
			}
		}
	}

    if (!opt.wasErrors)
    {
        // Test is generated. Now run it and check that there should be no errors
        if ((Options::get().singleTest && Options::get().singleTestName == testname) || !Options::get().singleTest)
            cnote << "TEST " << testname + ":";

        executeFile(boostTestPath);
    }
}

void TestSuite::executeFile(boost::filesystem::path const& _file) const
{
    TestSuiteOptions opt;
	string const s = asString(dev::contents(_file));
	BOOST_REQUIRE_MESSAGE(s.length() > 0, "Contents of " << _file.string() << " is empty. Have you cloned the 'tests' repo branch develop and set ETHEREUM_TEST_PATH to its path?");
    doTests(test::convertJsonCPPtoData(readJson(s)), opt);
}

}

