#include <BuildInfo.h>
#include <fcntl.h>
#include <boost/algorithm/string/trim.hpp>
#include <boost/uuid/uuid_generators.hpp>  // generators
#include <boost/uuid/uuid_io.hpp>
#include <libdataobj/ConvertFile.h>
#include <libdataobj/ConvertYaml.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace boost::unit_test;
namespace fs = boost::filesystem;

namespace  test {
#ifdef JSONCPP
Json::Value readJson(fs::path const& _file)
{
    Json::Value v;
    Json::Reader reader;
    string s = dev::contentsString(_file);
    string const& fname = _file.filename().c_str();
    ETH_ERROR_REQUIRE_MESSAGE(s.length() > 0, "Contents of " + fname +
                                                  " is empty. Have you cloned the 'tests' repo branch "
                                                  "develop and set ETHEREUM_TEST_PATH to its path?");
    bool parsingSuccessful = reader.parse(s, v);
    if (!parsingSuccessful)
        ETH_ERROR_MESSAGE("Failed to parse json file\n" + reader.getFormattedErrorMessages() + "(" + fname + ")");
    return v;
}
#endif

/// Safely read the json file into DataObject
spDataObject readJsonData(fs::path const& _file, CJOptions const& _opt)
{
    try
    {
        const string s = dev::contentsString(_file);
        ETH_ERROR_REQUIRE_MESSAGE(
            s.length() > 0, "Contents of " + _file.string() + " is empty. Trying to parse empty file. (forgot --filltests?)");
        return dataobject::ConvertJsoncppStringToData(s, _opt);
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("\nError when parsing file (") + _file.c_str() + ") " + _ex.what());
        return spDataObject(0);
    }
}

/// Safely read the yaml file into DataObject
spDataObject readYamlData(fs::path const& _file, bool _sort)
{
    try
    {
        const string s = dev::contentsString(_file);
        ETH_ERROR_REQUIRE_MESSAGE(
            s.length() > 0, "Contents of " + _file.string() + " is empty. Trying to parse empty file. (forgot --filltests?)");
        return dataobject::ConvertYamlToData(YAML::Load(s), _sort);
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("\nError when parsing file (") + _file.c_str() + ") " + _ex.what());
        return spDataObject(0);
    }
}

spDataObject readAutoDataWithoutOptions(boost::filesystem::path const& _file, bool _sort)
{
    try
    {
        const string s = dev::contentsString(_file);
        if (s.length() == 0)
            std::cerr << "Contents of " + _file.string() + " is empty. Trying to parse empty file." << std::endl;
        if (_file.extension() == ".json")
            return dataobject::ConvertJsoncppStringToData(s);
        else if (_file.extension() == ".yml")
            return dataobject::ConvertYamlToData(YAML::Load(s), _sort);
        std::cerr << "Unknown test file: " << _file.string() << std::endl;
    }
    catch (std::exception const& _ex)
    {
        std::cerr << string("\nError when parsing file (") + _file.c_str() + ") " + _ex.what() << std::endl;
    }
    return spDataObject(0);
}

vector<fs::path> getFiles(fs::path const& _dirPath, set<string> const& _extentionMask, string const& _particularFile)
{
    vector<fs::path> files;
    for (auto const& ext : _extentionMask)
    {
        if (!_particularFile.empty())
        {
            const fs::path file = _dirPath / (_particularFile + ext);
            if (fs::exists(file))
                files.emplace_back(file);
        }
        else
        {
            if (fs::exists(_dirPath))
            {
                using fsIterator = fs::directory_iterator;
                for (fsIterator it(_dirPath); it != fsIterator(); ++it)
                {
                    if (fs::is_regular_file(it->path()) && it->path().extension() == ext)
                        files.emplace_back(it->path());
                }
            }
        }
    }
    return files;
}

