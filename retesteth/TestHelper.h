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

/// Read a single string object or an Array of string objects into set<string>
void parseJsonStrValueIntoSet(DataObject const& _json, std::set<std::string>& _out);

/// Read a single int object or an Array of int objects into set<int>
void parseJsonIntValueIntoSet(DataObject const& _json, std::set<int>& _out);

/// Safe dev::fromHex
dev::bytes sfromHex(string const& _hexStr);

/// Informatice exception dev::toCompactHexPrefixed
std::string stoCompactHexPrefixed(dev::u256 const& _val, int _minsize = 0);

/// Convert string letters to lowercase
void strToLower(string& _input);

/// retesteth version string
std::string prepareVersionString();

/// local lllc version string
std::string prepareLLLCVersionString();

/// check system command
bool checkCmdExist(std::string const& _command);

/// run system command
enum class ExecCMDWarning
{
    WarningOnEmptyResult,
    NoWarning
};
std::string executeCmd(std::string const& _command, ExecCMDWarning _warningOnEmpty = ExecCMDWarning::WarningOnEmptyResult);

/// compile LLL / wasm or other src code into bytecode
std::string replaceCode(std::string const& _code);

// Return the vector of most looking like as _needles strings from the vector
std::vector<std::string> levenshteinDistance(
    std::string const& _needle, std::vector<std::string> const& _sVec, size_t _max = 3);


/// Find element in array as vector
template <class T>
bool inArray(std::vector<T> const& _array, const T& _val)
{
    if (std::find(_array.begin(), _array.end(), _val) == _array.end())
        return false;
    return true;
}

/// Find element in array as list
template <class T>
bool inArray(std::list<T> const& _array, const T& _val)
{
    if (std::find(_array.begin(), _array.end(), _val) == _array.end())
        return false;
    return true;
}

/// Explode string into array of strings by `delim`
std::vector<std::string> explode(std::string const& s, char delim);

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

/// to
string fto_string(size_t);

}  // namespace test
