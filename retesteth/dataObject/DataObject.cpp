#include <dataObject/DataObject.h>
#include <algorithm>
#include <sstream>
using namespace dataobject;

/// Default dataobject is null
DataObject::DataObject()
{
    m_type = DataType::Null;
}

/// Define dataobject of _type, pass the value later (will check the value and _type)
DataObject::DataObject(DataType _type)
{
    m_type = _type;
}

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

DataObject::DataObject(std::string const& _key, int _val)
{
    m_type = DataType::Integer;
    m_intVal = _val;
    m_strKey = _key;
}

/// Define dataobject of int
DataObject::DataObject(int _int)
{
    m_type = DataType::Integer;
    m_intVal = _int;
}

/// Define dataobject of bool
DataObject::DataObject(DataType type, bool _bool)
{
    m_type = type;
    m_boolVal = _bool;
}

/// Get dataobject type
DataType DataObject::type() const
{
    return m_type;
}

/// Set key of the dataobject
void DataObject::setKey(std::string const& _key)
{
    m_strKey = _key;
}

/// Get key of the dataobject
std::string const& DataObject::getKey() const
{
    return m_strKey;
}

/// Get vector of subobjects
std::vector<DataObject> const& DataObject::getArraySubObjects() const
{
    _assert(m_type == DataType::Array);
    return m_arraySubObjects;
}

/// Get ref vector of subobjects
std::vector<DataObject>& DataObject::getArraySubObjectsUnsafe()
{
    _assert(m_type == DataType::Array);
    return m_arraySubObjects;
}

/// Get vector of subobjects
std::map<string, DataObjectInfo> const& DataObject::getMapSubObjects() const
{
    _assert(m_type == DataType::Object);
    return m_mapSubObjects;
}

/// Get ref vector of subobjects
std::map<string, DataObjectInfo>& DataObject::getMapSubObjectsUnsafe()
{
    _assert(m_type == DataType::Object);
    return m_mapSubObjects;
}

/*
/// Add new subobject
DataObject& DataObject::addSubObject(DataObject const& _obj)
{
    return _addSubObject(_obj);
}*/

/// Add new subobject and set it's key
DataObject& DataObject::addMapObject(std::string const& _key, DataObject const& _obj)
{
    return _addMapSubObject(_obj, _key);
}

/// Set key for subobject _index
/*void DataObject::setSubObjectKey(size_t _index, std::string const& _key)
{
    _assert(_index < m_subObjects.size(), "_index < m_subObjects.size() (DataObject::setSubObjectKey)");
    if (m_subObjects.size() > _index)
        m_subObjects.at(_index).setKey(_key);
}*/

// Helpers for fast subObject lookup
// Subobjects are in vector because double/empty keys and order in the vector
/*vector<DataObject>::const_iterator DataObject::subByKey(string const& _key) const
{
    return std::find_if(m_subObjects.begin(), m_subObjects.end(), [&_key](DataObject const& x) { return x.getKey() == _key; });
}
vector<DataObject>::iterator DataObject::subByKeyU(string const& _key)
{
    return std::find_if(m_subObjects.begin(), m_subObjects.end(), [&_key](DataObject const& x) { return x.getKey() == _key; });
}*/

/// look if there is a subobject with _key
bool DataObject::count(std::string const& _key) const
{
    return m_mapSubObjects.count(_key);
}

/// Get string value
std::string const& DataObject::asString() const
{
    _assert(m_type == DataType::String, "m_type == DataType::String (DataObject::asString())");
    return m_strVal;
}

/// Get int value
int DataObject::asInt() const
{
    _assert(m_type == DataType::Integer, "m_type == DataType::Integer (DataObject::asInt())");
    return m_intVal;
}

/// Get bool value
bool DataObject::asBool() const
{
    _assert(m_type == DataType::Bool, "m_type == DataType::Bool (DataObject::asBool())");
    return m_boolVal;
}