boost::filesystem::path getTestPath()
{
    if (!Options::get().testpath.empty())
        return Options::get().testpath;

    static boost::filesystem::path testPath;
    if (!testPath.empty())
        return testPath;

    const char* ptestPath = getenv("ETHEREUM_TEST_PATH");
    if (ptestPath == nullptr)
    {
        ETH_WARNING("Could not find environment variable `ETHEREUM_TEST_PATH`");
        ETH_WARNING("Use the --testpath <path> option to set the test path!");
        testPath = boost::filesystem::path(boost::filesystem::current_path());
        return testPath;
    }
    testPath = boost::filesystem::path(ptestPath);
    return testPath;
}

void copyFile(fs::path const& _source, fs::path const& _destination)
{
    fs::ifstream src(_source, ios::binary);
    fs::ofstream dst(_destination, ios::binary);
    dst << src.rdbuf();
}

// A simple C++ implementation of the Levenshtein distance algorithm to measure the amount of
// difference between two strings. https://gist.github.com/TheRayTracer/2644387
size_t levenshteinDistance(char const* _s, size_t _n, char const* _t, size_t _m)
{
    ++_n;
    ++_m;
    size_t* d = new size_t[_n * _m];

    memset(d, 0, sizeof(size_t) * _n * _m);
    for (size_t i = 1, im = 0; i < _m; ++i, ++im)
    {
        for (size_t j = 1, jn = 0; j < _n; ++j, ++jn)
        {
            if (_s[jn] == _t[im])
                d[(i * _n) + j] = d[((i - 1) * _n) + (j - 1)];
            else
            {
                d[(i * _n) + j] = min(d[(i - 1) * _n + j] + 1, /* A deletion. */
                    min(d[i * _n + (j - 1)] + 1,               /* An insertion. */
                        d[(i - 1) * _n + (j - 1)] + 1));       /* A substitution. */
            }
        }
    }

    const size_t r = d[_n * _m - 1];
    delete[] d;
    return r;
}

vector<string> levenshteinDistance(std::string const& _needle, std::vector<std::string> const& _sVec, size_t _max)
{
    // <index in availableTests, compared distance>
    vector<string> ret;
    size_t allTestsElementIndex = 0;
    // Use `vector` here because `set` does not work with sort
    typedef std::pair<size_t, size_t> NameDistance;
    std::vector<NameDistance> distanceMap;
    for (auto const& it : _sVec)
    {
        const int dist = levenshteinDistance(_needle.c_str(), _needle.size(), it.c_str(), it.size());
        distanceMap.emplace_back(allTestsElementIndex++, dist);
    }
    std::sort(distanceMap.begin(), distanceMap.end(),
        [](NameDistance const& _a, NameDistance const& _b) { return _a.second < _b.second; });
    for (size_t i = 0; i < _max && i < distanceMap.size(); i++)
        ret.emplace_back(_sVec[distanceMap[i].first]);
    return ret;
}

std::mutex g_strFindMutex;
DigitsType stringIntegerType(std::string const& _string, bool _wasPrefix)
{
    if (_string.size() >= 2 && _string.at(0) == '0' && _string.at(1) == 'x' && !_wasPrefix)
    {
        const DigitsType substringType = stringIntegerType(_string, true);
        if (substringType == DigitsType::Hex)
            return DigitsType::HexPrefixed;

        if (substringType == DigitsType::Decimal)
        {
            if (_string.size() % 2 == 0)
                return DigitsType::HexPrefixed;
            else
                return DigitsType::UnEvenHexPrefixed;
        }

        if (substringType == DigitsType::UnEvenHex)
            return DigitsType::UnEvenHexPrefixed;
    }

    bool isDecimalOnly = true;
    std::lock_guard<std::mutex> lock(g_strFindMutex);  // string.find is not thread safe + static
    for (size_t i = _wasPrefix ? 2 : 0; i < _string.length(); i++)
    {
        if (!isxdigit(_string.at(i)))
            return DigitsType::String;

        if (isDecimalOnly && !isdigit(_string.at(i)))
            isDecimalOnly = false;
    }

    if (isDecimalOnly)
        return DigitsType::Decimal;

    if (_string.size() % 2 == 0)
        return DigitsType::Hex;

    return DigitsType::UnEvenHex;
}

