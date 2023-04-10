#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/basetypes/BYTES.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <retesteth/testStructures/types/StateTests/Filler/InfoIncomplete.h>

namespace test::teststruct
{

struct EOFTestVectorInFiller
{
    EOFTestVectorInFiller(dataobject::spDataObject&);
    std::string const& getExpectException(FORK const& _net) const;
    BYTES const& data() const { return m_data.getCContent(); }
private:
    spBYTES m_data;
    std::map<FORK, std::string> m_expectExceptions;
};

struct EOFTestInFiller : GCP_SPointerBase
{
    EOFTestInFiller(dataobject::spDataObject&);
    std::string const& testName() const { return m_name; }
    InfoIncomplete const& Info() const { return m_info; }
    bool hasInfo() const { return !m_info.isEmpty(); }
    std::vector<FORK> Forks() const { return m_forks; }
    std::vector<EOFTestVectorInFiller> Vectors() const { return m_vectors; }

private:
    EOFTestInFiller() {}
    std::string m_name;
    GCP_SPointer<InfoIncomplete> m_info;
    std::vector<FORK> m_forks;
    std::vector<EOFTestVectorInFiller> m_vectors;

};

struct EOFTestFiller
{
    EOFTestFiller(dataobject::spDataObject&);
    std::vector<EOFTestInFiller> const& tests() const { return m_tests; }

private:
    EOFTestFiller() {}
    std::vector<EOFTestInFiller> m_tests;
};

}  // namespace teststruct