/// Set position in vector of the subobject with _key
void DataObject::setKeyPos(std::string const& _key, size_t _pos)
{
    _assert(_pos < m_mapSubObjects.size(), "_pos < m_mapSubObjects.size()");
    _assert(count(_key), "count(_key) _key = " + _key + " (DataObject::setKeyPos)");
    _assert(!_key.empty(), "!_key.empty() (DataObject::setKeyPos)");

    DataObjectInfo& info = m_mapSubObjects.at(_key);
    info.pos = _pos;
}


/// replace this object with _value
void DataObject::replace(DataObject const& _value)
{
    m_strKey = _value.getKey();
    clearSubobjects();
    switch (_value.type())
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
    case DataType::Array:
        m_arraySubObjects = _value.getArraySubObjects();
    case DataType::Object:
        m_mapSubObjects = _value.getMapSubObjects();
    default:
        break;
    }

    m_type = _value.type();
    m_allowOverwrite = _value.isOverwritable();
    setAutosort(_value.isAutosort());
}

DataObject const& DataObject::atKey(std::string const& _key) const
{
    _assert(m_mapSubObjects.count(_key), "count(_key) _key=" + _key + " (DataObject::at)");
    return m_mapSubObjects.at(_key).obj;
}

DataObject& DataObject::atKeyUnsafe(std::string const& _key)
{
    _assert(m_mapSubObjects.count(_key), "count(_key) _key=" + _key + " (DataObject::at)");
    return m_mapSubObjects.at(_key).obj;
}

DataObject const& DataObject::at(size_t _pos) const
{
    if (m_type == DataType::Array)
    {
        _assert((size_t)_pos < m_arraySubObjects.size(), "DataObject::at(int) out of range!");
        return m_arraySubObjects[_pos];
    }
    _assert(m_type == DataType::Object, "DataObject::at(size_t _pos) must be of type Object!");
    auto const& it = std::find(m_mapSubObjects.begin(), m_mapSubObjects.end(),
                               [&_pos](auto const& x) { return x.second.pos == _pos; });
    if (it != m_mapSubObjects.end())
        return it->second.obj;
    _assert(false, "DataObject::at(size_t _pos) element in map not found!");
    return m_arraySubObjects.at(0);
}

DataObject& DataObject::atUnsafe(size_t _pos)
{
    if (m_type == DataType::Array)
    {
        _assert((size_t)_pos < m_arraySubObjects.size(), "DataObject::atUnsafe(int) out of range!");
        return m_arraySubObjects[_pos];
    }
    _assert(m_type == DataType::Object, "DataObject::atUnsafe(size_t _pos) must be of type Object!");
    auto const& it = std::find(m_mapSubObjects.begin(), m_mapSubObjects.end(),
                               [&_pos](auto const& x) { return x.second.pos == _pos; });
    if (it != m_mapSubObjects.end())
        return it->second.obj;
    _assert(false, "DataObject::atUnsafe(size_t _pos) element in map not found!");
    return m_arraySubObjects.at(0);
}

void DataObject::addArrayObject(DataObject const& _obj)
{
    _assert(m_type == DataType::Null || m_type == DataType::Array,
        "m_type == DataType::Null || m_type == DataType::Array (DataObject::addArrayObject)");
    m_type = DataType::Array;
    m_arraySubObjects.push_back(_obj);
    m_arraySubObjects.at(m_arraySubObjects.size() - 1).setAutosort(m_autosort);
}

void DataObject::renameKey(std::string const& _currentKey, std::string const& _newKey)
{
    _assert(m_type == DataType::Object, "DataObject::renameKey must be map object!");
    if (m_strKey == _currentKey)
        m_strKey = _newKey;
    _assert(m_mapSubObjects.count(_currentKey), "DataObject::renameKey key not found!");
    m_mapSubObjects.at(_currentKey).obj.setKey(_newKey);
    m_mapSubObjects[_newKey] = std::move(m_mapSubObjects.at(_currentKey));
    m_mapSubObjects.erase(_currentKey);
}

