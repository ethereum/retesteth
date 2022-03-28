#pragma once
#include <dataObject/DataObject.h>
#include <libdevcore/SHA3.h>
#include <boost/filesystem/path.hpp>
using namespace dataobject;
namespace fs = boost::filesystem;

namespace test
{
namespace testsuite
{
struct TestFileData
{
    spDataObject data;
    dev::h256 hash;
    bool hashCalculated = true;
};

bool addClientInfo(DataObject& _filledTest, fs::path const& _testSource, dev::h256 const& _testSourceHash,
    fs::path const& _existingFilledTest);

TestFileData readTestFile(fs::path const& _testFileName);
void removeComments(spDataObject& _obj);
void checkFillerHash(fs::path const& _compiledTest, fs::path const& _sourceTest);

}  // namespace testsuite
}  // namespace test
