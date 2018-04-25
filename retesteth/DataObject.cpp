#include <retesteth/DataObject.h>
using namespace  test;

/// Default dataobject is null
DataObject::DataObject() {	m_type = DataType::Null; }

/// Define dataobject of _type, pass the value later (will check the value and _type)
DataObject::DataObject(DataType _type) { m_type = _type; }

/// Define dataobject of string
DataObject::DataObject(std::string const& _str)
{
	m_type = DataType::String;
	m_strVal = _str;
}

/// Define dataobject[_key] = string
DataObject::DataObject(std::string const& _key, std::string const& _str)
{
	m_type = DataType::String;
	m_strVal = _str;
	m_strKey = _key;
}

/// Define dataobject of int
DataObject::DataObject(int _int)
{
	m_type = DataType::Integer;
	m_intVal = _int;
}

/// Get dataobject type
DataType DataObject::type() const {	return m_type; }

/// Set key of the dataobject
void DataObject::setKey(std::string const& _key) { m_strKey = _key; }

/// Get key of the dataobject
std::string const& DataObject::getKey() const {	return m_strKey; }

/// Get vector of subobjects
std::vector<DataObject>const& DataObject::getSubObjects() const { return m_subObjects; }

/// Get ref vector of subobjects
std::vector<DataObject>& DataObject::getSubObjectsUnsafe() { return m_subObjects; }

/// Add new subobject
void DataObject::addSubObject(DataObject const& _obj) {	_addSubObject(_obj); }

/// Add new subobject and set it's key
void DataObject::addSubObject(std::string const& _key, DataObject const& _obj)
{
	_addSubObject(_obj);
	setSubObjectKey(m_subObjects.size() - 1, _key);
}

/// Set key for subobject _index
void DataObject::setSubObjectKey(size_t _index, std::string const& _key)
{
	_assert(_index < m_subObjects.size(), "_index < m_subObjects.size()");
	if (m_subObjects.size() > _index)
		m_subObjects.at(_index).setKey(_key);
}

/// look if there is a subobject with _key
bool DataObject::count(std::string const& _key) const
{
	for (auto const& i: m_subObjects)
		if (i.getKey() == _key)
			return true;
	return false;
}

/// Get string value
std::string DataObject::asString() const
{
	_assert(m_type == DataType::String, "m_type == DataType::String");
	return m_strVal;
}

/// Get int value
int DataObject::asInt() const
{
	_assert(m_type == DataType::Integer, "m_type == DataType::Integer");
	return m_intVal;
}

/// Get bool value
bool DataObject::asBool() const
{
	_assert(m_type == DataType::Bool, "m_type == DataType::Bool");
	return m_boolVal;
}

/// Set position in vector of the subobject with _key
void DataObject::setKeyPos(std::string const& _key, size_t _pos)
{
	_assert(_pos < m_subObjects.size(), "_pos < m_subObjects.size()");
	_assert(count(_key), "count(_key)");
	_assert(!_key.empty(), "!_key.empty()");
	_checkDoubleKeys();
	for (size_t i = 0; i < m_subObjects.size(); i++)
	{
		if (m_subObjects[i].getKey() == _key)
		{
			if (i == _pos)
				return;	//item already at _pos;
			else
				break;
		}
	}

	std::vector<DataObject> newSubObjects;
	for (size_t i = 0; i < m_subObjects.size(); i++)
	{
		if (i == _pos)
		{
			if (i == m_subObjects.size() - 1 || (m_subObjects[i-1].getKey() == _key && i >= 1))
			{
				newSubObjects.push_back(m_subObjects[i]);
				newSubObjects.push_back(this->at(_key));
				continue;
			}
			newSubObjects.push_back(this->at(_key));
		}
		if (m_subObjects[i].getKey() != _key)
			newSubObjects.push_back(m_subObjects[i]);
	}
	m_subObjects.clear();
	m_subObjects = newSubObjects;
	_checkDoubleKeys();
}


/// replace this object with _value
void DataObject::replace(DataObject const& _value)
{
	m_strKey = _value.getKey();
	switch(_value.type())
	{
		case DataType::String:
			m_strVal = _value.asString();
			break;
		case DataType::Integer:
			m_intVal = _value.asInt();
			break;
		case DataType::Bool:
			m_boolVal = _value.asBool();
			break;
		default:
		break;
	}

	m_type = _value.type();
	m_subObjects.clear();
	m_subObjects = _value.getSubObjects();
}

DataObject const& DataObject::at(std::string const& _key) const
{
	_assert(count(_key), "count(_key)");
	for (auto const& i: m_subObjects)
		if (i.getKey() == _key)
			return i;
	return m_subObjects[0]; // should never hit this line
}

void DataObject::addArrayObject(DataObject const& _obj)
{
	_assert(m_type == DataType::Null || m_type == DataType::Array, "m_type == DataType::Null || m_type == DataType::Array");
	m_type = DataType::Array;
	m_subObjects.push_back(_obj);
}

