#include "Common.h"
#include <retesteth/Options.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestHelper.h>
#include <libdevcore/CommonIO.h>
using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
namespace fs = boost::filesystem;

namespace  {
string getAndPrintRemoteState(test::session::SessionInterface& _session, FH32 const& _root)
{
    auto const remStateJson = getRemoteState(_session).asDataObject()->asJson();
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
            dev::writeFile(fs::path(poststate.outpath) / testNameOut, dev::asBytes(remStateJson));
        }
    }
}

void performPostStateBlockOnly(TxContext const& _context)
{
    auto const& poststate = Options::get().poststate;
    if (poststate.initialized())
    {
        if (!(poststate.isBlockSelected && poststate.blockNumber == _context.blIndex))
            return;

        auto const remStateJson = getAndPrintRemoteState(_context.session, _context.blockHeader->stateRoot());
        if (!poststate.outpath.empty())
        {
            string testNameOut = _context.testName + "_block" + test::fto_string(_context.blIndex);
            testNameOut += "_" + _context.network.asString() + ".txt";
            dev::writeFile(fs::path(poststate.outpath) / testNameOut, dev::asBytes(remStateJson));
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
            dev::writeFile(fs::path(poststate.outpath) / testNameOut, dev::asBytes(remStateJson));
        }
    }
}

}
