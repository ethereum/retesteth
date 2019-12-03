#include <BuildInfo.h>
#include <fcntl.h>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid.hpp>             // uuid class
#include <boost/uuid/uuid_generators.hpp>  // generators
#include <boost/uuid/uuid_io.hpp>          // streaming operators etc
#include <boost/uuid/uuid_io.hpp>
#include <csignal>
#include <mutex>

#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/dataObject/ConvertFile.h>
#include <retesteth/dataObject/ConvertYaml.h>

using namespace std;
namespace fs = boost::filesystem;

namespace  test {
#ifdef JSONCPP
Json::Value readJson(fs::path const& _file)
{
    Json::Value v;
    Json::Reader reader;
    string s = dev::contentsString(_file);
    string const& fname = _file.filename().c_str();
    ETH_ERROR_REQUIRE_MESSAGE(
        s.length() > 0, "Contents of " + fname +
                            " is empty. Have you cloned the 'tests' repo branch "
                            "develop and set ETHEREUM_TEST_PATH to its path?");
    bool parsingSuccessful = reader.parse(s, v);
    if (!parsingSuccessful)
        ETH_ERROR_MESSAGE(
            "Failed to parse json file\n" + reader.getFormattedErrorMessages() + "(" + fname + ")");
    return v;
}
#endif

/// Safely read the json file into DataObject
DataObject readJsonData(fs::path const& _file, string const& _stopper, bool _autosort)
{
    try
    {
        string s = dev::contentsString(_file);
        ETH_ERROR_REQUIRE_MESSAGE(s.length() > 0,
            "Contents of " + _file.string() + " is empty. Trying to parse empty file.");
        return dataobject::ConvertJsoncppStringToData(s, _stopper, _autosort);
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(
            string("\nError when parsing file (") + _file.c_str() + ") " + _ex.what());
        return DataObject();
    }
}

/// Safely read the yaml file into DataObject
DataObject readYamlData(fs::path const& _file)
{
    try
    {
        string s = dev::contentsString(_file);
        ETH_ERROR_REQUIRE_MESSAGE(s.length() > 0,
            "Contents of " + _file.string() + " is empty. Trying to parse empty file.");
        return dataobject::ConvertYamlToData(YAML::Load(s));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(
            string("\nError when parsing file (") + _file.c_str() + ") " + _ex.what());
        return DataObject();
    }
}

vector<fs::path> getFiles(
	fs::path const& _dirPath, set<string> const _extentionMask, string const& _particularFile)
{
	vector<fs::path> files;
	for (auto const& ext : _extentionMask)
	{
		if (!_particularFile.empty())
		{
			fs::path file = _dirPath / (_particularFile + ext);
			if (fs::exists(file))
				files.push_back(file);
		}
		else
		{
            if (fs::exists(_dirPath))
            {
                using fsIterator = fs::directory_iterator;
                for (fsIterator it(_dirPath); it != fsIterator(); ++it)
                {
                    if (fs::is_regular_file(it->path()) && it->path().extension() == ext)
                        files.push_back(it->path());
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
        ETH_WARNING("WARNING: Could not find environment variable `ETHEREUM_TEST_PATH`");
        ETH_STDERROR_MESSAGE("Use the --testpath <path> option to set the test path!");
        throw EthError() << "Error getting the test path!";
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

    size_t r = d[_n * _m - 1];
    delete[] d;
    return r;
}

std::vector<std::string> testSuggestions(
    vector<string> const& _testList, std::string const& _sMinusTArg)
{
    vector<string> ret;
    size_t allTestsElementIndex = 0;
    // <index in availableTests, compared distance>
    typedef std::pair<size_t, size_t> NameDistance;
    // Use `vector` here because `set` does not work with sort
    std::vector<NameDistance> distanceMap;
    for (auto& it : _testList)
    {
        int const dist = test::levenshteinDistance(
            _sMinusTArg.c_str(), _sMinusTArg.size(), it.c_str(), it.size());
        distanceMap.emplace_back(allTestsElementIndex++, dist);
    }
    std::sort(distanceMap.begin(), distanceMap.end(),
        [](NameDistance const& _a, NameDistance const& _b) { return _a.second < _b.second; });
    for (size_t i = 0; i < 3 && i < distanceMap.size(); i++)
        ret.push_back(_testList[distanceMap[i].first]);
    return ret;
}

/// translate network names in expect section field
/// >Homestead to EIP150, EIP158, Byzantium, ...
/// <=Homestead to Frontier, Homestead
set<string> translateNetworks(set<string> const& _networks, vector<string> const& _networkOrder)
{
    // construct vector with test network names in a right order
    // (from Frontier to Homestead ... to Constantinople)
    vector<string> const& forks = _networkOrder;

    set<string> out;
    for (auto const& net : _networks)
    {
        bool isNetworkTranslated = false;
        string possibleNet = net.substr(1, net.length() - 1);
        vector<string>::const_iterator it = std::find(forks.begin(), forks.end(), possibleNet);

        if (it != forks.end() && net.size() > 1)
        {
            if (net[0] == '>')
            {
                while (++it != forks.end())
                {
                    out.emplace(*it);
                    isNetworkTranslated = true;
                }
            }
            else if (net[0] == '<')
            {
                while (it != forks.begin())
                {
                    out.emplace(*(--it));
                    isNetworkTranslated = true;
                }
            }
        }

        possibleNet = net.substr(2, net.length() - 2);
        it = std::find(forks.begin(), forks.end(), possibleNet);
        if (it != forks.end() && net.size() > 2)
        {
            if (net[0] == '>' && net[1] == '=')
            {
                while (it != forks.end())
                {
                    out.emplace(*(it++));
                    isNetworkTranslated = true;
                }
            }
            else if (net[0] == '<' && net[1] == '=')
            {
                out.emplace(*it);
                isNetworkTranslated = true;
                while (it != forks.begin())
                    out.emplace(*(--it));
            }
        }

        // if nothing has been inserted, just push the untranslated network as is
        if (!isNetworkTranslated)
        {
            checkAllowedNetwork(net, _networkOrder);
            out.emplace(net);
        }
    }
    return out;
}


void checkAllowedNetwork(string const& _network, vector<string> const& _networkOrder)
{
    bool found = false;
    for (auto const& net : _networkOrder)
    {
        if (net == _network)
            found = true;
    }

    if (!found)
    {
        fs::path configFile = Options::getDynamicOptions().getCurrentConfig().getConfigFilePath();
        ETH_WARNING(
            "Specified network not found: '" + _network +
            "', skipping the test. Enable the fork network in config file: " + configFile.string());
    }
}

void parseJsonStrValueIntoSet(DataObject const& _json, set<string>& _out)
{
    if (_json.type() == DataType::Array)
    {
        for (auto const& val: _json.getSubObjects())
        {
            ETH_ERROR_REQUIRE_MESSAGE(val.type() == DataType::String,
                "parseJsonStrValueIntoSet expected value type = string!");
            _out.emplace(val.asString());
        }
    }
    else
    {
        ETH_ERROR_REQUIRE_MESSAGE(_json.type() == DataType::String,
            "parseJsonStrValueIntoSet expected json type = string!");
        _out.emplace(_json.asString());
    }
}

void parseJsonIntValueIntoSet(DataObject const& _json, set<int>& _out)
{
    if (_json.type() == DataType::Array)
    {
        for (auto const& val: _json.getSubObjects())
        {
            ETH_ERROR_REQUIRE_MESSAGE(val.type() == DataType::Integer,
                "parseJsonIntValueIntoSet expected value type = int!");
            _out.emplace(val.asInt());
        }
    }
    else if (_json.type() == DataType::Integer)
    {
        ETH_ERROR_REQUIRE_MESSAGE(_json.type() == DataType::Integer,
            "parseJsonIntValueIntoSet expected json type = int!");
        _out.emplace(_json.asInt());
    }
}

string prepareVersionString()
{
    // cpp-1.3.0+commit.6be76b64.Linux.g++
    string commit(DEV_QUOTED(ETH_COMMIT_HASH));
	string version = "retesteth-" + string(ETH_PROJECT_VERSION);
    version += "+commit." + commit.substr(0, 8);
    version +=
        "." + string(DEV_QUOTED(ETH_BUILD_OS)) + "." + string(DEV_QUOTED(ETH_BUILD_COMPILER));
    return version;
}

string prepareLLLCVersionString()
{
    if (test::checkCmdExist("lllc"))
    {
        string result = test::executeCmd("lllc --version");
        string::size_type pos = result.rfind("Version");
        if (pos != string::npos)
            return result.substr(pos, result.length());
    }
    string const res = "Error getting LLLC Version";
    ETH_WARNING("WARNING: " + res);
    return res;
}

bool checkCmdExist(std::string const& _command)
{
    string checkCmd = string("which " + _command + " > /dev/null 2>&1");
    if (system(checkCmd.c_str()))
        return false;
    return true;
}

string executeCmd(string const& _command)
{
#if defined(_WIN32)
    BOOST_ERROR("executeCmd() has not been implemented for Windows.");
    return "";
#else
    string out;
    char output[1024];
    FILE* fp = popen(_command.c_str(), "r");
    if (fp == NULL)
        ETH_FAIL_MESSAGE("Failed to run " + _command);
    if (fgets(output, sizeof(output) - 1, fp) == NULL)
        ETH_WARNING("WARNING: Reading empty result for " + _command);
    else
    {
        while (true)
        {
            out += string(output);
            if (fgets(output, sizeof(output) - 1, fp) == NULL)
                break;
        }
    }

    int exitCode = pclose(fp);
    if (exitCode != 0)
        ETH_FAIL_MESSAGE(
            "The command '" + _command + "' exited with " + toString(exitCode) + " code.");
    return boost::trim_copy(out);
#endif
}

void checkHexHasEvenLength(string const& _hex)
{
    ETH_ERROR_REQUIRE_MESSAGE(
        _hex.length() % 2 == 0, TestOutputHelper::get().testName() +
                                    ": Hex field is expected to be of odd length: '" + _hex + "'");
}

string compileLLL(string const& _code)
{
#if defined(_WIN32)
	BOOST_ERROR("LLL compilation only supported on posix systems.");
	return "";
#else
	fs::path path(fs::temp_directory_path() / fs::unique_path());
	string cmd = string("lllc ") + path.string();
	writeFile(path.string(), _code);
	string result = executeCmd(cmd);
	fs::remove_all(path);
	result = "0x" + result;
	checkHexHasEvenLength(result);
	return result;
#endif
}

string replaceCode(string const& _code)
{
	if (_code == "")
		return "0x";

	if (_code.substr(0, 2) == "0x" && _code.size() >= 2)
	{
		checkHexHasEvenLength(_code);
		return _code;
	}

	//wasm support
	//if (_code.find("(module") == 0)
	//	return wast2wasm(_code);

	string compiledCode = compileLLL(_code);
	if (_code.size() > 0)
        ETH_FAIL_REQUIRE_MESSAGE(compiledCode.size() > 0,
			"Bytecode is missing! '" + _code + "' " + TestOutputHelper::get().testName());
	return compiledCode;
}

#include <sys/wait.h>
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
            case 0: EXECLARG0(cmd.c_str()); break;
            case 1: EXECLARG1(cmd.c_str(), _args[0].c_str()); break;
            case 2: EXECLARG2(cmd.c_str(), _args[0].c_str(), _args[1].c_str()); break;
            case 3: EXECLARG3(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str()); break;
            case 4: EXECLARG4(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str()); break;
            case 5: EXECLARG5(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str()); break;
            case 6: EXECLARG6(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str(), _args[5].c_str()); break;
            case 7: EXECLARG7(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str(), _args[5].c_str(), _args[6].c_str()); break;
            case 8: EXECLARG8(cmd.c_str(), _args[0].c_str(), _args[1].c_str(), _args[2].c_str(), _args[3].c_str(), _args[4].c_str(), _args[5].c_str(), _args[6].c_str(), _args[7].c_str()); break;
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
    string cmd = "kill " + toString((long)_pid);
    std::lock_guard<std::mutex> lock(g_pclosemutex);
    if (_fp)
        pclose(_fp);
    int ret = system(cmd.c_str());
    return ret;
}

std::mutex g_createUniqueTmpDirectory;
fs::path createUniqueTmpDirectory() {
  std::lock_guard<std::mutex> lock(g_createUniqueTmpDirectory);
  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  string uuidStr = boost::lexical_cast<string>(uuid);
  boost::filesystem::create_directory(fs::temp_directory_path() / uuidStr);
  return fs::temp_directory_path() / uuidStr;
}

}//namespace
