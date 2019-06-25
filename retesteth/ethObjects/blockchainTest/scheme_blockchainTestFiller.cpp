#include "scheme_blockchainTestFiller.h"
using namespace test;

scheme_blockchainTestFiller::fieldChecker::fieldChecker(DataObject const& _test)
{
    requireJsonFields(_test, "blockchainTest " + _test.getKey(),
        {{"_info", {{DataType::Object}, jsonField::Optional}},
            {"blocks", {{DataType::Array}, jsonField::Required}},
            {"expect", {{DataType::Array}, jsonField::Required}},
            {"genesisBlockHeader", {{DataType::Object}, jsonField::Required}},
            {"network", {{DataType::String}, jsonField::Required}},
            {"pre", {{DataType::Object}, jsonField::Required}},
            {"sealEngine", {{DataType::String}, jsonField::Optional}}});
}

scheme_blockchainTestFiller::scheme_blockchainTestFiller(DataObject const& _test)
  : scheme_blockchainTestBase(_test), m_checker(_test), m_expectSection(_test.atKey("expect"))
{}
