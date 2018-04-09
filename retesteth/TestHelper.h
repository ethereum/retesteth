#pragma once

#include <vector>
#include <set>
#include <boost/filesystem.hpp>
#include <json/json.h>

#include <retesteth/EthChecks.h>
#include <retesteth/DataObject.h>

namespace fs = boost::filesystem;
namespace test {

// Helping functions

/// Construct Json object from string
Json::Value readJson(std::string const& _s);

/// Get files from directory
std::vector<boost::filesystem::path> getFiles(boost::filesystem::path const& _dirPath, std::set<std::string> _extentionMask, std::string const& _particularFile = {});

/// Get test repo path from ETHEREUM_TEST_PATH environment variable
boost::filesystem::path getTestPath();

/// Copy file from _source to _destination
void copyFile(fs::path const& _source, fs::path const& _destination);

/// check the presents of fields in a DataObject with a validation map
typedef std::set<DataType> possibleType;
void requireJsonFields(DataObject const& _o, std::string const& _section, std::map<std::string, possibleType> const& _validationMap);

/// Read Json Object into DataObject
DataObject convertJsonCPPtoData(Json::Value const& _input);

/// Get Networks / Fork Rules
std::vector<std::string> const& getNetworks();

/// Translate smart network names into network names ( `<=Homestead` to `Frontier, Homestead`)
std::set<std::string> translateNetworks(std::set<std::string> const& _networks);

/// Check string to be a valid network name
void checkAllowedNetwork(std::string const& _network);

/// Read a single string object or an Array of string objects into set<string>
void parseJsonStrValueIntoSet(DataObject const& _json, std::set<std::string>& _out);

/// Read a single int object or an Array of int objects into set<int>
void parseJsonIntValueIntoSet(DataObject const& _json, std::set<int>& _out);

/// retesteth version string
std::string prepareVersionString();

/// local lllc version string
std::string prepareLLLCVersionString();

/// run system command
std::string executeCmd(std::string const& _command);

/// compile LLL / wasm or other src code into bytecode
std::string replaceCode(std::string const& _code);

}
