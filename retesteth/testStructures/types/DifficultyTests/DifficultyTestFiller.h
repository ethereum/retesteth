#pragma once
#include "../StateTests/Filler/InfoIncomplete.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <retesteth/testStructures/basetypes.h>
#include <map>

namespace test
{
namespace teststruct
{

struct DifficultyRange
{
    DifficultyRange(){};
    DifficultyRange(DataObject&);
    std::vector<spVALUE> const& vector() const { return m_vector; };

private:
    std::vector<spVALUE> m_vector;
};

struct DifficultyTestInFiller : GCP_SPointerBase
{
    DifficultyTestInFiller(spDataObject&);
    std::string const& testName() const { return m_name; }
    InfoIncomplete const& info() const { return m_info; }
    bool hasInfo() const { return !m_info.isEmpty(); }

    std::vector<FORK> const& networks() const { return m_networks; }
    DifficultyRange const& blocknumbers() const { return m_blockNumberD;}
    DifficultyRange const& timestumps() const { return m_timestampD;}
    DifficultyRange const& parentdiffs() const { return m_parentDiffD;}
    std::vector<size_t> const& uncles() const { return m_uncles; }

private:
    DifficultyTestInFiller() {}

    std::string m_name;
    GCP_SPointer<InfoIncomplete> m_info;
    std::vector<FORK> m_networks;

    DifficultyRange m_blockNumberD;
    DifficultyRange m_timestampD;
    DifficultyRange m_parentDiffD;
    std::vector<size_t> m_uncles;
};

struct DifficultyTestFiller
{
    DifficultyTestFiller(spDataObject&);
    std::vector<DifficultyTestInFiller> const& tests() const { return m_tests; }

private:
    DifficultyTestFiller() {}
    std::vector<DifficultyTestInFiller> m_tests;
};


}  // namespace teststruct
}  // namespace test
