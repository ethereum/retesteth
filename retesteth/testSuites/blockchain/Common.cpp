#include "Common.h"
#include <retesteth/Options.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/helpers/TestHelper.h>
#include <libdevcore/CommonIO.h>
using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
namespace fs = boost::filesystem;

namespace  {
string getAndPrintRemoteState(test::session::SessionInterface& _session, FH32 const& _root)
{
    auto const remStateJson = getRemoteState(_session)->asDataObject()->asJson();
    ETH_DC_MESSAGE(DC::STATE,
        "\nState Dump:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + remStateJson);
    ETH_DC_MESSAGE(DC::STATE, "PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                  cDefault + "Hash: " + _root.asString());
    return remStateJson;
}
}

namespace test {

#define CHECK_SELECTOR(opt, context) \
        if (opt.isBlockSelected)     \
        {                            \
            if (opt.blockNumber != context.blIndex || opt.transactionNumber != context.trIndex) \
                return;              \
        }

void performVMTrace(TxContext const& _context)
{
    auto const& vmtrace = Options::get().vmtrace;
    if (vmtrace.initialized())
    {
        CHECK_SELECTOR(vmtrace, _context)
        string const testNameOut = _context.testName + "_" + _context.tr->hash().asString() + ".txt";
        VMtraceinfo info(_context.session, _context.tr->hash(), _context.blockHeader->stateRoot(), testNameOut);
        printVmTrace(info);
    }
}

void performPostState(test::session::SessionInterface& _session, string const& _testName, string const& _network, FH32 const& _root)
{
    auto const& poststate = Options::get().poststate;
    if (poststate.initialized() && !poststate.isBlockSelected)
    {
        auto const remStateJson = getAndPrintRemoteState(_session, _root);
        if (!poststate.outpath.empty())
        {
            string const testNameOut = _testName + "_" + _network + ".txt";
            fs::path const pathOut = fs::path(poststate.outpath) / testNameOut;
            ETH_DC_MESSAGEC(DC::STATE, "Export post state to " + pathOut.string(), LogColor::LIME);
            dev::writeFile(pathOut, dev::asBytes(remStateJson));
        }
    }
}

void performPostStateBlockOnly(TxContext const& _context)
{
    auto const& poststate = Options::get().poststate;
    if (poststate.initialized() && poststate.isBlockSelected)
    {
        if (poststate.blockNumber != _context.blIndex)
            return;

        auto const remStateJson = getAndPrintRemoteState(_context.session, _context.blockHeader->stateRoot());
        if (!poststate.outpath.empty())
        {
            string testNameOut = _context.testName + "_block" + test::fto_string(_context.blIndex);
            testNameOut += "_" + _context.network.asString() + ".txt";
            fs::path const pathOut = fs::path(poststate.outpath) / testNameOut;
            ETH_DC_MESSAGEC(DC::STATE, "Export post state to " + pathOut.string(), LogColor::LIME);
            dev::writeFile(pathOut, dev::asBytes(remStateJson));
        }
    }
}

void performPostState(TxContext const& _context)
{
    auto const& poststate = Options::get().poststate;
    if (poststate.initialized())
    {
        CHECK_SELECTOR(poststate, _context)
        auto const remStateJson = getAndPrintRemoteState(_context.session, _context.blockHeader->stateRoot());
        if (!poststate.outpath.empty())
        {
            string testNameOut = _context.testName + "_block" + test::fto_string(_context.blIndex)
                               + "_transaction" + test::fto_string(_context.trIndex);
            testNameOut += "_" + _context.network.asString() + ".txt";
            fs::path const pathOut = fs::path(poststate.outpath) / testNameOut;
            ETH_DC_MESSAGEC(DC::STATE, "Export post state to " + pathOut.string(), LogColor::LIME);
            dev::writeFile(pathOut, dev::asBytes(remStateJson));
        }
    }
}

void showWarningIfStatediffNotFound(spState const& _stateA, spState const& _stateB)
{
    auto const& statediff = Options::get().statediff;
    if (statediff.initialized() && statediff.isBlockSelected)
    {
        if (statediff.isTransSelected)
        {
            if (_stateA.isEmpty())
                ETH_WARNING("--statediff unable to get intermidiate state for block: `" + test::fto_string(statediff.firstBlock) +
                            "`, tx: `" + test::fto_string(statediff.firstTrnsx) + "`");
            else if (_stateB.isEmpty())
                ETH_WARNING("--statediff unable to get intermidiate state for block: `" + test::fto_string(statediff.seconBlock) +
                            "`, tx: `" + test::fto_string(statediff.seconTrnsx) + "`");
        }
        else
        {
            if (_stateA.isEmpty())
                ETH_WARNING("--statediff unable to get state for block: `" + test::fto_string(statediff.firstBlock));
            else if (_stateB.isEmpty())
                ETH_WARNING("--statediff unable to get state for block: `" + test::fto_string(statediff.seconBlock));
        }
    }
}

}
