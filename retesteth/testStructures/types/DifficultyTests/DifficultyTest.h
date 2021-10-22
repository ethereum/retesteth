#pragma once
#include "../StateTests/Filler/InfoIncomplete.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <map>

namespace test
{
namespace teststruct
{
struct DifficultyTestInFilled : GCP_SPointerBase
{
    DifficultyTestInFilled(spDataObject&);
    string const& testName() const { return m_name; }

private:
    DifficultyTestInFilled() {}
    string m_name;
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