/// vector<element> erase method with `replace()` function
void DataObject::removeKey(std::string const& _key)
{
    _assert(type() == DataType::Object, "type() == DataType::Object");
    _assert(m_mapSubObjects.count(_key), "DataObject::removeKey key" + _key + " not found!");
    m_mapSubObjects.erase(_key);
}

void DataObject::clear(DataType _type)
{
    m_intVal = 0;
    m_strKey = "";
    m_strVal = "";
    clearSubobjects();
    m_type = _type;
}

void DataObject::setVerifier(void (*f)(DataObject&))
{
    m_verifier = f;
    m_verifier(*this);
}

void DataObject::performModifier(void (*f)(DataObject&), std::set<string> const& _exceptionKeys)
{
    if (m_type == DataType::Array)
        for (auto& el : m_arraySubObjects)
            el.performModifier(f, _exceptionKeys);
    else if (m_type == DataType::Object)
        for (auto& el : m_mapSubObjects)
            el.second.obj.performModifier(f, _exceptionKeys);
    if (!_exceptionKeys.count(getKey()))
        f(*this);
}

void DataObject::performVerifier(void (*f)(DataObject const&)) const
{
    if (m_type == DataType::Array)
        for (auto const& el : m_arraySubObjects)
            el.performVerifier(f);
    else if (m_type == DataType::Object)
            for (auto const& el : m_mapSubObjects)
                el.second.obj.performVerifier(f);
    f(*this);
}

std::string DataObject::asJsonNoFirstKey() const
{
    return asJson(0, true, true);
}

std::string DataObject::asJson(int level, bool pretty, bool nokey) const
{
    std::ostringstream out;
    auto printLevel = [level, pretty, &out]() -> void {
        if (pretty)
            for (int i = 0; i < level * 4; i++)
                out << " ";
    };

    auto printElements = [this, &out, level, pretty]() -> void {
        if (m_type == DataType::Array)
        {
            for (std::vector<DataObject>::const_iterator it = this->m_arraySubObjects.begin();
                 it < this->m_arraySubObjects.end(); it++)
            {
                out << (*it).asJson(level + 1, pretty);
                if (it + 1 != this->m_arraySubObjects.end())
                    out << ",";
                if (pretty)
                    out << std::endl;
            }
        }
        else if (m_type == DataType::Object)
        {
            for (size_t pos = 0; pos < m_mapSubObjects.size(); pos++)
            {
                auto it = std::find(m_mapSubObjects.begin(), m_mapSubObjects.end(), [&pos](auto const& x){
                    return (x.second.pos == pos);
                });
                _assert(it != m_mapSubObjects.end(), "DataObject::asJson map pos element missing!");
                out << (*it).second.obj.asJson(level + 1, pretty);
                auto it2 = std::find(m_mapSubObjects.begin(), m_mapSubObjects.end(), [&pos](auto const& x){
                    return (x.second.pos == pos + 1);
                });
                if (it2 != this->m_mapSubObjects.end())
                    out << ",";
                if (pretty)
                    out << std::endl;
            }
        }
    };

    string buffer;
    switch (m_type)
    {
    case DataType::Null:
        printLevel();
        if (!m_strKey.empty() && !nokey)
        {
            if (pretty)
                out << "\"" << m_strKey << "\" : ";
            else
                out << "\"" << m_strKey << "\":";
        }
        //out << "\"" << "null" << "\"";
        out << "{}";
        break;
    case DataType::Object:
        if (!m_strKey.empty() && !nokey)
        {
            printLevel();
            if (pretty)
                out << "\"" << m_strKey << "\" : {" << std::endl;
            else
                out << "\"" << m_strKey << "\":{";
        }
        else
        {
            printLevel();
            out << "{";
            if (pretty)
                out << std::endl;
        }
        printElements();
        printLevel();
        out << "}";
        break;
    case DataType::Array:
        if (!m_strKey.empty() && !nokey)
        {
            printLevel();
            if (pretty)
                out << "\"" << m_strKey << "\" : [" << std::endl;
            else
                out << "\"" << m_strKey << "\":[";
        }
        else
        {
            printLevel();
            out << "[";
            if (pretty)
                out << std::endl;
        }
        printElements();
        printLevel();
        out << "]";
        break;
    case DataType::String:
        printLevel();
        if (pretty)
        {
            if (!m_strKey.empty() && !nokey)
                out << "\"" << m_strKey << "\" : ";
        }
        else
        {
            if (!m_strKey.empty() && !nokey)
                out << "\"" << m_strKey << "\":";
        }

        //  threat special chars
        for (auto const& ch: m_strVal)
        {
            if (ch == 10)
                buffer += "\\n";
            else if (ch == 9)
                buffer += "\\t";
            else
                buffer += ch;
        }
        out << "\"" << buffer << "\"";
        break;
    case DataType::Integer:
        printLevel();
        if (!m_strKey.empty() && !nokey)
        {
            if (pretty)
                out << "\"" << m_strKey << "\" : ";
            else
                out << "\"" << m_strKey << "\":";
        }
        out << m_intVal;
        break;
    case DataType::Bool:
        printLevel();
        if (!m_strKey.empty() && !nokey)
        {
            if (pretty)
                out << "\"" << m_strKey << "\" : ";
            else
                out << "\"" << m_strKey << "\":";
        }
        if (m_boolVal)
            out << "true";
        else
            out << "false";
        break;
    default:
        out << "unknown " << dataTypeAsString(m_type) << std::endl;
        break;
    }
    return out.str();
}

