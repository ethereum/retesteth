#pragma once
#include <libdataobj/DataObject.h>
#include <libdevcore/SHA3.h>
#include <boost/filesystem/path.hpp>
namespace test::testsuite
{
using namespace dataobject;

struct TestFileData
{
    spDataObject data;
    dev::h256 hash;
    bool hashCalculated = true;
};


bool addClientInfoIfUpdate(DataObject& _filledTest, boost::filesystem::path const& _testSource, dev::h256 const& _testSourceHash,
    boost::filesystem::path const& _existingFilledTest);

TestFileData readFillerTestFile(boost::filesystem::path const& _testFileName);
void removeComments(spDataObject& _obj);
bool checkFillerHash(boost::filesystem::path const& _compiledTest, boost::filesystem::path const& _sourceTest);

void clearGeneratedTestNamesMap();
void checkDoubleGeneratedTestNames();
std::vector<std::string> const& getGeneratedTestNames(boost::filesystem::path const& _filler);

}  // namespace testsuite
