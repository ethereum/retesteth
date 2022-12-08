#include "Common.h"
#include <retesteth/Options.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestHelper.h>
#include <libdevcore/CommonIO.h>
using namespace std;
using namespace dev;
using namespace test::debug;
namespace fs = boost::filesystem;

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

void performPostState(TxContext const& _context)
{
    auto const& poststate = Options::get().poststate;
    if (poststate.initialized())
    {
        CHECK_SELECTOR(poststate, _context)
        auto const remStateJson = getRemoteState(_context.session).asDataObject()->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nRunning test State Dump:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + remStateJson);

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
