#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/basetypes.h>
#include <map>

namespace test
{
namespace teststruct
{
struct DifficultyTestVector
{
    DifficultyTestVector(spDataObject const&);
    spVALUE currentBlockNumber;
    spVALUE currentDifficulty;
    spVALUE currentTimestamp;
    spVALUE parentDifficulty;
    spVALUE parentTimestamp;
    bool parentUncles;
    std::string testVectorName;
};

typedef std::vector<DifficultyTestVector> TestVector;

struct DifficultyTestInFilled : GCP_SPointerBase
{
    DifficultyTestInFilled(spDataObject&);
    std::string const& testName() const { return m_name; }
    std::map<std::string, TestVector> const& testVectors() const { return m_testVectors; }

private:
    DifficultyTestInFilled() {}
    std::string m_name;
    std::map<std::string, TestVector> m_testVectors;
};

struct DifficultyTest
{
    DifficultyTest(spDataObject&);
    std::vector<DifficultyTestInFilled> const& tests() const { return m_tests; }

private:
    DifficultyTest() {}
    std::vector<DifficultyTestInFilled> m_tests;
};


}  // namespace teststruct
}  // namespace test
