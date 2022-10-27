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

TestFileData readTestFile(boost::filesystem::path const& _testFileName);
void removeComments(spDataObject& _obj);
void checkFillerHash(boost::filesystem::path const& _compiledTest, boost::filesystem::path const& _sourceTest);

}  // namespace testsuite