std::string DataObject::dataTypeAsString(DataType _type)
{
    switch (_type)
    {
    case String:
        return "string";
    case Integer:
        return "int";
    case Array:
        return "array";
    case Bool:
        return "bool";
    case Object:
        return "object";
    case Null:
        return "null";
    default:
        break;
    }
    return "";
}

size_t dataobject::findOrderedKeyPosition(string const& _key, vector<DataObject> const& _objects)
{
    if (_objects.size() == 0)
        return 0;
    size_t step = _objects.size() / 2;
    size_t guess = step;
    while (step > 0)
    {
        step = step / 2;
        if (_objects.at(guess).getKey() > _key)
            guess -= std::max(step, (size_t)1);
        else
            guess += std::max(step, (size_t)1);
    }
    if (guess == _objects.size())
        return guess;
    guess = max(0, (int)guess - 5);
    while (guess < _objects.size() && _objects.at(guess).getKey() <= _key)
        guess++;
    return guess;
}

DataObject& DataObject::_addSubObject(DataObject const& _obj, string const& _keyOverwrite)
{
    if (m_type == DataType::Null)
        m_type = DataType::Object;

    size_t pos;
    string const& key = _keyOverwrite.empty() ? _obj.getKey() : _keyOverwrite;

    if (key.empty() || !m_autosort)
    {
        m_subObjects.push_back(_obj);
        pos = m_subObjects.size() - 1;
        setSubObjectKey(pos, key);
        m_subObjects.at(pos).setOverwrite(m_allowOverwrite);
        m_subObjects.at(pos).setAutosort(m_autosort);
    }
    else
    {
        // find ordered position to insert key
        // better use it only when export as ordered json !!!
        pos = findOrderedKeyPosition(key, m_subObjects);
        if (pos == m_subObjects.size())
            m_subObjects.push_back(_obj);
        else
        {
            setOverwrite(true);
            m_subObjects.insert(m_subObjects.begin() + pos, 1, _obj);
            setOverwrite(false);
        }
        m_subObjects.at(pos).setKey(key);
        m_subObjects.at(pos).setOverwrite(true);
        m_subObjects.at(pos).setAutosort(m_autosort);
    }
    return m_subObjects.at(pos);
}

void DataObject::_assert(bool _flag, std::string const& _comment) const
{
    if (!_flag)
    {
        std::ostringstream out;
        out << "Error in DataObject: " << std::endl;
        out << " key: '" << m_strKey << "'";
        out << " type: '" << dataTypeAsString(m_type) << "'" << std::endl;
        out << " assert: " << _comment << std::endl;
        out << asJson() << std::endl;
        throw DataObjectException(out.str());
    }
}

