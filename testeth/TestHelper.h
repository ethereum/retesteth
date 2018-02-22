#pragma once

#include <vector>
#include <set>
#include <boost/filesystem.hpp>
#include <json/json.h>
#include <testeth/DataObject.h>
namespace fs = boost::filesystem;
namespace test {

// Helping functions

/// Get files from directory
std::vector<boost::filesystem::path> getFiles(boost::filesystem::path const& _dirPath, std::set<std::string> _extentionMask, std::string const& _particularFile = {});

/// Get test repo path from ETHEREUM_TEST_PATH environment variable
boost::filesystem::path getTestPath();

/// Copy file from _source to _destination
void copyFile(fs::path const& _source, fs::path const& _destination);

/// check the presents of fields in a DataObject with a validation map
void requireJsonFields(DataObject const& _o, std::string const& _section, std::map<std::string, DataType> const& _validationMap);

/// Read Json Object into DataObject
DataObject convertJsonCPPtoData(Json::Value const& _input);


}