void parseJsonStrValueIntoSet(DataObject const& _json, set<string>& _out)
{
    if (_json.type() == DataType::Array)
    {
        for (auto const& val: _json.getSubObjects())
        {
            ETH_ERROR_REQUIRE_MESSAGE(val->type() == DataType::String, "parseJsonStrValueIntoSet expected value type = string!");
            _out.emplace(val->asString());
        }
    }
    else
    {
        ETH_ERROR_REQUIRE_MESSAGE(_json.type() == DataType::String, "parseJsonStrValueIntoSet expected json type = string!");
        _out.emplace(_json.asString());
    }
}

void parseJsonIntValueIntoSet(DataObject const& _json, set<int>& _out)
{
    auto parseRange = [&_out](DataObject const& a) {
        const string& s = a.asString();
        const size_t delimeter = s.find('-');
        if (delimeter != string::npos)
        {
            const string firstPartString = s.substr(0, delimeter);
            if (stringIntegerType(firstPartString) != DigitsType::Decimal)
                ETH_ERROR_MESSAGE("parseJsonIntValueIntoSet require x to be decimal in `x-y` range! `" + firstPartString);

            const string secondPartString = s.substr(delimeter + 1);
            if (stringIntegerType(secondPartString) != DigitsType::Decimal)
                ETH_ERROR_MESSAGE("parseJsonIntValueIntoSet require y to be decimal in `x-y` range! `" + secondPartString);

            const size_t indexStart = atoi(firstPartString.c_str());
            const size_t indexEnd = atoi(secondPartString.c_str());
            for (size_t i = indexStart; i <= indexEnd; i++)
                _out.emplace(i);
        }
        else
            ETH_ERROR_MESSAGE("parseJsonIntValueIntoSet: Error parsing integer range string! format: \"x-y\", got: `" + s);
    };

    if (_json.type() == DataType::Array)
    {
        for (auto const& val: _json.getSubObjects())
        {
            if (val->type() == DataType::Integer)
                _out.emplace(val->asInt());
            else
            {
                ETH_ERROR_REQUIRE_MESSAGE(
                    val->type() == DataType::String, "parseJsonIntValueIntoSet expected value type = int, \"int-int\" range!");
                parseRange(val);
            }
        }
    }
    else if (_json.type() == DataType::Integer)
    {
        ETH_ERROR_REQUIRE_MESSAGE(_json.type() == DataType::Integer, "parseJsonIntValueIntoSet expected json type = int!");
        _out.emplace(_json.asInt());
    }
    else if (_json.type() == DataType::String)
    {
        // Try to parse range into values "x-y"
        parseRange(_json);
    }
}

string prepareVersionString()
{
    // cpp-1.3.0+commit.6be76b64.Linux.g++
    const string commit(DEV_QUOTED(ETH_COMMIT_HASH));
    string version = "retesteth-" + string(ETH_PROJECT_VERSION) + "-" + string(ETH_VERSION_SUFFIX);
    version += "+commit." + commit.substr(0, 8);
    version += "." + string(DEV_QUOTED(ETH_BUILD_OS)) + "." + string(DEV_QUOTED(ETH_BUILD_COMPILER));
    return version;
}

int retestethVersion()
{
    static int iversion = 0;
    if (iversion == 0)
    {
        string version = string(ETH_PROJECT_VERSION);
        version.erase(std::remove(version.begin(), version.end(), '.'), version.end());
        iversion = atoi(version.c_str());
    }
    return iversion;
}

std::mutex g_lllcversionMutex;
string prepareLLLCVersionString()
{
    std::lock_guard<std::mutex> lock(g_lllcversionMutex);
    static string lllcVersion;
    if (!lllcVersion.empty())
        return lllcVersion;
    if (test::checkCmdExist("lllc"))
    {
        int exitCode;
        const string cmd = "lllc --version";
        const string result = test::executeCmd(cmd, exitCode);
        const string::size_type pos = result.rfind("Version");
        if (pos != string::npos)
        {
            lllcVersion = result.substr(pos, result.length());
            return lllcVersion;
        }
    }
    lllcVersion = "Error getting LLLC Version";
    ETH_WARNING(lllcVersion);
    return lllcVersion;
}