void DataObject::setString(string const& _value)
{
    _assert(
        m_type == DataType::String || m_type == DataType::Null, "In DataObject=(string) DataObject must be string or Null!");
    m_type = DataType::String;
    m_strVal = _value;
}

void DataObject::setInt(int _value)
{
    _assert(m_type == DataType::Integer || m_type == DataType::Null, "In DataObject=(int) DataObject must be int or Null!");
    m_type = DataType::Integer;
    m_intVal = _value;
}

void DataObject::setBool(bool _value)
{
    _assert(
        m_type == DataType::Bool || m_type == DataType::Null, "In DataObject:setBool(bool) DataObject must be bool or Null!");
    m_type = DataType::Bool;
    m_boolVal = _value;
}

DataObject& DataObject::operator=(DataObject const& _value)
{
    // So not to overwrite the existing data
    // Do not replace the key. Assuming that key is set upon calling DataObject[key] =
    if (!m_allowOverwrite && !m_autosort)
        _assert(m_type == DataType::Null,
            "m_type == DataType::Null (DataObject& operator=). Overwriting dataobject that is "
            "not NULL");

    if (m_type != DataType::Null)
        replace(_value);  // overwrite value and key
    else
    {
        // keep the key "newkey" for object["newkey"] = object2;  declarations when object["newkey"] is null;
        string const currentKey = m_strKey;
        replace(_value);
        m_strKey = currentKey;
    }
    return *this;
}

bool DataObject::operator==(DataObject const& _value) const
{
    if (type() != _value.type() || getSubObjects().size() != _value.getSubObjects().size())
        return false;
    bool equal = true;
    equal = m_type == _value.type();
    equal = getKey() == _value.getKey();
    switch (m_type)
    {
    case DataType::Bool:
        equal = asBool() == _value.asBool();
        break;
    case DataType::Integer:
        equal = asInt() == _value.asInt();
        break;
    case DataType::String:
        equal = asString() == _value.asString();
        break;
    case DataType::Array:
        equal = getSubObjects().size() == _value.getSubObjects().size();
        for (size_t i = 0; i < getSubObjects().size(); i++)
        {
            equal = getSubObjects().at(i) == _value.getSubObjects().at(i);
            if (!equal)
                break;
        }
        break;
    case DataType::Object:
        //_assert(getSubObjects().size() == 1,
        //    "in DataObject::==(Object)  LType Object must have only 1 object!");
        //_assert(_value.getSubObjects().size() == 1,
        //    "in DataObject::==(Object)  RType Object must have only 1 object!");
        equal = getSubObjects().size() == _value.getSubObjects().size();
        // equal = getSubObjects().at(0) == _value.getSubObjects().at(0);
        for (size_t i = 0; i < getSubObjects().size(); i++)
        {
            equal = getSubObjects().at(i) == _value.getSubObjects().at(i);
            if (!equal)
                break;
        }
        break;
    default:
        _assert(false, "in DataObject::== unknown object type!");
        equal = false;
        break;
    }
    return equal;
}

bool DataObject::operator==(bool _value) const
{
    DataObject tmp(DataType::Bool, _value);
    return *this == tmp;
}

DataObject& DataObject::operator=(int _value)
{
    setInt(_value);
    return *this;
}

DataObject& DataObject::operator[](std::string const& _key)
{
    _assert(m_type == DataType::Null || m_type == DataType::Object,
        "m_type == DataType::Null || m_type == DataType::Object (DataObject& operator[])");
    for (auto& i : m_subObjects)
        if (i.getKey() == _key)
            return i;
    DataObject newObj(DataType::Null);
    newObj.setKey(_key);
    return _addSubObject(newObj);  // !could change the item order!
}

DataObject& DataObject::operator=(std::string const& _value)
{
    setString(_value);
    return *this;
}
