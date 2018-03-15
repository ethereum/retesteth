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
		BOOST_CHECK_MESSAGE(_validationMap.count(field.getKey()),
			field.getKey() + " should not be declared in " + _section + " section!");

	// check field types with validation map
	for (auto const vmap : _validationMap)
	{
		BOOST_REQUIRE_MESSAGE(_o.count(vmap.first) > 0, vmap.first + " not found in " + _section +
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
             BOOST_REQUIRE(val.type() == DataType::String);
            _out.emplace(val.asString());
        }
    }
    else
    {
        BOOST_REQUIRE(_json.type() == DataType::String);
        _out.emplace(_json.asString());
    }
}

void parseJsonIntValueIntoSet(DataObject const& _json, set<int>& _out)
{
    if (_json.type() == DataType::Array)
    {
        for (auto const& val: _json.getSubObjects())
        {
            BOOST_REQUIRE(val.type() == DataType::Integer);
            _out.emplace(val.asInt());
        }
    }
    else if (_json.type() == DataType::Integer)
    {
        BOOST_REQUIRE(_json.type() == DataType::Integer);
        _out.emplace(_json.asInt());
    }
}

}//namespace
