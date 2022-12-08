#pragma once
#include <retesteth/session/SessionInterface.h>


namespace test {
struct TxContext {
    TxContext(test::session::SessionInterface& _session, std::string const& _testName,
        test::teststruct::spTransaction const& _tr, test::teststruct::spBlockHeader const& _header,
        size_t _blIndex, size_t _trIndex)
      : session(_session), testName(_testName), tr(_tr), blockHeader(_header), blIndex(_blIndex), trIndex(_trIndex) {}
    test::session::SessionInterface& session;
    std::string const& testName;
    test::teststruct::spTransaction const& tr;
    test::teststruct::spBlockHeader const& blockHeader;
    size_t blIndex;
    size_t trIndex;
};

void performVMTrace(TxContext const& _context);

}
