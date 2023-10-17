#pragma once

#include <vector>
#include <set>
#include <boost/filesystem.hpp>
#ifdef JSONCPP
#include <json/json.h>
#endif
#include <libdataobj/DataObject.h>
#include <libdataobj/ConvertFile.h>
#include <retesteth/compiler/Compiler.h>
#include <retesteth/testStructures/basetypes/BYTES.h>
#include <retesteth/testStructures/configs/FORK.h>

namespace test {
using namespace dataobject;
// Helping functions

#ifdef JSONCPP
/// Construct Json object from string
Json::Value readJson(boost::filesystem::path const& _path);
#endif

/// Safely read the json file into DataObject
spDataObject readJsonData(boost::filesystem::path const& _file, dataobject::CJOptions const& _opt = CJOptions());
spDataObject readYamlData(boost::filesystem::path const& _file, bool _sort = false);
spDataObject readAutoDataWithoutOptions(boost::filesystem::path const& _file, bool _sort = false);

/// Get files from directory
std::vector<boost::filesystem::path> getFiles(boost::filesystem::path const& _dirPath, std::set<std::string> const& _extentionMask, std::string const& _particularFile = {});

/// Get test repo path from ETHEREUM_TEST_PATH environment variable
boost::filesystem::path getTestPath();

/// Copy file from _source to _destination
void copyFile(boost::filesystem::path const& _source, boost::filesystem::path const& _destination);

/// Read a single string object or an Array of string objects into set<string>
void parseJsonStrValueIntoSet(DataObject const& _json, std::set<std::string>& _out);

/// Read a single int object or an Array of int objects into set<int>
void parseJsonIntValueIntoSet(DataObject const& _json, std::set<int>& _out);

/// Safe dev::fromHex
dev::bytes sfromHex(std::string const& _hexStr);

/// Informatice exception dev::toCompactHexPrefixed
std::string stoCompactHex(dev::u256 const& _val, int _minsize = 0);
std::string stoCompactHexPrefixed(dev::u256 const& _val, int _minsize = 0);

/// Convert string letters to lowercase
void strToLower(std::string& _input);

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
std::string executeCmd(std::string const& _command, int& _exitCode, ExecCMDWarning _warningOnEmpty = ExecCMDWarning::WarningOnEmptyResult);

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

std::vector<std::string> parseArgsFromStringIntoArray(std::string const& _stream, size_t& _pos, const char _delim = ' ');

/// Explode string into array of strings by `delim`
std::vector<std::string> explode(std::string const& s, char delim);
std::set<std::string> explodeIntoSet(std::string const& s, char delim);
void removeSubChar(std::string& _string, unsigned char _r);
void removeSubChar(std::string& _string, std::vector<unsigned char> _r);
std::string makePlussedFork(test::teststruct::FORK const& _net);
bool isBoostSuite(std::string const& suiteName);
size_t substrCount(std::string const& _str, std::string const& _needle);


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
boost::filesystem::path createUniqueTmpDirectory();

///
template <class t>
std::string fto_string(t _val)
{
    return std::to_string(_val);
}

/// RLP header calculator / Stream Emulator
class RLPStreamU
{
public:
    RLPStreamU(size_t _size);
    void appendRaw(std::string const& _data);
    void appendString(std::string const& _data);
    std::string outHeader() const;

private:
    bool m_wrapString = false;
    size_t m_size;
    std::string const* m_data = 0;
};

}  // namespace test
