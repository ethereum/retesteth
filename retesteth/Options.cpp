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
 * Class for handling testeth custom options
 */

#include <iostream>
#include <iomanip>

#include <dataObject/ConvertFile.h>
#include <libdevcore/Log.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace test;
namespace fs = boost::filesystem;
Options::DynamicOptions Options::m_dynamicOptions;
void displayTestSuites();

void printHelp()
{
    cout << "Usage: \n";
    cout << std::left;
    cout << "\nSetting test suite\n";
    cout << setw(30) << "-t <TestSuite>" << setw(0) << "Execute test operations\n";
    cout << setw(30) << "-t <TestSuite>/<TestCase>" << setw(0) << "\n";
    cout << "\nAll options below must be followed by `--`\n";
    cout << "\nRetesteth options\n";
    cout << setw(40) << "-j <ThreadNumber>" << setw(0) << "Run test execution using threads\n";
    cout << setw(40) << "--clients `<client1, client2>`" << setw(0)
         << "Use following configurations from the testpath/Retesteth\n";
    cout << setw(40) << "--help" << setw(25) << "Display list of command arguments\n";
    cout << setw(40) << "--version" << setw(25) << "Display build information\n";
    cout << setw(40) << "--list" << setw(25) << "Display available test suites\n";

    cout << "\nSetting test suite and test\n";
    cout << setw(40) << "--testpath <PathToTheTestRepo>" << setw(25) << "Set path to the test repo\n";
    cout << setw(40) << "--testfile <TestFile>" << setw(0) << "Run tests from a file. Requires -t <TestSuite>\n";
    cout << setw(40) << "--singletest <TestName>/<Subtest>" << setw(0) << "Run on a single test (Testname is filename without Filler.json, Subtest is a test name inside the file)\n";

    cout << "\nDebugging\n";
    cout << setw(30) << "-d <index>" << setw(25) << "Set the transaction data array index when running GeneralStateTests\n";
    cout << setw(30) << "-g <index>" << setw(25) << "Set the transaction gas array index when running GeneralStateTests\n";
    cout << setw(30) << "-v <index>" << setw(25) << "Set the transaction value array index when running GeneralStateTests\n";
    cout << setw(30) << "--vmtrace" << setw(25) << "Trace transaction execution\n";
    cout << setw(30) << "--verbosity <level>" << setw(25) << "Set logs verbosity. 0 - silent, 1 - only errors, 2 - informative, >2 - detailed\n";
    cout << setw(30) << "--exectimelog" << setw(25) << "Output execution time for each test suite\n";
    cout << setw(30) << "--statediff" << setw(25) << "Trace state difference for state tests\n";
    cout << setw(30) << "--stderr" << setw(25) << "Redirect ipc client stderr to stdout\n";
    cout << setw(30) << "--travisout" << setw(25) << "Output `.` to stdout\n";

    cout << "\nAdditional Tests\n";
    cout << setw(30) << "--all" << setw(25) << "Enable all tests\n";

    cout << "\nTest Generation\n";
    cout << setw(30) << "--filltests" << setw(0) << "Run test fillers\n";
    cout << setw(30) << "--fillchain" << setw(25) << "When filling the state tests, fill tests as blockchain instead\n";
    cout << setw(30) << "--showhash" << setw(25) << "Show filler hash debug information\n";
    cout << setw(30) << "--fullstate" << setw(25) << "Do not compress large states to hash\n";
    //	cout << setw(30) << "--randomcode <MaxOpcodeNum>" << setw(25) << "Generate smart random EVM
    //code\n"; 	cout << setw(30) << "--createRandomTest" << setw(25) << "Create random test and
    //output it to the console\n"; 	cout << setw(30) << "--createRandomTest <PathToOptions.json>" <<
    //setw(25) << "Use following options file for random code generation\n"; 	cout << setw(30) <<
    //"--seed <uint>" << setw(25) << "Define a seed for random test\n"; 	cout << setw(30) <<
    //"--options <PathTo.json>" << setw(25) << "Use following options file for random code
    //generation\n";  cout << setw(30) << "--fulloutput" << setw(25) << "Disable address compression
    // in the output field\n";
}

void printVersion()
{
    cout << prepareVersionString() << "\n";
}

