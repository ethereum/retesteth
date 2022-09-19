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
#include <retesteth/Options.h>
#include <testStructures/Common.h>
#include <retesteth/dataObject/SPointer.h>

using namespace std;
using namespace test;
namespace fs = boost::filesystem;
Options::DynamicOptions Options::m_dynamicOptions;

void displayTestSuites();
void printVersion()
{
    cout << prepareVersionString() << "\n";
}

bool Options::Option::match(string const& _arg) const
{
    if (m_argType == ARGS::ONEMERGED)
        return m_sOptionName == _arg.substr(0, m_sOptionName.size());
    else
    {
        for (auto const& el : explode(m_sOptionName, '|'))
            if (el == _arg)
                return true;
    }
    return false;
}
bool Options::Option::isAfterSeparatorOption() const
{
    return !m_allowBeforeSeparator;
}

void Options::Option::validate() const
{
    if (initialized() && m_validatorFunc)
        m_validatorFunc();
}

void Options::Option::setDefHelp(string&& _def, std::function<void()> _help)
{
    m_sOptionName = std::move(_def);
    m_printHelpFunc = _help;
}

void Options::Option::printHelp()
{
    if (m_printHelpFunc)
        m_printHelpFunc();
}

void Options::Option::initArgs(const char** _argv, size_t _argc, string const& _optionName, size_t _i)
{
    m_inited = m_inited || false;
    auto throwIfNoArgumentFollows = [&_argc, &_argv, this](size_t i) {
        auto throwException = [this](){
            BOOST_THROW_EXCEPTION(InvalidOption(m_sOptionName + " option is missing an argument."));
        };
        if (i + 1 >= _argc)
            throwException();
        string nextArg{_argv[i + 1]};
        if (nextArg.substr(0, 1) == "-")
            throwException();
    };

    switch(m_argType)
    {
    case ARGS::NONE:
    {
        m_inited = true;
        return;
    }
    case ARGS::NONE_OPTIONAL:
    {
        if (_i + 1 < _argc)
            initArg(string{_argv[++_i]});
        m_inited = true;
        break;
    }
    case ARGS::ONEMERGED:
    {
        size_t optNameLength = m_sOptionName.length();
        if (_optionName.length() != optNameLength)
        {
            string mergedArg = _optionName.substr(optNameLength, _optionName.length());
            initArg(mergedArg);
        }
        else
        {
            throwIfNoArgumentFollows(_i);
            initArg(string{_argv[++_i]});
        }
        m_inited = true;
        return;
    }
    case ARGS::ONE:
    {
        throwIfNoArgumentFollows(_i);
        initArg(string{_argv[++_i]});
        m_inited = true;
        return;
    }
    default:
        m_inited = false;
    }
}

void Options::Option::tryInit(const char** _argv, size_t _argc)
{
    bool seenSeparator = false;
    for (size_t i = 1; i < _argc; ++i)
    {
        auto arg = std::string{_argv[i]};
        if (arg == "--")
            seenSeparator = true;

        if (match(arg))
        {
            if (m_optionOverrides)
            {
                m_inited = true;
                break;
            }

            if (isAfterSeparatorOption() && !seenSeparator)
                BOOST_THROW_EXCEPTION(
                    InvalidOption(arg + " option appears before the separator `--`"));
            if (!isAfterSeparatorOption() && seenSeparator)
                BOOST_THROW_EXCEPTION(
                    InvalidOption(arg + " option appears after the separator `--`"));

            initArgs(_argv, _argc, arg, i);
            break;
        }
    }
}

#define ADD_OPTION(VAR, STR, STRHELP) \
 VAR.setDefHelp(STR, STRHELP); \
 m_options.push_back(&VAR);

#define ADD_OPTIONV(VAR, STR, STRHELP, VALIDATOR) \
 VAR.setValidator(VALIDATOR); \
 VAR.setDefHelp(STR, STRHELP); \
 m_options.push_back(&VAR);

#define ADD_OPTION_BOOST(VAR, STR, STRHELP) \
 VAR.setBeforeSeparator(); \
 VAR.setDefHelp(STR, STRHELP); \
 m_options.push_back(&VAR);

#define ADD_OPTION_OVERRIDE(VAR, STR, STRHELP) \
 VAR.setOverrideOption(); \
 VAR.setDefHelp(STR, STRHELP); \
 m_options.push_back(&VAR);