std::mutex g_solcversionMutex;
string prepareSolidityVersionString()
{
    std::lock_guard<std::mutex> lock(g_solcversionMutex);
    static string solcVersion;
    if (!solcVersion.empty())
        return solcVersion;

    if (test::checkCmdExist("solc"))
    {
        int exitCode;
        const string cmd = "solc --version";
        const string result = test::executeCmd(cmd, exitCode);
        const string cVersion  = "Version";
        const string::size_type pos = result.rfind(cVersion);
        if (pos != string::npos)
        {
            solcVersion = result.substr(pos, result.length());
            string number;
            const string numberDirty = solcVersion.substr(cVersion.length(), 15);
            for (auto const& chr : numberDirty)
            {
                if (std::isdigit(chr))
                    number += chr;
            }
            int v = std::atoi(number.c_str());
            if (v < 85)
                ETH_WARNING("Solidity version detected (" + solcVersion + ") is less then '0.8.5', "
                    "ethereum test filling require version >= 0.8.5 to fill with correct results!");
            return solcVersion;
        }
    }
    solcVersion = "Error getting solc Version";
    ETH_WARNING(solcVersion);
    return solcVersion;
}

/// Safe dev::fromHex
dev::bytes sfromHex(string const& _hexStr)
{
    try
    {
        return dev::fromHex(_hexStr);
    }
    catch (BadHexCharacter const&)
    {
        ETH_ERROR_MESSAGE("Bad hex character around: " + _hexStr);
        return dev::bytes();
    }
}

std::string stoCompactHexPrefixed(dev::u256 const& _val, int _minsize)
{
    try
    {
        return dev::toCompactHexPrefixed(_val, _minsize);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(
            string("toCompactHexPrefixed error converting `" + _val.str() + "` to compact hex prefixed") + _ex.what());
    }
    return string();
}

std::string stoCompactHex(dev::u256 const& _val, int _minsize)
{
    try
    {
        return dev::toCompactHex(_val, _minsize);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("toCompactHex error converting `" + _val.str() + "` to compact hex") + _ex.what());
    }
    return string();
}

void strToLower(string& _input)
{
    std::transform(_input.begin(), _input.end(), _input.begin(), [](unsigned char c) { return std::tolower(c); });
}

bool checkCmdExist(std::string const& _command)
{
    string cmd;
    const size_t pos = _command.find_first_of(" ");
    if (pos != string::npos)
        cmd = _command.substr(0, pos);
    else
        cmd = _command;

    const string checkCmd = string("which " + cmd + " > /dev/null 2>&1");
    const bool checkBoost = fs::exists(cmd);
    if (!checkBoost && system(checkCmd.c_str()))
        return false;
    return true;
}

mutex g_popenmutex;
string executeCmd(string const& _command, int& _exitCode, ExecCMDWarning _warningOnEmpty)
{
#if defined(_WIN32)
    BOOST_ERROR("executeCmd() has not been implemented for Windows.");
    return "";
#else
    string out;
    char output[1024];
    ETH_FAIL_REQUIRE_MESSAGE(!_command.empty(), "executeCmd: empty argument!");

    // TODO: do this check only once and remember that it exist? syscalls are expensive
    if (!test::checkCmdExist(_command))
        ETH_FAIL_MESSAGE("Command `" + _command + "` does not found!");

    FILE* fp;
    {
        std::lock_guard<std::mutex> lock(g_popenmutex);
        fp = popen(_command.c_str(), "r");
    }
    if (fp == NULL || fp == 0)
        ETH_FAIL_MESSAGE("Failed to run " + _command);
    if (fgets(output, sizeof(output) - 1, fp) == NULL)
    {
        if (_warningOnEmpty == ExecCMDWarning::WarningOnEmptyResult)
            ETH_WARNING("Reading empty result for " + _command);
    }
    else
    {
        while (true)
        {
            out += string(output);
            if (fgets(output, sizeof(output) - 1, fp) == NULL)
                break;
        }
    }

    _exitCode = pclose(fp);
    if (_exitCode != 0 )
    {
        const string msg = "The command '" + _command + "' exited with " + toString(_exitCode) + " code.";
        if (_warningOnEmpty != ExecCMDWarning::NoWarningNoError)
            ETH_ERROR_MESSAGE(msg);
        else
            return msg;
    }
    return boost::trim_copy(out);
#endif
}