Options::Options(int argc, const char** argv)
{
	trDataIndex = -1;
	trGasIndex = -1;
	trValueIndex = -1;
	bool seenSeparator = false; // true if "--" has been seen.
	for (auto i = 0; i < argc; ++i)
	{
		auto arg = std::string{argv[i]};
		auto throwIfNoArgumentFollows = [&i, &argc, &arg]()
		{
			if (i + 1 >= argc)
				BOOST_THROW_EXCEPTION(InvalidOption(arg + " option is missing an argument."));
		};
		auto throwIfAfterSeparator = [&seenSeparator, &arg]()
		{
			if (seenSeparator)
				BOOST_THROW_EXCEPTION(InvalidOption(arg + " option appears after the separator --."));
		};
		if (arg == "--")
		{
			if (seenSeparator)
				BOOST_THROW_EXCEPTION(InvalidOption("The separator -- appears more than once in the command line."));
			seenSeparator = true;
			continue;
		}
		if (arg == "--help")
		{
			printHelp();
			exit(0);
		}
		else if (arg == "--version")
		{
			printVersion();
			exit(0);
		}
		else if (arg.substr(0,2) == "-j")
		{
			if (arg.length() != 2)
			{
				string threadDigits = arg.substr(2, arg.length());
				threadCount = max(1, atoi(threadDigits.c_str()));
			}
			else
			{
				throwIfNoArgumentFollows();
				string nextArg = argv[++i];
				if (nextArg.substr(0,1) != "-")
					threadCount = max(1, atoi(nextArg.c_str()));
			}
		}
		else if (arg == "--stderr")
		{
			enableClientsOutput = true;
		}
        else if (arg == "--travisout")
        {
            travisOutThread = true;
        }
        else if (arg == "--vm" || arg == "--evmc")
		{
			// Skip VM options because they are handled by vmProgramOptions().
			throwIfNoArgumentFollows();
			++i;
		}
		else if (arg == "--vmtrace")
        {
            vmtrace = true;
        }
        else if (arg == "--jsontrace")
		{
			throwIfNoArgumentFollows();
			jsontrace = true;
			auto arg = std::string{argv[++i]};
			//Json::Value value;
			//Json::Reader().parse(arg, value);
			//jsontraceOptions = debugOptions(value);
		}
		else if (arg == "--filltests")
			filltests = true;
		else if (arg == "--fillchain")
        {
            fillchain = true;
            filltests = true;
        }
        else if (arg == "--showhash")
            showhash = true;
		else if (arg == "--stats")
		{
			throwIfNoArgumentFollows();
			stats = true;
			statsOutFile = argv[++i];
		}
		else if (arg == "--exectimelog")
			exectimelog = true;
		else if (arg == "--all")
			all = true;
		else if (arg == "--singletest")
		{
			throwIfNoArgumentFollows();
			singleTest = true;
            singleTestName = std::string{argv[++i]};
            size_t pos = singleTestName.find_last_of('/');
            if (pos != string::npos)
            {
                singleSubTestName = singleTestName.substr(pos + 1);
                singleTestName = singleTestName.substr(0, pos);
            }
        }
        else if (arg == "--testfile")
        {
            throwIfNoArgumentFollows();
            singleTestFile = std::string{argv[++i]};
            if (!boost::filesystem::exists(singleTestFile.get()))
            {
                ETH_STDERROR_MESSAGE("Could not locate custom test file: '" + singleTestFile.get() + "'");
                exit(1);
            }
        }
        else if (arg == "--singlenet")
        {
            throwIfNoArgumentFollows();
            singleTestNet = std::string{argv[++i]};
        }
        else if (arg == "--fullstate")
            fullstate = true;
        else if (arg == "--poststate")
            poststate = true;
        else if (arg == "--verbosity")
		{
			throwIfNoArgumentFollows();
			static std::ostringstream strCout; //static string to redirect logs to
            logVerbosity = atoi(argv[++i]);
            if (logVerbosity == 0)
            {
                // disable all output
                std::cout.rdbuf(strCout.rdbuf());
                std::cerr.rdbuf(strCout.rdbuf());
                break;
            }
            if (logVerbosity > (size_t)g_logVerbosity)
                g_logVerbosity = logVerbosity;
		}
		else if (arg == "--options")
		{
			throwIfNoArgumentFollows();
			boost::filesystem::path file(std::string{argv[++i]});
			if (boost::filesystem::exists(file))
				randomCodeOptionsPath = file;
			else
			{
                ETH_STDERROR_MESSAGE(
                    "Options file not found! Default options at: "
                    "tests/src/randomCodeOptions.json\n");
                exit(0);
			}
		}
		else if (arg == "-t")
		{
			throwIfAfterSeparator();
			throwIfNoArgumentFollows();
			rCurrentTestSuite = std::string{argv[++i]};
		}
		else if (arg == "--nonetwork")
			nonetwork = true;
		else if (arg == "-d")
		{
			throwIfNoArgumentFollows();
			trDataIndex = atoi(argv[++i]);
		}
		else if (arg == "-g")
		{
			throwIfNoArgumentFollows();
			trGasIndex = atoi(argv[++i]);
		}
		else if (arg == "-v")
		{
			throwIfNoArgumentFollows();
			trValueIndex = atoi(argv[++i]);
		}
		else if (arg == "--testpath")
		{
			throwIfNoArgumentFollows();
            ETH_FAIL_REQUIRE_MESSAGE(testpath.empty(),
                "testpath is already set! Make sure that testpath is provided as a first option.");
            testpath = std::string{argv[++i]};
        }
		else if (arg == "--statediff")
			statediff = true;
		else if (arg == "--randomcode")
		{
			throwIfNoArgumentFollows();
			int maxCodes = atoi(argv[++i]);
			if (maxCodes > 1000 || maxCodes <= 0)
			{
				cerr << "Argument for the option is invalid! (use range: 1...1000)\n";
				exit(1);
			}
			//test::RandomCodeOptions options;
			//cout << test::RandomCode::get().generate(maxCodes, options) << "\n";
			exit(0);
		}
		else if (arg == "--createRandomTest")
		{
			createRandomTest = true;
			if (i + 1 < argc) // two params
			{
				auto options = std::string{argv[++i]};
				if (options[0] == '-') // not param, another option
					i--;
				else
				{
					boost::filesystem::path file(options);
					if (boost::filesystem::exists(file))
						randomCodeOptionsPath = file;
					else
						BOOST_THROW_EXCEPTION(InvalidOption("Options file not found! Default options at: tests/src/randomCodeOptions.json\n"));
				}
			}
		}
		else if (arg == "--seed")
		{
			throwIfNoArgumentFollows();
			/*u256 input = toInt(argv[++i]);
			if (input > std::numeric_limits<uint64_t>::max())
				BOOST_WARN("Seed is > u64. Using u64_max instead.");
			randomTestSeed = static_cast<uint64_t>(min<u256>(std::numeric_limits<uint64_t>::max(), input));*/
		}
        else if (arg == "--clients")
        {
            throwIfNoArgumentFollows();
            vector<string> clientNames;
            string nnn = std::string{argv[++i]};
            boost::split(clientNames, nnn, boost::is_any_of(", "));
            for (auto& it : clientNames)
            {
                boost::algorithm::trim(it);
                if (!it.empty())
                    clients.push_back(it);
            }
        }
        else if (arg == "--list")
        {
            displayTestSuites();
            exit(0);
        }
        else if (seenSeparator)
		{
			cerr << "Unknown option: " + arg << "\n";
			exit(1);
		}
	}

	//check restrickted options
	if (createRandomTest)
	{
		if (trValueIndex >= 0 || trGasIndex >= 0 || trDataIndex >= 0 || nonetwork || singleTest
			|| all || stats || filltests || fillchain)
		{
			cerr << "--createRandomTest cannot be used with any of the options: " <<
					"trValueIndex, trGasIndex, trDataIndex, nonetwork, singleTest, all, " <<
					"stats, filltests, fillchain \n";
			exit(1);
		}
	}
	else
	{
		if (randomTestSeed.is_initialized())
			BOOST_THROW_EXCEPTION(InvalidOption("--seed <uint> could be used only with --createRandomTest \n"));
	}

	//Default option
    if (logVerbosity == 1)
        g_logVerbosity = -1;  // disable cnote but leave cerr and cout
}

