#pragma once

#include <vector>
#include <set>
#include <boost/filesystem.hpp>
#ifdef JSONCPP
#include <json/json.h>
#endif
#include <dataObject/DataObject.h>
#include <retesteth/EthChecks.h>

using namespace dataobject;
namespace fs = boost::filesystem;
namespace test {

// Helping functions

#ifdef JSONCPP
/// Construct Json object from string
Json::Value readJson(fs::path const& _path);
#endif

/// Safely read the json file into DataObject
DataObject readJsonData(
    fs::path const& _file, string const& _stopper = string(), bool _autosort = false);
DataObject readYamlData(fs::path const& _file);

/// Get files from directory
std::vector<boost::filesystem::path> getFiles(boost::filesystem::path const& _dirPath, std::set<std::string> _extentionMask, std::string const& _particularFile = {});

/// Get test repo path from ETHEREUM_TEST_PATH environment variable
boost::filesystem::path getTestPath();

/// Copy file from _source to _destination
void copyFile(fs::path const& _source, fs::path const& _destination);

/// Translate smart network names into network names ( `<=Homestead` to `Frontier, Homestead`)
std::set<std::string> translateNetworks(
    std::set<std::string> const& _networks, vector<string> const& _networkOrder);

/// Check string to be a valid network name
void checkAllowedNetwork(std::string const& _network, vector<string> const& _networkOrder);

/// Read a single string object or an Array of string objects into set<string>
void parseJsonStrValueIntoSet(DataObject const& _json, std::set<std::string>& _out);

/// Read a single int object or an Array of int objects into set<int>
void parseJsonIntValueIntoSet(DataObject const& _json, std::set<int>& _out);

/// retesteth version string
std::string prepareVersionString();

/// local lllc version string
std::string prepareLLLCVersionString();

/// check system command
bool checkCmdExist(std::string const& _command);

/// run system command
std::string executeCmd(std::string const& _command);

/// compile LLL / wasm or other src code into bytecode
std::string replaceCode(std::string const& _code);

/// find element in array
template <class T>
bool inArray(std::vector<T> const& _array, const T& _val)
{
    for (auto const& obj : _array)
        if (obj == _val)
            return true;
    return false;
}


/// popen with pid at return
enum popenOutput
{
    DisableAll,
    EnableSTDOUT,
    EnableSTDERR,
    EnableALL
};
FILE* popen2(std::string const& _command, std::vector<std::string>const& _args, std::string const& _type, int& _pid, popenOutput _debug = popenOutput::DisableAll);
int pclose2(FILE* _fp, pid_t _pid);

/// return path to the unique tmp directory
fs::path createUniqueTmpDirectory();

/// return strings from _testList most matching _sMinusTArg
std::vector<std::string> testSuggestions(
    std::vector<std::string> const& _testList, std::string const& _sMinusTArg);
}
