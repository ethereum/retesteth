#include "Common.h"
#include <retesteth/Options.h>
#include <retesteth/testSuites/Common.h>
using namespace std;

namespace test {

void performVMTrace(TxContext const& _context)
{
    auto const& vmtrace = Options::get().vmtrace;
    if (vmtrace)
    {
        if (vmtrace.isBlockSelected)
        {
            if (vmtrace.blockNumber != _context.blIndex || vmtrace.transactionNumber != _context.trIndex)
                return;
        }
        string const testNameOut = _context.testName + "_" + _context.tr->hash().asString() + ".txt";
        VMtraceinfo info(_context.session, _context.tr->hash(), _context.blockHeader->stateRoot(), testNameOut);
        printVmTrace(info);
    }
}

}
