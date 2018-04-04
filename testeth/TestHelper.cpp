#include <BuildInfo.h>
#include <boost/algorithm/string/trim.hpp>
#include <testeth/TestHelper.h>
#include <testeth/TestOutputHelper.h>
#include <testeth/Options.h>

using namespace std;
namespace fs = boost::filesystem;

namespace  test {

Json::Value readJson(string const& _s)
{
    Json::Value v;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(_s, v);
    if (!parsingSuccessful)
        BOOST_ERROR("Failed to parse configuration\n" + reader.getFormattedErrorMessages());
    return v;
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
			using fsIterator = fs::directory_iterator;
			for (fsIterator it(_dirPath); it != fsIterator(); ++it)
			{
				if (fs::is_regular_file(it->path()) && it->path().extension() == ext)
					files.push_back(it->path());
			}
		}
	}
	return files;
}

boost::filesystem::path getTestPath()
{
	if (!Options::get().testpath.empty())
		return Options::get().testpath;

	string testPath;
	const char* ptestPath = getenv("ETHEREUM_TEST_PATH");

	if (ptestPath == nullptr)
	{
//		ctest << " could not find environment variable ETHEREUM_TEST_PATH \n";
		testPath = "../../test/jsontests";
	}
	else
		testPath = ptestPath;

	return boost::filesystem::path(testPath);
}

void copyFile(fs::path const& _source, fs::path const& _destination)
{
	fs::ifstream src(_source, ios::binary);
	fs::ofstream dst(_destination, ios::binary);
	dst << src.rdbuf();
}

void requireJsonFields(DataObject const& _o, string const& _section,
	map<string, possibleType> const& _validationMap)
{
	// check for unexpected fiedls
	for (auto const field : _o.getSubObjects())
		ETH_CHECK_MESSAGE(_validationMap.count(field.getKey()),
			field.getKey() + " should not be declared in " + _section + " section!");

	// check field types with validation map
	for (auto const vmap : _validationMap)
	{
		ETH_REQUIRE_MESSAGE(_o.count(vmap.first) > 0, vmap.first + " not found in " + _section +
															" section! " +
															TestOutputHelper::get().testName());
		bool matched = false;
		string sTypes;
		for(auto const& type: vmap.second)
		{
			if (sTypes.size())
				sTypes += ", or ";
			sTypes += DataObject::dataTypeAsString(type);
			if (_o.at(vmap.first).type() == type)
				matched = true;
		}
		if (matched == false)
		{
			BOOST_ERROR(_section + " " + vmap.first + " expected to be " + sTypes +
					", but set to " + DataObject::dataTypeAsString(_o.at(vmap.first).type()) + " in " +
					TestOutputHelper::get().testName());
		}
	}
}


string jsonTypeAsString(Json::ValueType _type)
{
	switch (_type) {
	case Json::ValueType::nullValue: return "null";
	case Json::ValueType::intValue: return "int";
	case Json::ValueType::uintValue: return "uint";
	case Json::ValueType::realValue: return "double";
	case Json::ValueType::stringValue: return "string";
	case Json::ValueType::booleanValue: return "bool";
	case Json::ValueType::arrayValue: return "array";
	case Json::ValueType::objectValue: return "object";
	default:
		break;
	}
	return "";
}

DataObject convertJsonCPPtoData(Json::Value const& _input)
{
	if (_input.isNull())
        return DataObject(DataType::Object); // threat json "null" as empty object

	if (_input.isString())
		return DataObject(_input.asString());

	if (_input.isInt())
		return DataObject(_input.asInt());

	if (_input.isArray())
	{
		DataObject root(DataType::Array);
		for (Json::ArrayIndex i = 0; i < _input.size(); i++)
			root.addArrayObject(convertJsonCPPtoData(_input.get(i, Json::Value())));
		return root;
	}

	if (_input.isObject())
	{
		DataObject root(DataType::Object);
		for (auto const& i: _input)
			root.addSubObject(convertJsonCPPtoData(i));

		size_t index = 0;
		for (auto const& i: _input.getMemberNames())
			root.setSubObjectKey(index++, i);
		return root;
	}

	BOOST_ERROR("Error parsing JSON node. Element type not defined! " + jsonTypeAsString(_input.type()));
	return DataObject(DataType::Null);
}

vector<string> const& getNetworks()
{
    static vector<string> networks;
	if(networks.size() == 0)
	{
        networks.push_back("Frontier");
        networks.push_back("Homestead");
        networks.push_back("EIP150");
        networks.push_back("EIP158");
        networks.push_back("Byzantium");
		networks.push_back("Constantinople");
	}
	return networks;
}

/// translate network names in expect section field
/// >Homestead to EIP150, EIP158, Byzantium, ...
/// <=Homestead to Frontier, Homestead
set<string> translateNetworks(set<string> const& _networks)
{
    // construct vector with test network names in a right order (from Frontier to Homestead ... to
    // Constantinople)
    vector<string> const& forks = getNetworks();

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
            checkAllowedNetwork(net);
            out.emplace(net);
        }
    }
    return out;
}


void checkAllowedNetwork(string const& _network)
{
    bool found = false;
    vector<string> const& allowedNetowks = getNetworks();
    for(auto const& net: allowedNetowks)
    {
        if (net == _network)
            found = true;
    }

    if (!found)
    {
        // Can't use boost at this point
        std::cerr << TestOutputHelper::get().testName() + " Specified Network not found: "
                  << _network << "\n";
        exit(1);
    }
}

void parseJsonStrValueIntoSet(DataObject const& _json, set<string>& _out)
{
    if (_json.type() == DataType::Array)
    {
        for (auto const& val: _json.getSubObjects())
        {
             ETH_REQUIRE(val.type() == DataType::String);
            _out.emplace(val.asString());
        }
    }
    else
    {
        ETH_REQUIRE(_json.type() == DataType::String);
        _out.emplace(_json.asString());
    }
}

void parseJsonIntValueIntoSet(DataObject const& _json, set<int>& _out)
{
    if (_json.type() == DataType::Array)
    {
        for (auto const& val: _json.getSubObjects())
        {
            ETH_REQUIRE(val.type() == DataType::Integer);
            _out.emplace(val.asInt());
        }
    }
    else if (_json.type() == DataType::Integer)
    {
        ETH_REQUIRE(_json.type() == DataType::Integer);
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
    string result = test::executeCmd("lllc --version");
    string::size_type pos = result.rfind("Version");
    if (pos != string::npos)
        return result.substr(pos, result.length());
    return "Error getting LLLC Version";
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
        BOOST_ERROR("Failed to run " + _command);
    if (fgets(output, sizeof(output) - 1, fp) == NULL)
        BOOST_ERROR("Reading empty result for " + _command);
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
        BOOST_ERROR("The command '" + _command + "' exited with " + toString(exitCode) + " code.");
    return boost::trim_copy(out);
#endif
}

void checkHexHasEvenLength(string const& _hex)
{
	ETH_CHECK_MESSAGE(_hex.length() % 2 == 0,
		TestOutputHelper::get().testName() + ": Hex field is expected to be of odd length: '"
		 + _hex + "'");
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
	fs::remove(path);
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
		ETH_REQUIRE_MESSAGE(compiledCode.size() > 0,
			"Bytecode is missing! '" + _code + "' " + TestOutputHelper::get().testName());
	return compiledCode;
}

}//namespace
