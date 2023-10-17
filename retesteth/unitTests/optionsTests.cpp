#include <retesteth/Options.h>
#include <retesteth/helpers/TestOutputHelper.h>

using namespace std;
using namespace dev;
using namespace test;

BOOST_FIXTURE_TEST_SUITE(OptionsSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(options_initdefault)
{
    const char* argv[] = {"./retesteth", "--"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().all.initialized() == false);
    BOOST_CHECK(opt.get().blockLimit.initialized() == false);
    BOOST_CHECK(opt.get().blockLimit == 0);
    BOOST_CHECK(opt.get().checkhash == false);
    BOOST_CHECK(opt.get().clients.initialized() == false);
    BOOST_CHECK(std::vector<string>(opt.get().clients).empty() == true);
    BOOST_CHECK(opt.get().datadir.empty() == true);
    BOOST_CHECK(opt.get().enableClientsOutput == false);
    BOOST_CHECK(opt.get().exectimelog == false);
    BOOST_CHECK(opt.get().fillchain == false);
    BOOST_CHECK(opt.get().filltests == false);
    BOOST_CHECK(opt.get().forceupdate == false);
    BOOST_CHECK(opt.get().fullstate == false);
    BOOST_CHECK(opt.get().logVerbosity == 1);
    BOOST_CHECK(opt.get().lowcpu == false);
    BOOST_CHECK(opt.get().nodesoverride.size() == 0);
    BOOST_CHECK(opt.get().nologcolor == false);
    BOOST_CHECK(opt.get().poststate == false);
    BOOST_CHECK(opt.get().rCurrentTestSuite == "");
    BOOST_CHECK(opt.get().rpcLimit == 0);
    BOOST_CHECK(opt.get().showhash == false);
    BOOST_CHECK(opt.get().singleTestFile.empty() == true);
    BOOST_CHECK(opt.get().singleTestNet.empty() == true);
    BOOST_CHECK(opt.get().singleTestOutFile.empty() == true);
    BOOST_CHECK(opt.get().singletest.initialized() == false);
    BOOST_CHECK(opt.get().testpath.empty() == true);
    BOOST_CHECK(opt.get().threadCount == 1);
    BOOST_CHECK(opt.get().trData.index == -1);
    BOOST_CHECK(opt.get().trData.label.empty() == true);
    BOOST_CHECK(opt.get().trGasIndex == -1);
    BOOST_CHECK(opt.get().trValueIndex == -1);
    BOOST_CHECK(opt.get().travisOutThread == false);
    BOOST_CHECK(opt.get().vmtrace == false);
    BOOST_CHECK(opt.get().vmtrace_nomemory == false);
    BOOST_CHECK(opt.get().vmtrace_noreturndata == false);
    BOOST_CHECK(opt.get().vmtrace_nostack == false);
    BOOST_CHECK(opt.get().vmtraceraw == false);
}

BOOST_AUTO_TEST_CASE(options_missarg)
{
    try
    {
        const char* argv[] = {"./retesteth", "-t"};
        TestOptions opt(std::size(argv), argv);
        BOOST_ERROR("Expected Exception!");
    }
    catch (std::exception const& _ex)
    {
        BOOST_CHECK(string(_ex.what()).find("`-t` option is missing an argument") != string::npos);
    }
}

BOOST_AUTO_TEST_CASE(options_dwrongarg)
{
    try
    {
        const char* argv[] = {"./retesteth", "--", "-d", "0x23"};
        TestOptions opt(std::size(argv), argv);
        BOOST_ERROR("Expected Exception!");
    }
    catch (std::exception const& _ex)
    {
        BOOST_CHECK(string(_ex.what()).find("`-d` option has wrong argument") != string::npos);
    }
}

BOOST_AUTO_TEST_CASE(options_beforeseparator)
{
    try
    {
        const char* argv[] = {"./retesteth", "-d", "0x23"};
        TestOptions opt(std::size(argv), argv);
        BOOST_ERROR("Expected Exception!");
    }
    catch (std::exception const& _ex)
    {
        BOOST_CHECK(string(_ex.what()).find("option appears before the separator") != string::npos);
    }
}

BOOST_AUTO_TEST_CASE(options_afterseparator)
{
    try
    {
        const char* argv[] = {"./retesteth", "--", "-t", "somearg"};
        TestOptions opt(std::size(argv), argv);
        BOOST_ERROR("Expected Exception!");
    }
    catch (std::exception const& _ex)
    {
        BOOST_CHECK(string(_ex.what()).find("option appears after the separator") != string::npos);
    }
}


BOOST_AUTO_TEST_CASE(options_int)
{
    const char* argv[] = {"./retesteth", "-t", "GeneralStateTests", "--", "-g", "3"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().trGasIndex.initialized() == true);
    BOOST_CHECK(opt.get().trGasIndex == 3);
}

BOOST_AUTO_TEST_CASE(options_bool)
{
    const char* argv[] = {"./retesteth", "--", "--vmtrace"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().vmtrace.initialized() == true);
}

BOOST_AUTO_TEST_CASE(options_booloutpath)
{
    const char* argv[] = {"./retesteth", "--", "--vmtraceraw"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().vmtraceraw.initialized() == true);
    BOOST_CHECK(opt.get().vmtraceraw.outpath.empty() == true);
    {
        const char* argv[] = {"./retesteth", "--", "--vmtraceraw", "/somepath"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().vmtraceraw.initialized() == true);
        BOOST_CHECK(opt.get().vmtraceraw.outpath == "/somepath");
    }
}

BOOST_AUTO_TEST_CASE(options_string)
{
    const char* argv[] = {"./retesteth", "--", "--datadir", "/anything"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().datadir == "/anything");
    BOOST_CHECK(opt.get().datadir.initialized() == true);
}

BOOST_AUTO_TEST_CASE(options_fspath)
{
    const char* argv[] = {"./retesteth", "--", "--testpath", "/tmp"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().testpath == "/tmp");
    BOOST_CHECK(opt.get().testpath.initialized() == true);
}

BOOST_AUTO_TEST_CASE(options_fspath_wrong)
{
    try
    {
        const char* argv[] = {"./retesteth", "--", "--testpath", "/wrong"};
        TestOptions opt(std::size(argv), argv);
        BOOST_ERROR("Expected Exception!");
    }
    catch (std::exception const& _ex)
    {
        BOOST_CHECK(string(_ex.what()).find("`--testpath` could not locate file or path") != string::npos);
    }
}

BOOST_AUTO_TEST_CASE(options_vecstr)
{
    const char* argv[] = {"./retesteth", "--", "--clients", "geth"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(std::vector<string>(opt.get().clients).size() == 1);
    BOOST_CHECK(opt.get().clients.initialized() == true);
    BOOST_CHECK(std::vector<string>(opt.get().clients).at(0) == "geth");
    {
        const char* argv[] = {"./retesteth", "--", "--clients", "geth,besu"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(std::vector<string>(opt.get().clients).size() == 2);
        BOOST_CHECK(opt.get().clients.initialized() == true);
        BOOST_CHECK(std::vector<string>(opt.get().clients).at(0) == "geth");
        BOOST_CHECK(std::vector<string>(opt.get().clients).at(1) == "besu");
    }
}

BOOST_AUTO_TEST_CASE(options_vecaddr)
{
    const char* argv[] = {"./retesteth", "--", "--nodes", "192.168.1.1:8534"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().nodesoverride.size() == 1);
    BOOST_CHECK(opt.get().nodesoverride.initialized() == true);
    BOOST_CHECK(std::vector<IPADDRESS>(opt.get().nodesoverride).at(0) == IPADDRESS(DataObject("192.168.1.1:8534")));
    {
        const char* argv[] = {"./retesteth", "--", "--nodes", "192.168.1.1:8534,192.168.2.1:1534"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().nodesoverride.size() == 2);
        BOOST_CHECK(opt.get().nodesoverride.initialized() == true);
        BOOST_CHECK(std::vector<IPADDRESS>(opt.get().nodesoverride).at(0) == IPADDRESS(DataObject("192.168.1.1:8534")));
        BOOST_CHECK(std::vector<IPADDRESS>(opt.get().nodesoverride).at(1) == IPADDRESS(DataObject("192.168.2.1:1534")));
    }
}

BOOST_AUTO_TEST_CASE(options_dataint)
{
    const char* argv[] = {"./retesteth", "-t", "GeneralStateTests", "--", "-d", "4", "-g", "5"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().trData.index == 4);
    BOOST_CHECK(opt.get().trData.initialized() == true);
    BOOST_CHECK(opt.get().trData.label.empty() == true);
    {
        const char* argv[] = {"./retesteth", "-t", "GeneralStateTests", "--", "-d", "label", "-g", "5"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().trData.initialized() == true);
        BOOST_CHECK(opt.get().trData.label == ":label label");
    }
}

BOOST_AUTO_TEST_CASE(options_joption)
{
    const char* argv[] = {"./retesteth", "--", "-j", "4"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().threadCount == 4);
    BOOST_CHECK(opt.get().threadCount.initialized() == true);
    {
        const char* argv[] = {"./retesteth", "--", "-j4"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().threadCount.initialized() == true);
        BOOST_CHECK(opt.get().threadCount == 4);
    }
}

BOOST_AUTO_TEST_CASE(options_singletestA)
{
    const char* argv[] = {"./retesteth", "--", "--singletest", "filename"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().singletest.initialized() == true);
    BOOST_CHECK(opt.get().singletest.name == "filename");
    BOOST_CHECK(opt.get().singletest.subname.empty() == true);
}

BOOST_AUTO_TEST_CASE(options_singletestAsB)
{
    const char* argv[] = {"./retesteth", "--", "--singletest", "filename/testname"};
    TestOptions opt(std::size(argv), argv);
    BOOST_CHECK(opt.get().singletest.initialized() == true);
    BOOST_CHECK(opt.get().singletest.name == "filename");
    BOOST_CHECK(opt.get().singletest.subname == "testname");
}

BOOST_AUTO_TEST_CASE(options_singlenetMultiple)
{
    {
        const char* argv[] = {"./retesteth", "--", "--singlenet", ">=Berlin"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().singleTestNet.initialized() == false);
        BOOST_CHECK(opt.get().runOnlyNets.initialized() == true);
        BOOST_CHECK(opt.get().runOnlyNets == ">=Berlin");
    }
    {
        const char* argv[] = {"./retesteth", "--", "--singlenet", "<Berlin"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().singleTestNet.initialized() == false);
        BOOST_CHECK(opt.get().runOnlyNets.initialized() == true);
        BOOST_CHECK(opt.get().runOnlyNets == "<Berlin");
    }
    {
        const char* argv[] = {"./retesteth", "--", "--singlenet", "Berlin,London"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().singleTestNet.initialized() == false);
        BOOST_CHECK(opt.get().runOnlyNets.initialized() == true);
        BOOST_CHECK(opt.get().runOnlyNets == "Berlin,London");
    }
    {
        const char* argv[] = {"./retesteth", "--", "--singlenet", "Berlin"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().singleTestNet.initialized() == true);
        BOOST_CHECK(opt.get().singleTestNet == "Berlin");
        BOOST_CHECK(opt.get().runOnlyNets.initialized() == false);
        BOOST_CHECK(opt.get().runOnlyNets.empty());
    }
}

BOOST_AUTO_TEST_CASE(options_statediff_froktofork)
{
    {
        const char* argv[] = {"./retesteth", "--", "--statediff", "Berlin--Merge"};
        TestOptions opt(std::size(argv), argv);
        BOOST_CHECK(opt.get().statediff.initialized() == true);
        BOOST_CHECK(opt.get().statediff.isBlockSelected == false);
        BOOST_CHECK(opt.get().statediff.isTransSelected == false);
        BOOST_CHECK(opt.get().statediff.isForkSelected == true);
        BOOST_CHECK(opt.get().statediff.firstFork == "Berlin");
        BOOST_CHECK(opt.get().statediff.seconFork == "Merge");
    }
}

BOOST_AUTO_TEST_SUITE_END()