/// Explode string into array of strings by `delim`
std::vector<std::string> explode(std::string const& s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);
    for (std::string token; std::getline(iss, token, delim);)
        result.emplace_back(token);
    return result;
}
std::set<std::string> explodeIntoSet(std::string const& s, char delim)
{
    std::set<std::string> result;
    std::istringstream iss(s);
    for (std::string token; std::getline(iss, token, delim);)
        result.emplace(token);
    return result;

}

#define READ   0
#define WRITE  1
#define EXECLARG0(cmd) execl(cmd, cmd, (char*)NULL)
#define EXECLARG1(cmd, arg1) execl(cmd, cmd, arg1, (char*)NULL)
#define EXECLARG2(cmd, arg1, arg2) execl(cmd, cmd, arg1, arg2, (char*)NULL)
#define EXECLARG3(cmd, arg1, arg2, arg3) execl(cmd, cmd, arg1, arg2, arg3, (char*)NULL)
#define EXECLARG4(cmd, arg1, arg2, arg3, arg4) execl(cmd, cmd, arg1, arg2, arg3, arg4, (char*)NULL)
#define EXECLARG5(cmd, arg1, arg2, arg3, arg4, arg5) execl(cmd, cmd, arg1, arg2, arg3, arg4, arg5, (char*)NULL)
#define EXECLARG6(cmd, arg1, arg2, arg3, arg4, arg5, arg6) execl(cmd, cmd, arg1, arg2, arg3, arg4, arg5, arg6, (char*)NULL)
#define EXECLARG7(cmd, arg1, arg2, arg3, arg4, arg5, arg6, arg7) execl(cmd, cmd, arg1, arg2, arg3, arg4, arg5, arg6, arg7, (char*)NULL)
#define EXECLARG8(cmd, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) execl(cmd, cmd, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, (char*)NULL)