Options const& Options::get(int argc, const char** argv)
{
	static Options instance(argc, argv);
	return instance;
}

void displayTestSuites()
{
    cout << "List of available test suites: \n";
    cout << std::left;
    cout << setw(40) << "-t GeneralStateTests" << setw(0) << "Basic state transition tests\n";
    cout << setw(40) << "-t BCGeneralStateTests" << setw(0) << "Basic state transition tests in blockchain form\n";
    cout << setw(40) << "-t BlockchainTests" << setw(0) << "All Blockchain tests\n";
    cout << setw(40) << "-t BlockchainTests/ValidBlocks" << setw(0) << "Subset of correct blocks\n";
    cout << setw(40) << "-t BlockchainTests/InvalidBlocks" << setw(0) << "Subset of malicious blocks\n";
    cout << setw(40) << "-t BlockchainTests/TransitionTests" << setw(0) << "Subset of fork transition tests\n";
    cout << "(Use --filltests to generate the tests, --fillchain to generate BCGeneralStateTests)\n";

    cout << "\nLegacy test suites (Frontier .. ConstantinopleFix):\n";
    cout << setw(55) << "-t LegacyTests" << setw(0) << "All Legacy tests\n";
    cout << setw(55) << "-t LegacyTests/Constantinople" << setw(0) << "Subset of Frontier .. Constantinople tests\n";
    cout << setw(55) << "-t LegacyTests/Constantinople/GeneralStateTests" << setw(0) << "Old state tests\n";
    cout << setw(55) << "-t LegacyTests/Constantinople/BCGeneralStateTests" << setw(0) << "Old state tests in blockchain form\n";
    cout << setw(55) << "-t LegacyTests/Constantinople/BlockchainTests" << setw(0) << "Old blockchain tests\n";

    cout << "\nRetesteth unit tests:\n";
    cout << setw(30) << "-t DataObjectTestSuite" << setw(0) << "Unit tests for json parsing\n";
    cout << setw(30) << "-t EthObjectsSuite" << setw(0) << "Unit tests for test data objects\n";
    cout << setw(30) << "-t LLLCSuite" << setw(0) << "Unit tests for external solidity compiler\n";
    cout << setw(30) << "-t OptionsSuite" << setw(0) << "Unit tests for this cmd menu\n";
    cout << setw(30) << "-t TestHelperSuite" << setw(0) << "Unit tests for retesteth logic\n";
    cout << "\n";
}