void DataObject::renameKey(std::string const& _currentKey, std::string const& _newKey)
{
	if (m_strKey == _currentKey)
		m_strKey = _newKey;
	for (auto& obj : m_subObjects)
	{
		if (!obj.getKey().empty() && obj.getKey() == _currentKey)
		{
			obj.setKey(_newKey);
			break;
		}
	}
}

/// vector<element> erase method with `replace()` function
void DataObject::removeKey(std::string const& _key)
{
    _assert(type() == DataType::Object, "type() == DataType::Object");
    bool startReplace = false;
    for (std::vector<DataObject>::iterator it = m_subObjects.begin(); it != m_subObjects.end(); it++)
    {
        if ((*it).getKey() == _key)
            startReplace = true;
        std::vector<DataObject>::iterator next = it + 1;
        if (startReplace)
        {
            if (next != m_subObjects.end())
                (*it).replace(*next);
            else
            {
                m_subObjects.erase(it);
                break;
            }
        }
    }
}

void DataObject::clear()
{
    m_strKey = "";
    m_intVal = 0;
    m_subObjects.clear();
    m_type = DataType::Null;
}

void DataObject::print(size_t level) const
{
	for (size_t i = 0; i < level; i++)
		std::cerr << "-";
	std::cerr << dataTypeAsString(m_type) << " ";
	if (!m_strKey.empty())
		std::cerr << m_strKey << ": ";
	if (m_type == DataType::String)
		std::cerr << m_strVal;
	if (m_type == DataType::Integer)
		std::cerr << (m_intVal);
	std::cerr << std::endl;

	for (auto const& obj : m_subObjects)
		obj.print(level+1);
}

std::string DataObject::asJson(int level) const
{
	std::ostringstream out;
	auto printLevel = [level, &out]() -> void
	{
		for (int i = 0; i < level*4; i++)
			out << " ";
	};

	auto printElements = [this, &out, level]() -> void
	{
		for(std::vector<DataObject>::const_iterator it = this->m_subObjects.begin();
			it < this->m_subObjects.end(); it++)
		{
			out << (*it).asJson(level+1);
			if (it+1 != this->m_subObjects.end())
				out << ",";
			out << std::endl;
		}
	};

	switch(m_type)
	{
		case DataType::Null:
			printLevel();
			if (!m_strKey.empty())
				out << "\"" << m_strKey << "\" : ";
			out << "\"" << "null" << "\"";
		break;
		case DataType::Object:
			if (!m_strKey.empty())
			{
				printLevel();
				out << "\"" << m_strKey << "\" : {" << std::endl;
			}
			else
			{
				printLevel();
				out << "{" << std::endl;
			}
			printElements();
			printLevel();
			out << "}";
		break;
		case DataType::Array:
			if (!m_strKey.empty())
			{
				printLevel();
				out << "\"" << m_strKey << "\" : [" << std::endl;
			}
			else
			{
				printLevel();
				out << "[" << std::endl;
			}
			printElements();
			printLevel();
			out << "]";
		break;
		case DataType::String:
			printLevel();
			if (!m_strKey.empty())
				out << "\"" << m_strKey << "\" : ";
			out << "\"" << m_strVal << "\"";
		break;
		case DataType::Integer:
			printLevel();
			if (!m_strKey.empty())
				out << "\"" << m_strKey << "\" : ";
			out << m_intVal;
		break;
		case DataType::Bool:
			printLevel();
			if (!m_strKey.empty())
				out << "\"" << m_strKey << "\" : ";
			out << m_boolVal;
		break;
		default:
			out << "unknown " << dataTypeAsString(m_type) << std::endl;
		break;
	}
	return out.str();
}

std::string DataObject::dataTypeAsString(DataType _type)
{
	switch (_type) {
	case String: return "string";
	case Integer: return "int";
	case Array: return "array";
	case Bool: return "bool";
	case Object: return "object";
	case Null: return "null";
	default:
	break;
	}
	return "";
}

void DataObject::_addSubObject(DataObject const& _obj)
{
	if (m_type == DataType::Null)
		m_type = DataType::Object;
	_checkDoubleKeys();
	m_subObjects.push_back(_obj);
}

void DataObject::_checkDoubleKeys() const
{
	_assert(m_type == DataType::Object, "m_type == DataType::Object");
	for(size_t i = 0; i < m_subObjects.size(); i++)
	{
		std::string const& key = m_subObjects[i].getKey();
		if (key.empty())
			continue;
		for(size_t j = i + 1; j < m_subObjects.size(); j++)
			_assert(m_subObjects[j].getKey() != key, "m_subObjects[j].getKey() != key");
	}
}

void DataObject::_assert(bool _flag, std::string const& _comment) const
{
	if (!_flag)
	{
		std::cerr << "Error in DataObject: " << std::endl;
		std::cerr << " key: '" << m_strKey << "'";
		std::cerr << " type: '" << dataTypeAsString(m_type) << "'" << std::endl;
		std::cerr << " assert: " << _comment << std::endl;
		assert(_flag);
	}
}
