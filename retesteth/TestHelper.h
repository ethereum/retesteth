#pragma once

#include <vector>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#ifdef JSONCPP
#include <json/json.h>
#endif
#include <libdataobj/DataObject.h>
#include <retesteth/EthChecks.h>
#include <retesteth/compiler/Compiler.h>
#include <retesteth/testStructures/basetypes/BYTES.h>

using namespace dataobject;
namespace fs = boost::filesystem;
namespace test {

// Helping functions

#ifdef JSONCPP
/// Construct Json object from string
Json::Value readJson(fs::path const& _path);
#endif

/// Safely read the json file into DataObject
spDataObject readJsonData(
    fs::path const& _file, string const& _stopper = string(), bool _autosort = false);
spDataObject readYamlData(fs::path const& _file, bool _sort = false);

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
std::string stoCompactHex(dev::u256 const& _val, int _minsize = 0);
std::string stoCompactHexPrefixed(dev::u256 const& _val, int _minsize = 0);

/// Convert string letters to lowercase
void strToLower(string& _input);

/// retesteth version string
std::string prepareVersionString();
int retestethVersion();

/// local lllc version string
std::string prepareLLLCVersionString();
std::string prepareSolidityVersionString();

/// check system command
bool checkCmdExist(std::string const& _command);

/// run system command
enum class ExecCMDWarning
{
    WarningOnEmptyResult,
    NoWarning,
    NoWarningNoError
};
std::string executeCmd(std::string const& _command, ExecCMDWarning _warningOnEmpty = ExecCMDWarning::WarningOnEmptyResult);

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

/// See what kind of a string is str
enum class DigitsType
{
    Decimal,
    Hex,
    UnEvenHex,
    HexPrefixed,
    UnEvenHexPrefixed,
    String
};
DigitsType stringIntegerType(std::string const& _string, bool _wasPrefix = false);

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

///
template <class t>
string fto_string(t _val)
{
    return std::to_string(_val);
}

/// RLP header calculator / Stream Emulator
class RLPStreamU
{
public:
    RLPStreamU(size_t _size) : m_size(_size)
    {
        if (m_size > 1)
            ETH_FAIL_MESSAGE("RLPStreamU does not support stream of multiple rlp items. It's a mock to wrap 1 transaction.");
    }
    void appendRaw(string const& _data);
    void appendString(string const& _data);
    string outHeader() const;

private:
    bool m_wrapString = false;
    size_t m_size;
    string const* m_data = 0;
};

}  // namespace test
