#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <retesteth/testStructures/types/Ethereum/Transactions/Transaction.h>
#include <map>

namespace test::teststruct
{

struct EOFTestVectorInFilled
{
    EOFTestVectorInFilled(dataobject::DataObject const&);
    std::map<FORK, std::string> getResultForks() const { return m_expectExceptions; }
    std::string const& getExpectException(FORK const& _net) const;
    BYTES const& data() const { return m_data.getCContent(); }
private:
    spBYTES m_data;
    std::map<FORK, std::string> m_expectExceptions;
};


struct EOFTestInFilled : GCP_SPointerBase
{
    EOFTestInFilled(spDataObject&);
    std::string const& testName() const { return m_name; }
    std::map<std::string, EOFTestVectorInFilled> const& getVectors() const { return m_vectors; }

private:
    EOFTestInFilled() {}
    std::map<std::string, EOFTestVectorInFilled> m_vectors;
    std::string m_name;
};

struct EOFTest
{
    EOFTest(spDataObject&);
    std::vector<EOFTestInFilled> const& tests() const { return m_tests; }

private:
    EOFTest() {}
    std::vector<EOFTestInFilled> m_tests;
};


}  // namespace teststruct