//https://stackoverflow.com/questions/26852198/getting-the-pid-from-popen
FILE* popen2(string const& _command, vector<string> const& _args, string const& _type, int& _pid, popenOutput _debug)
{
    string testIfCmdExist = "which " + _command;
    if (system(testIfCmdExist.c_str()) == 256)
        ETH_FAIL_MESSAGE("Command " + _command + " not found in the system!");

    pid_t child_pid;
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if((child_pid = fork()) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /* child process */
    if (child_pid == 0)
    {
        if (_type == "r")
        {
            close(fd[READ]);    //Close the READ end of the pipe since the child's fd is write-only
            int fdo = open("/dev/null", O_WRONLY);
            switch (_debug) {
            case popenOutput::DisableAll:
                dup2(fdo, 1);
                dup2(fdo, 2);
                break;
            case popenOutput::EnableSTDOUT:
                dup2(fdo, 2);
                break;
            case popenOutput::EnableSTDERR:
                dup2(fdo, 1);
                break;
            case popenOutput::EnableALL: break;
            default:
                break;
            }
        }
        else
        {
            close(fd[WRITE]);    //Close the WRITE end of the pipe since the child's fd is read-only
            dup2(fd[READ], 0);   //Redirect stdin to pipe
        }

        setpgid(child_pid, child_pid); //Needed so negative PIDs can kill children of /bin/sh
        string cmd("/bin/" + _command);
        switch(_args.size())
        {
        case 0:
            EXECLARG0(cmd.c_str());
            break;
        case 1:
            EXECLARG1(cmd.c_str(), _args[0].c_str());
            break;
        case 2:
            EXECLARG2(cmd.c_str(), _args[0].c_str(), _args[1].c_str());
            break;
        case 3:
            EXECLARG3(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str());
            break;
        case 4:
            EXECLARG4(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str());
            break;
        case 5:
            EXECLARG5(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str());
            break;
        case 6:
            EXECLARG6(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str(),
                _args[5].c_str());
            break;
        case 7:
            EXECLARG7(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str(),
                _args[5].c_str(), _args[6].c_str());
            break;
        case 8:
            EXECLARG8(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str(),
                _args[5].c_str(), _args[6].c_str(), _args[7].c_str());
            break;
        default:
            ETH_STDERROR_MESSAGE("Wrong number of arguments provided in popen2!");
        }
        exit(0);
    }
    else
    {
        if (_type == "r")
            close(fd[WRITE]); //Close the WRITE end of the pipe since parent's fd is read-only
        else
            close(fd[READ]); //Close the READ end of the pipe since parent's fd is write-only
    }

    _pid = child_pid;

    if (_type == "r")
        return fdopen(fd[READ], "r");

    return fdopen(fd[WRITE], "w");
}

std::mutex g_pclosemutex;
int pclose2(FILE* _fp, pid_t _pid)
{
    const string cmd = "kill " + toString((long)_pid);
    std::lock_guard<std::mutex> lock(g_pclosemutex);
    if (_fp)
        pclose(_fp);
    int ret = system(cmd.c_str());
    return ret;
}

std::mutex g_createUniqueTmpDirectory;
fs::path createUniqueTmpDirectory() {
    std::lock_guard<std::mutex> lock(g_createUniqueTmpDirectory);
    const boost::uuids::uuid uuid = boost::uuids::random_generator()();
    const string uuidStr = boost::lexical_cast<string>(uuid);

    static auto tpath = fs::exists("/dev/shm") ? fs::path("/dev/shm") : fs::temp_directory_path();
    auto const& tmpDir = Options::getCurrentConfig().cfgFile().tmpDir();

    // tmpDir here is dynamic depengin on the client config
    if (tmpDir.empty())
    {
        if (fs::exists(tpath / uuidStr))
            ETH_FAIL_MESSAGE("boost create tmp directory which already exist!");
        boost::filesystem::create_directory(tpath / uuidStr);
        return tpath / uuidStr;
    }
    else
    {
        if (fs::exists(tmpDir / uuidStr))
            ETH_FAIL_MESSAGE("boost create tmp directory which already exist!");
        boost::filesystem::create_directory(tmpDir / uuidStr);
        return tmpDir / uuidStr;
    }
}

size_t substrCount(std::string const& _str, std::string const& _needle)
{
    size_t count = 0;
    size_t pos = _str.find(_needle);
    while (pos != string::npos)
    {
        pos = _str.find(_needle, pos + 1);
        count++;
    }
    return count;
}

// RLPStream emulator
void RLPStreamU::appendString(string const& _data)
{
    if (m_data != 0)
        ETH_FAIL_MESSAGE("RLPStreamU::appendString called more than 1 times!");
    m_data = &_data;
    m_wrapString = true;
}

void RLPStreamU::appendRaw(string const& _data)
{
    if (m_data != 0)
        ETH_FAIL_MESSAGE("RLPStreamU::appendRaw called more than 1 times!");
    m_data = &_data;
}

// RLPStream emulator
string RLPStreamU::outHeader() const
{
    // For a single byte whose value is in the [0x00, 0x7f] range, that byte is its own RLP encoding.
    // Otherwise, if a string is 0-55 bytes long, the RLP encoding consists of a single byte with value 0x80 plus the length of
    // the string followed by the string. The range of the first byte is thus [0x80, 0xb7]. If a string is more than 55 bytes
    // long, the RLP encoding consists of a single byte with value 0xb7 plus the length in bytes of the length of the string in
    // binary form, followed by the length of the string, followed by the string. For example, a length-1024 string would be
    // encoded as \xb9\x04\x00 followed by the string. The range of the first byte is thus [0xb8, 0xbf].

    // If the total payload of a list (i.e. the combined length of all its items being RLP encoded) is 0-55 bytes long, the RLP
    // encoding consists of a single byte with value 0xc0 plus the length of the list followed by the concatenation of the RLP
    // encodings of the items. The range of the first byte is thus [0xc0, 0xf7]. If the total payload of a list is more than 55
    // bytes long, the RLP encoding consists of a single byte with value 0xf7 plus the length in bytes of the length of the
    // payload in binary form, followed by the length of the payload, followed by the concatenation of the RLP encodings of the
    // items. The range of the first byte is thus [0xf8, 0xff].

    size_t header;
    long payloadSize = (m_data->size() / 2) - 1;
    string wrappedStringHeaderStr = "";
    if (m_wrapString)
    {
        // if a string is 0-55 bytes long, the RLP encoding consists of a single byte
        // with value 0x80 plus the length of the string followed by the string.
        // The range of the first byte is thus [0x80, 0xb7].

        // If a string is more than 55 bytes long, the RLP encoding consists of a single byte
        // with value 0xb7 plus the length in bytes of the length of the string in binary form,
        // followed by the length of the string, followed by the string.
        // For example, a length-1024 string would be encoded as \xb9\x04\x00 followed by the string.
        // The range of the first byte is thus [0xb8, 0xbf].

        if (payloadSize > 55)
        {
            auto const lengthOfTheString = dev::toCompactHex(payloadSize);
            auto const lengthInBytesOfTheLengthOfTheStringInBinaryForm = lengthOfTheString.size() / 2;

            const size_t wrappedStringHeader = 183 + lengthInBytesOfTheLengthOfTheStringInBinaryForm;
            wrappedStringHeaderStr = dev::toCompactHex(wrappedStringHeader) + lengthOfTheString;
        }
        else
        {
            const size_t wrappedStringHeader = 128 + payloadSize;
            wrappedStringHeaderStr = dev::toCompactHex(wrappedStringHeader);
        }
        payloadSize += wrappedStringHeaderStr.size() / 2;
    }

    if (payloadSize > 55)
    {
        auto const lengthOfThePayload = dev::toCompactHex(payloadSize);
        auto const theLengthInBytesOfTheLengthOfThePayloadInBinaryForm = lengthOfThePayload.size() / 2;
        header = 247 + theLengthInBytesOfTheLengthOfThePayloadInBinaryForm;
        return "0x" + dev::toCompactHex(header) + lengthOfThePayload + wrappedStringHeaderStr;
    }
    else
        header = 192 + payloadSize;
    return "0x" + dev::toCompactHex(header) + wrappedStringHeaderStr;
}

RLPStreamU::RLPStreamU(size_t _size) : m_size(_size)
{
    if (m_size > 1)
        ETH_FAIL_MESSAGE("RLPStreamU does not support stream of multiple rlp items. It's a mock to wrap 1 transaction.");
}

void removeSubChar(std::string& _string, std::vector<unsigned char> _r)
{
    _string.erase(
        std::remove_if(_string.begin(), _string.end(), [&_r](unsigned char ch) {
            for (auto const& charToRemove : _r)
                if (ch == charToRemove)
                    return true;
            return false;
        }), _string.end());
}

void removeSubChar(std::string& _string, unsigned char _r)
{
    _string.erase(
        std::remove_if(_string.begin(), _string.end(), [&_r](unsigned char ch) {
            return ch == _r;
        }), _string.end());
}

string makePlussedFork(FORK const& _net)
{
    const size_t pos = _net.asString().find("+");
    if (pos != string::npos)
        return _net.asString().substr(0, pos);
    return string();
}

bool isBoostSuite(std::string const& suiteName)
{
    const test_suite* suite = &boost::unit_test::framework::master_test_suite();
    auto const allSuites = test::explode(suiteName, '/');
    for (auto const& suiteName : allSuites)
    {
        auto const suiteid = suite->get(suiteName);
        if (suiteid != INV_TEST_UNIT_ID)
        {
            try
            {
                suite = &framework::get<test_suite>(suiteid);
            }
            catch (std::exception const&)
            {
                // asked test case
                return false;
            }
        }
        else
            return false;
    }
    return true;
}

std::vector<std::string> parseArgsFromStringIntoArray(std::string const& _stream, size_t& _pos, const char _delim)
{
    string arg;
    std::vector<std::string> args;
    for (; _pos < _stream.size()
           && _stream.at(_pos) != '\n'
           && _stream.at(_pos) != '{'; _pos++)
    {
        if (_stream.at(_pos) == _delim)
        {
            args.emplace_back(arg);
            arg = string();
            continue;
        }
        arg += _stream.at(_pos);
    }
    if (!arg.empty())
        args.emplace_back(arg);
    return args;
}

}//namespace
