#pragma once
#include <retesteth/session/SessionInterface.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <retesteth/testStructures/basetypes/FH32.h>


namespace test {
struct TxContext {
    TxContext(test::session::SessionInterface& _session, std::string const& _testName,
        test::teststruct::spTransaction const& _tr, test::teststruct::spBlockHeader const& _header,
        test::teststruct::FORK const& _fork, size_t _blIndex, size_t _trIndex)
      : session(_session), testName(_testName), tr(_tr), blockHeader(_header),
        network(_fork), blIndex(_blIndex), trIndex(_trIndex) {}
    test::session::SessionInterface& session;
    std::string const& testName;
    test::teststruct::spTransaction const& tr;
    test::teststruct::spBlockHeader const& blockHeader;
    test::teststruct::FORK const& network;
    size_t blIndex;
    size_t trIndex;
};

void performVMTrace(TxContext const& _context);
void performPostState(test::session::SessionInterface& _session, std::string const& _testName,
                      std::string const& _net, test::teststruct::FH32 const&);
void performPostState(TxContext const& _context);
void performPostStateBlockOnly(TxContext const& _context);
}