Options::Options(int argc, const char** argv)
{
    ADD_OPTION_OVERRIDE(help, "-h|--help", [](){
        printVersion();
        cout << std::left;
        cout << "\nUsage:\n";
        cout << "General options\n";
        cout << setw(30) << "-h --help" << setw(0) << "Display list of command arguments\n";
    });
    help.setValidator([this](){
        for(auto& option : m_options)
            option->printHelp();
        exit(0);
    });

    ADD_OPTION_OVERRIDE(version, "-v|--version", [](){
        cout << setw(30) << "-v --version " << setw(0) << "Display build information\n";
    });
    version.setValidator([](){
            printVersion();
            exit(0);
    });

    ADD_OPTION_OVERRIDE(listsuites, "--list", [](){
        cout << setw(30) << "--list" << setw(0) << "Display available test suites\n";
    });
    listsuites.setValidator([](){
            displayTestSuites();
            exit(0);
    });

    ADD_OPTION_BOOST(rCurrentTestSuite, "-t", [](){
        cout << "\nSetting test suite\n";
        cout << setw(30) << "-t <TestSuite>" << setw(0) << "Execute test operations\n";
        cout << setw(30) << "-t <TestSuite>/<TestCase>" << setw(0) << "See `--testfile` and `--testfolder` to run custom tests\n";
        cout << "\nAll options below must follow after `--`\n";
    });
    ADD_OPTIONV(threadCount, "-j", [](){
        cout << "\nRetesteth options\n";
        cout << setw(40) << "-j <ThreadNumber>" << setw(0) << "Run test execution using threads\n";
        },[this](){
            threadCount = max((size_t)1, (size_t)threadCount);
    });
    ADD_OPTION(clients, "--clients", [](){
        cout << setw(40) << "--clients `client1, client2`" << setw(0)
             << "Use following configurations from datadir path (default: ~/.retesteth)\n";
    });
    ADD_OPTION(datadir, "--datadir", [](){
        cout << setw(40) << "--datadir" << setw(0) << "Path to configs (default: ~/.retesteth)\n";
    });
    ADD_OPTION(nodesoverride, "--nodes", [](){
        cout << setw(40) << "--nodes" << setw(0) << "List of client tcp ports (\"addr:ip, addr:ip\")\n";
        cout << setw(40) << " " << setw(0) << "|-Overrides the config file \"socketAddress\" section \n";
    });
    ADD_OPTION(testpath, "--testpath", [](){
        cout << "\nSetting test suite and test\n";
        cout << setw(40) << "--testpath <PathToTheTestRepo>" << setw(25) << "Set path to the test repo\n";
    });
    ADD_OPTIONV(singleTestFile, "--testfile", [](){
        cout << setw(40) << "--testfile <TestFile>" << setw(0) << "Run tests from a file. Requires -t <TestSuite>\n";
        }, [this](){
        if (customTestFolder.initialized())
        {
            std::cerr << "Error: `--testfolder` initialized together with `--testfile`" << std::endl;
            exit(1);
        }
    });
    ADD_OPTION(singleTestOutFile, "--outfile", [](){
        cout << setw(40) << "--outfile <TestFile>" << setw(0) << "When using `--testfile` with `--filltests` output to this file\n";
    });
    ADD_OPTIONV(customTestFolder, "--testfolder", [](){
        cout << setw(40) << "--testfolder <SubFolder>" << setw(0) << "Run tests from a custom test folder located in a given suite. Requires -t <TestSuite>\n";
        }, [this](){
        if (singleTestFile.initialized())
        {
            std::cerr << "Error: `--testfolder` initialized together with `--testfile`" << std::endl;
            exit(1);
        }
    });
    ADD_OPTION(singletest, "--singletest", [](){
        cout << setw(40) << "--singletest <TestName>" << setw(0)
             << "Run on a single test. `Testname` is filename without Filler.json\n";
        cout << setw(40) << "--singletest <TestName>/<Subtest>" << setw(0) << "`Subtest` is a test name inside the file\n";
    });
    ADD_OPTION(singleTestNet, "--singlenet", [](){
        cout << setw(40) << "--singlenet <ForkName>" << setw(0) << "Run only specific fork configuration\n";
    });
    ADD_OPTION(trData, "-d", [](){
        cout << "\nDebugging\n";
        cout << setw(30) << "-d <index>" << setw(25) << "Set the transaction data array index when running GeneralStateTests\n";
        cout << setw(30) << "-d <label>" << setw(25) << "Set the transaction data array label (string) when running GeneralStateTests\n";
    });
    ADD_OPTION(trGasIndex, "-g", [](){
        cout << setw(30) << "-g <index>" << setw(25) << "Set the transaction gas array index when running GeneralStateTests\n";
    });
    ADD_OPTION(trValueIndex, "-v", [](){
        cout << setw(30) << "-v <index>" << setw(25) << "Set the transaction value array index when running GeneralStateTests\n";
    });
    ADD_OPTION(vmtrace, "--vmtrace", [](){
        cout << setw(30) << "--vmtrace" << setw(25) << "Trace transaction execution\n";
    });
    ADD_OPTIONV(vmtraceraw, "--vmtraceraw", [](){
        cout << setw(30) << "--vmtraceraw" << setw(25) << "Trace transaction execution raw format\n";
        cout << setw(30) << "--vmtraceraw <folder>" << setw(25) << "Trace transactions execution raw format to a given folder\n";
        }, [this](){
            vmtrace = true;
    });
    ADD_OPTIONV(vmtrace_nomemory, "--vmtrace.nomemory", [](){
        cout << setw(30) << "--vmtrace.nomemory" << setw(25) << "Disable memory in vmtrace/vmtraceraw\n";
        }, [this](){
            if (!(vmtrace.initialized() || vmtraceraw.initialized()))
                std::cerr << "Error: --vmtrace.nomemory requires --vmtrace or --vmtraceraw" << std::endl;
    });
    ADD_OPTIONV(vmtrace_nostack, "--vmtrace.nostack", [](){
            cout << setw(30) << "--vmtrace.nostack" << setw(25) << "Disable stack in vmtrace/vmtraceraw\n";
        }, [this](){
            if (!(vmtrace.initialized() || vmtraceraw.initialized()))
                std::cerr << "Error: --vmtrace.nostack requires --vmtrace or --vmtraceraw" << std::endl;
    });
    ADD_OPTIONV(vmtrace_noreturndata, "--vmtrace.noreturndata", [](){
            cout << setw(30) << "--vmtrace.noreturndata" << setw(25) << "Disable returndata in vmtrace/vmtraceraw\n";
        }, [this](){
            if (!(vmtrace.initialized() || vmtraceraw.initialized()))
            std::cerr << "Error: --vmtrace.noreturndata requires --vmtrace or --vmtraceraw" << std::endl;
    });
    ADD_OPTION(blockLimit, "--limitblocks", [](){
            cout << setw(30) << "--limitblocks" << setw(25) << "Limit the block exectuion in blockchain tests for debug\n";
    });
    ADD_OPTION(rpcLimit, "--limitrpc", [](){
        cout << setw(30) << "--limitrpc" << setw(25) << "Limit the rpc exectuion in tests for debug\n";
    });
    ADD_OPTIONV(logVerbosity, "--verbosity", [](){
        cout << setw(30) << "--verbosity <level>" << setw(25) << "Set logs verbosity. 0 - silent, 1 - only errors, 2 - informative, >2 - detailed\n";
        },[this](){
            // disable all output
            static std::ostringstream strCout;
            if (logVerbosity == 0)
            {
                std::cout.rdbuf(strCout.rdbuf());
                std::cerr.rdbuf(strCout.rdbuf());
            }
    });
    ADD_OPTION(nologcolor, "--nologcolor", [](){
        cout << setw(30) << "--nologcolor" << setw(25) << "Disable color codes in log output\n";
    });
    ADD_OPTION(exectimelog, "--exectimelog", [](){
        cout << setw(30) << "--exectimelog" << setw(25) << "Output execution time for each test suite\n";
    });
    ADD_OPTION(enableClientsOutput, "--stderr", [](){
        cout << setw(30) << "--stderr" << setw(25) << "Redirect ipc client stderr to stdout\n";
    });
    ADD_OPTION(travisOutThread, "--travisout", [](){
        cout << setw(30) << "--travisout" << setw(25) << "Output `.` to stdout\n";
    });
    ADD_OPTION(all, "--all", [](){
        cout << "\nAdditional Tests\n";
        cout << setw(30) << "--all" << setw(0) << "Enable all tests\n";
    });
    ADD_OPTION(lowcpu, "--lowcpu", [](){
        cout << setw(30) << "--lowcpu" << setw(25) << "Disable cpu intense tests\n";
    });
    ADD_OPTION(filltests, "--filltests", [](){
        cout << "\nTest Generation\n";
        cout << setw(30) << "--filltests" << setw(0) << "Run test fillers\n";
    });
    ADD_OPTIONV(fillchain, "--fillchain", [](){
        cout << setw(30) << "--fillchain" << setw(25) << "When filling the state tests, fill tests as blockchain instead\n";
        },[this](){
            if (!filltests.initialized())
            {
                std::cout << "WARNING: `--fillchain` option provided without `--filltests`, activating `--filltests` (did you mean `--filltests`?)\n";
                filltests = true;
            }
    });
    ADD_OPTION(showhash, "--showhash", [](){
        cout << setw(30) << "--showhash" << setw(25) << "Show filler hash debug information\n";
    });
    ADD_OPTION(checkhash, "--checkhash", [](){
        cout << setw(30) << "--checkhash" << setw(25) << "Check that tests are updated from fillers\n";
    });
    ADD_OPTIONV(poststate, "--poststate", [](){
        cout << setw(30) << "--poststate" << setw(25) << "Debug(6) show test postState hash or fullstate, when used with --filltests export `postState` in StateTests\n";
        cout << setw(30) << "--poststate <folder>" << setw(25) << "Same as above plus export test post states into a folder\n";
        }, [this](){
            if (!filltests.initialized())
                std::cerr << "Error: --poststate requires --filltests" << std::endl;
    });
    ADD_OPTION(fullstate, "--fullstate", [](){
        cout << setw(30) << "--fullstate" << setw(25) << "Do not compress large states to hash when debug\n";
    });
    ADD_OPTION(forceupdate, "--forceupdate", [](){
        cout << setw(30) << "--forceupdate" << setw(25) << "Update generated test (_info) even if there are no changes\n";
    });


    // check for unrecognized options

    for(auto& option : m_options)
        option->tryInit(argv, argc);
    for(auto& option : m_options)
        option->validate();



    exit(0);

    /*trDataIndex = -1;
    trGasIndex = -1;
    trValueIndex = -1;
    bool seenSeparator = false;  // true if "--" has been seen.
    for (auto i = 0; i < argc; ++i)
    {
        auto arg = std::string{argv[i]};
        auto throwIfNoArgumentFollows = [&i, &argc, &arg]() {
            if (i + 1 >= argc)
                BOOST_THROW_EXCEPTION(InvalidOption(arg + " option is missing an argument."));
        };
        auto throwIfAfterSeparator = [&seenSeparator, &arg]() {
            if (seenSeparator)
                BOOST_THROW_EXCEPTION(
                    InvalidOption(arg + " option appears after the separator `--`."));
        };
        auto throwIfBeforeSeparator = [&seenSeparator, &arg]() {
            if (!seenSeparator)
                BOOST_THROW_EXCEPTION(
                    InvalidOption(arg + " option appears before the separator `--`"));
        };
        auto getOptionalArg = [&i, &argc, &argv]() {
            if (i + 1 < argc)
            {
                string nextArg = argv[i + 1];
                if (nextArg.substr(0, 1) != "-")
                {
                    i++;
                    return nextArg;
                }
            }
            return string();
        };

        if (arg == "--")
        {
            if (seenSeparator)
                BOOST_THROW_EXCEPTION(
                    InvalidOption("The separator `--` appears more than once in the command line."));
            seenSeparator = true;
            continue;
        }
        else if (arg == "-t")
        {
            throwIfAfterSeparator();
            throwIfNoArgumentFollows();
            rCurrentTestSuite = std::string{argv[++i]};
            continue;
        }
        else if (i == 0)
        {
            // Skip './retesteth'
            continue;
        }

        if (arg == "--help" || arg == "-h")
        {
            printHelp();
            exit(0);
        }
        else if (arg == "--version" || (arg == "-v" && !seenSeparator))
        {
            printVersion();
            exit(0);
        }

        // Options below are not allowed before -- separator
        throwIfBeforeSeparator();
        if (arg.substr(0, 2) == "-j")
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
                if (nextArg.substr(0, 1) != "-")
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
        else if (arg == "--vmtraceraw")
        {
            vmtrace = true;
            vmtraceraw = true;
            vmtracerawfolder = getOptionalArg();
        }
        else if (arg == "--vmtrace.nomemory")
        {
            vmtrace_nomemory = true;
        }
        else if (arg == "--vmtrace.nostack")
        {
            vmtrace_nostack = true;
        }
        else if (arg == "--vmtrace.noreturndata")
        {
            vmtrace_noreturndata = true;
        }
        else if (arg == "--jsontrace")
        {
            throwIfNoArgumentFollows();
            jsontrace = true;
            auto arg = std::string{argv[++i]};
            // Json::Value value;
            // Json::Reader().parse(arg, value);
            // jsontraceOptions = debugOptions(value);
        }
        else if (arg == "--filltests")
            filltests = true;
        else if (arg == "--forceupdate")
            forceupdate = true;
        else if (arg == "--limitblocks")
        {
            throwIfNoArgumentFollows();
            blockLimit = atoi(argv[++i]);
        }
        else if (arg == "--limitrpc")
        {
            throwIfNoArgumentFollows();
            rpcLimit = atoi(argv[++i]);
        }
        else if (arg == "--fillchain")
        {
            fillchain = true;

            bool noFilltests = !filltests;
            if (noFilltests)
            {
                // Look ahead if this option ever provided
                for (auto i = 0; i < argc; ++i)
                {
                    auto arg = std::string{argv[i]};
                    if (arg == "--filltests")
                    {
                        noFilltests = false;
                        break;
                    }
                }
            }

            if (noFilltests)
                ETH_STDOUT_MESSAGEC("WARNING: `--fillchain` option provided without `--filltests`, activating `--filltests` (did you mean `--filltests`?)", cYellow);
            filltests = true;
        }
        else if (arg == "--showhash")
            showhash = true;
        else if (arg == "--checkhash")
            checkhash = true;
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
        else if (arg == "--lowcpu")
            lowcpu = true;
        else if (arg == "--singletest")
        {
            throwIfNoArgumentFollows();
            singleTest = true;
            singleTestName = std::string{argv[++i]};

            size_t pos = singleTestName.find("Filler");
            if (pos != string::npos)
            {
                singleTestName = singleTestName.substr(0, pos);
                ETH_STDOUT_MESSAGEC("WARNING: Correcting filter to: `" + singleTestName + "`", cYellow);
            }

            pos = singleTestName.find_last_of('/');
            if (pos != string::npos)
            {
                singleSubTestName = singleTestName.substr(pos + 1);
                singleTestName = singleTestName.substr(0, pos);
            }
        }
        else if (arg == "--testfile")
        {
            throwIfNoArgumentFollows();
            if (customTestFolder.is_initialized())
            {
                ETH_STDERROR_MESSAGE("--testfolder initialized together with --testfile");
                exit(1);
            }
            singleTestFile = std::string{argv[++i]};
            if (!boost::filesystem::exists(singleTestFile.get()))
            {
                ETH_STDERROR_MESSAGE(
                    "Could not locate custom test file: '" + singleTestFile.get() + "'");
                exit(1);
            }
        }
        else if (arg == "--testfolder")
        {
            throwIfNoArgumentFollows();
            if (singleTestFile.is_initialized())
            {
                ETH_STDERROR_MESSAGE("--testfolder initialized together with --testfile");
                exit(1);
            }
            customTestFolder = std::string{argv[++i]};
        }
        else if (arg == "--outfile")
        {
            throwIfNoArgumentFollows();
            singleTestOutFile = std::string{argv[++i]};
        }
        else if (arg == "--singlenet")
        {
            throwIfNoArgumentFollows();
            singleTestNet = std::string{argv[++i]};
        }
        else if (arg == "--fullstate")
            fullstate = true;
        else if (arg == "--poststate")
        {
            poststate = true;
            fullstate = true;
            poststatefolder = getOptionalArg();
        }
        else if (arg == "--verbosity")
        {
            throwIfNoArgumentFollows();
            static std::ostringstream strCout;  // static string to redirect logs to
            logVerbosity = atoi(argv[++i]);
            if (logVerbosity == 0)
            {
                // disable all output
                std::cout.rdbuf(strCout.rdbuf());
                std::cerr.rdbuf(strCout.rdbuf());
                break;
            }
        }
        else if (arg == "--nologcolor")
        {
            nologcolor = true;
        }
        else if (arg == "--datadir")
        {
            throwIfNoArgumentFollows();
            datadir = fs::path(std::string{argv[++i]});
        }
        else if (arg == "--nodes")
        {
            throwIfNoArgumentFollows();
            for (auto const& el : explode(std::string{argv[++i]}, ','))
                nodesoverride.push_back(IPADDRESS(el));
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
        else if (arg == "--nonetwork")
            nonetwork = true;
        else if (arg == "-d")
        {
            throwIfNoArgumentFollows();
            string const& argValue = argv[++i];
            DigitsType type = stringIntegerType(argValue);
            switch (type)
            {
            case DigitsType::Decimal:
                trDataIndex = atoi(argValue.c_str());
                break;
            case DigitsType::String:
                if (argValue.find(":label") == string::npos)
                    trDataLabel += ":label " + argValue;
                else
                    trDataLabel = argValue;
                break;
            default:
            {
                ETH_STDERROR_MESSAGE("Wrong argument format: " + argValue);
                exit(0);
            }
            }
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
            // test::RandomCodeOptions options;
            // cout << test::RandomCode::get().generate(maxCodes, options) << "\n";
            exit(0);
        }
        else if (arg == "--createRandomTest")
        {
            createRandomTest = true;
            if (i + 1 < argc)  // two params
            {
                auto options = std::string{argv[++i]};
                if (options[0] == '-')  // not param, another option
                    i--;
                else
                {
                    boost::filesystem::path file(options);
                    if (boost::filesystem::exists(file))
                        randomCodeOptionsPath = file;
                    else
                        BOOST_THROW_EXCEPTION(
                            InvalidOption("Options file not found! Default options at: "
                                          "tests/src/randomCodeOptions.json\n"));
                }
            }
        }
        else if (arg == "--seed")
        {
            throwIfNoArgumentFollows();
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

    if (poststate && logVerbosity < 6 && poststatefolder.empty())
    {
        ETH_STDOUT_MESSAGE("Warning: --poststate is defined, but state is printed with verbosity level 6, which is not set");
    }
    if (vmtraceraw && logVerbosity < 6 && vmtracerawfolder.empty())
    {
        ETH_STDOUT_MESSAGE("Warning: --vmtraceraw is defined, but trace is printed with verbosity level 6, which is not set");
    }

    // check restrickted options
    if (createRandomTest)
    {
        if (trValueIndex >= 0 || trGasIndex >= 0 || trDataIndex >= 0 || nonetwork || singleTest ||
            all || stats || filltests || fillchain)
        {
            cerr << "--createRandomTest cannot be used with any of the options: "
                 << "trValueIndex, trGasIndex, trDataIndex, nonetwork, singleTest, all, "
                 << "stats, filltests, fillchain \n";
            exit(1);
        }
    }
    else
    {
        if (randomTestSeed.is_initialized())
            BOOST_THROW_EXCEPTION(
                InvalidOption("--seed <uint> could be used only with --createRandomTest \n"));
    }

    if (threadCount == 1)
        dataobject::GCP_SPointer<int>::DISABLETHREADSAFE();
    */
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
    cout << setw(40) << "-t BlockchainTests/ValidBlocks/VMTests" << setw(0)
         << "VMTests converted\n";
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
    cout << setw(30) << "-t LLLCSuite" << setw(0) << "Unit tests for external lllc compiler\n";
    cout << setw(30) << "-t SOLCSuite" << setw(0) << "Unit tests for solidity support\n";
    cout << setw(30) << "-t OptionsSuite" << setw(0) << "Unit tests for this cmd menu\n";
    cout << setw(30) << "-t TestHelperSuite" << setw(0) << "Unit tests for retesteth logic\n";
    cout << "\n";
}

string Options::getGStateTransactionFilter() const
{
    string filter;
    filter += trData.index == -1 ? string() : " dInd: " + to_string(trData.index);
    filter += trData.label.empty() ? string() : " dLbl: " + trData.label;
    filter += trGasIndex == -1 ? string() : " gInd: " + to_string(trGasIndex);
    filter += trValueIndex == -1 ? string() : " vInd: " + to_string(trValueIndex);
    return filter;
}

bool Options::isLegacy()
{
    static bool isLegacy = (boost::unit_test::framework::current_test_case().full_name().find("LegacyTests") != string::npos);

    // Current test case is dynamic if we run all tests. need to see if we hit LegacyTests
    if (Options::get().rCurrentTestSuite.empty())
        isLegacy = (boost::unit_test::framework::current_test_case().full_name().find("LegacyTests") != string::npos);

    return isLegacy;
}


