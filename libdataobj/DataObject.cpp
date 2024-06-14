#include "DataObject.h"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace dataobject;
using namespace std;

namespace {
    std::string doubleToStringWithPrecision(double value, int precision)
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(precision) << value;
        return out.str();
    };
}

/// Default dataobject is null
DataObject::DataObject() {}

/// Define dataobject of _type, pass the value later (will check the value and _type)
DataObject::DataObject(DataType _type)
{
    if (_type == DataType::Object || _type == DataType::Array)
        _initArray(_type);
    else if (_type == DataType::Null)
        m_value = DataNull();
    else if (_type == DataType::String)
        m_value = string();
    else if (_type == DataType::Integer)
        m_value = 0;
    else if (_type == DataType::Double)
        m_value = (double)0.0;
    else if (_type == DataType::Bool)
        m_value = false;
}

/// Define dataobject of string
DataObject::DataObject(std::string&& _str) : m_value(std::move(_str)) {}
DataObject::DataObject(std::string const& _str) : m_value(_str)
{}

/// Define dataobject[_key] = string
DataObject::DataObject(std::string&& _key, std::string&& _str)
  : m_strKey(std::move(_key)), m_value(std::move(_str))
{}

DataObject::DataObject(std::string const& _key, std::string const& _str)
  : m_strKey(_key), m_value(_str)
{}

DataObject::DataObject(std::string&& _key, int _val)
  : m_strKey(std::move(_key))
{
    setInt(_val);
}

/// Define dataobject of int
DataObject::DataObject(int _int)
{
    setInt(_int);
}

DataObject::DataObject(DataType type, int _int)
{
    _assert(type == DataType::Integer, "DataObject(DataType type, int _int) is not initialized as Integer");
    setInt(_int);
}

DataObject::DataObject(DataType type, double _double)
{
    _assert(type == DataType::Double, "DataObject(DataType type, double _double) is not initialized as Double");
    setDouble(_double);
}

/// Define dataobject of bool
DataObject::DataObject(DataType type, SafeBool _bool)
{
    string stype;
    switch(type)
    {
    case DataType::Integer: stype = "INTEGER"; break;
    case DataType::Array: stype = "Array"; break;
    case DataType::Bool: stype = "Bool"; break;
    case DataType::Double: stype = "Double"; break;
    case DataType::NotInitialized: stype = "NotInit"; break;
    case DataType::Null: stype = "Null"; break;
    case DataType::String: stype = "String"; break;
    case DataType::Object: stype = "Object"; break;
    }
    _assert(type == DataType::Bool, "DataObject(DataType type, bool _bool) is not initialized as Bool, requested " + stype);
    setBool(_bool);
}

/// Get dataobject type
DataType DataObject::type() const
{
    if (_isNotInit())
        return DataType::NotInitialized;
    return (DataType)m_value.index();
}

constexpr bool DataObject::_isNotInit() const
{
    return std::holds_alternative<std::monostate>(m_value);
}

/// Set key of the dataobject
void DataObject::setKey(std::string&& _key) { m_strKey = std::move(_key); }
void DataObject::setKey(std::string const& _key) { m_strKey = _key; }

/// Get key of the dataobject
std::string const& DataObject::getKey() const { return m_strKey; }
std::string& DataObject::getKeyUnsafe() { return m_strKey; }

/// Get vector of subobjects
std::vector<spDataObject> const& DataObject::getSubObjects() const
{
    static std::vector<spDataObject> emptyVector;
    if (type() == DataType::Object)
        return std::get<DataObjecto>(m_value).first;
    if (type() == DataType::Array)
        return std::get<0>(std::get<DataArray>(m_value));
    return emptyVector;
}

/// Get map of keys to subobjects
std::map<string, spDataObject> const& DataObject::getSubObjectKeys() const
{
    static std::map<std::string, spDataObject> emptyMap;
    if (type() == DataType::Object)
        return std::get<DataObjecto>(m_value).second;
    if (type() == DataType::Array)
        return std::get<1>(std::get<DataArray>(m_value));
    return emptyMap;
}

std::map<string, spDataObject>& DataObject::_getSubObjectKeysUnsafe()
{
    static std::map<std::string, spDataObject> emptyMap;
    if (type() == DataType::Object)
        return std::get<DataObjecto>(m_value).second;
    if (type() == DataType::Array)
        return std::get<1>(std::get<DataArray>(m_value));
    return emptyMap; // Dangerous, Not Thread Safe, should never be here
}

/// Get ref vector of subobjects
std::vector<spDataObject>& DataObject::getSubObjectsUnsafe()
{
    static std::vector<spDataObject> emptyVector;
    if (type() == DataType::Object)
        return std::get<DataObjecto>(m_value).first;
    if (type() == DataType::Array)
        return std::get<0>(std::get<DataArray>(m_value));
    return emptyVector; // Dangerous, Not Thread Safe
}

/// Add new subobject
DataObject& DataObject::addSubObject(spDataObject const& _obj)
{
    return _addSubObject(_obj);
}

/// Add new subobject and set it's key
DataObject& DataObject::addSubObject(std::string&& _key, spDataObject const& _obj)
{
    return _addSubObject(_obj, std::forward<string&&>(_key));
}
DataObject& DataObject::addSubObject(std::string const& _key, spDataObject const& _obj)
{
    return _addSubObject(_obj, string(_key));
}

/// Set key for subobject _index
void DataObject::setSubObjectKey(size_t _index, std::string&& _key)
{
    static const string c_errorAssert = "DataObject::setSubObjectKey is not array";
    _assert(isArray(), c_errorAssert);
    auto& subObjects = getSubObjectsUnsafe();
    static const string c_errorAssert2 = "_index < m_subObjects.size() (DataObject::setSubObjectKey)";
    _assert(_index < subObjects.size(), c_errorAssert2);
    if (subObjects.size() > _index)
        subObjects.at(_index).getContent().setKey(std::forward<string&&>(_key));
}


/// look if there is a subobject with _key
bool DataObject::count(std::string const& _key) const
{
    auto const& subObjectKeys = getSubObjectKeys();
    return subObjectKeys.count(_key);
}

/// Get string value
std::string const& DataObject::asString() const
{
    static const string c_errorAssert = "m_type == DataType::String (DataObject::asString())";
    _assert(type() == DataType::String, c_errorAssert);
    return std::get<std::string>(m_value);
}
std::string& DataObject::asStringUnsafe()
{
    static const string c_errorAssert = "m_type == DataType::String (DataObject::asStringUnsafe())";
    _assert(type() == DataType::String, c_errorAssert);
    return std::get<std::string>(m_value);
}

std::string const DataObject::asStringAnyway() const
{
    switch(type())
    {
    case DataType::String: return asString();
    case DataType::Integer: return to_string(asInt());
    case DataType::Double: return doubleToStringWithPrecision(asDouble(), 4);
    case DataType::Bool: return asBool() ? "true" : "false";
    case DataType::Object: return "Object";
    case DataType::Array: return "Array";
    case DataType::Null: return "Null";
    case DataType::NotInitialized: return "Not initialized";
    default: return "N/A";
    }
}

/// Get int value
int DataObject::asInt() const
{
    static const string c_errorAssert = "m_type == DataType::Integer (DataObject::asInt())";
    _assert(type() == DataType::Integer, c_errorAssert);
    return std::get<int>(m_value);
}

/// Get int value
double DataObject::asDouble() const
{
    static const string c_errorAssert = "m_type == DataType::Double (DataObject::asDouble())";
    _assert(type() == DataType::Double, c_errorAssert);
    return std::get<double>(m_value);
}

/// Get bool value
bool DataObject::asBool() const
{
    static const string c_errorAssert = "m_type == DataType::Bool (DataObject::asBool())";
    _assert(type() == DataType::Bool, c_errorAssert);
    return std::get<bool>(m_value);
}

/// Set position in vector of the subobject with _key
void DataObject::setKeyPos(std::string const& _key, size_t _pos)
{
    static const string c_errorAssert = "count(_key) (DataObject::setKeyPos)";
    _assert(count(_key), c_errorAssert);
    static const string c_errorAssert2 = "!_key.empty() (DataObject::setKeyPos)";
    _assert(!_key.empty(), c_errorAssert2);

    size_t elementPos = 0;
    auto& subObjects = getSubObjectsUnsafe();
    for (size_t i = 0; i < subObjects.size(); i++)
        if (subObjects.at(i)->getKey() == _key)
        {
            if (i == _pos)
                return;  // item already at _pos;
            else
            {
                elementPos = i;
                break;
            }
        }

    spDataObject const data = subObjects.at(elementPos);
    subObjects.erase(subObjects.begin() + elementPos);
    if (_pos >= subObjects.size())
        subObjects.push_back(data);
    else
        subObjects.insert(subObjects.begin() + _pos, 1, data);
}


/// replace this object with _value
void DataObject::replace(DataObject const& _value)
{
    clear();
    m_strKey = _value.getKey();
    switch (_value.type())
    {
    case DataType::String:
        m_value = _value.asString();
        break;
    case DataType::Integer:
        m_value = _value.asInt();
        break;
    case DataType::Double:
        m_value = _value.asDouble();
        break;
    case DataType::Bool:
        m_value = _value.asBool();
        break;
    case DataType::Object:
        m_value = std::pair{_value.getSubObjects(), _value.getSubObjectKeys()};
        break;
    case DataType::Array:
        m_value = std::tuple{_value.getSubObjects(), _value.getSubObjectKeys()};
        break;
    case DataType::Null:
        m_value = DataNull();
        break;
    default:
        break;
    }

    setAutosort(_value.isAutosort());
}

DataObjectK& DataObjectK::operator=(spDataObject const& _value)
{
    if (m_data.count(m_key))
    {
        m_data.removeKey(m_key);
        if (_value->getKey().empty())
            throw DataObjectException("DataObjectK::operator=(spDataObject const& _value)  _value without key, but key required!");
        m_data.addSubObject(string(_value->getKey()), _value);
    }
    else
        m_data.addSubObject(string(m_key), _value);
    return *this;
}

spDataObject& DataObject::atKeyPointerUnsafe(std::string const& _key)
{
    auto& subObjectKeys = _getSubObjectKeysUnsafe();
    if (subObjectKeys.count(_key))
        return subObjectKeys.at(_key);
    _assert(false, "count(_key) _key=" + _key + " (DataObject::atKeyPointerUnsafe)");
    return subObjectKeys.at(0);
}

DataObjectK DataObject::atKeyPointer(std::string const& _key)
{
    return DataObjectK(_key, *this);
}

DataObject const& DataObject::atKey(std::string const& _key) const
{
    auto const& subObjectKeys = getSubObjectKeys();
    if (subObjectKeys.count(_key))
        return subObjectKeys.at(_key).getCContent();

    _assert(false, "count(_key) _key=" + _key + " (DataObject::atKey)");
    auto const& subObjects = getSubObjects();
    return subObjects.at(0).getCContent();
}

DataObject& DataObject::atKeyUnsafe(std::string const& _key)
{
    auto& subObjectKeys = _getSubObjectKeysUnsafe();
    if (subObjectKeys.count(_key))
        return subObjectKeys.at(_key).getContent();
    _assert(false, "count(_key) _key=" + _key + " (DataObject::atKeyUnsafe)");
    auto& subObjects = getSubObjectsUnsafe();
    return subObjects.at(0).getContent();
}

DataObject const& DataObject::at(size_t _pos) const
{
    auto const& subObjects = getSubObjects();
    _assert((size_t)_pos < subObjects.size(), "DataObject::at(int) out of range!");
    return subObjects[_pos];
}

DataObject& DataObject::atUnsafe(size_t _pos)
{
    auto& subObjects = getSubObjectsUnsafe();
    _assert((size_t)_pos < subObjects.size(), "DataObject::atUnsafe(int) out of range!");
    return subObjects[_pos].getContent();
}

DataObject const& DataObject::atLastElement() const
{
    auto const& subObjects = getSubObjects();
    static const string c_assert = "atLastElement()";
    _assert(subObjects.size() > 0, c_assert);
    return subObjects.at(subObjects.size() - 1);
}

DataObject& DataObject::atLastElementUnsafe()
{
    auto& subObjects = getSubObjectsUnsafe();
    static const string c_assert = "atLastElementUnsafe()";
    _assert(subObjects.size() > 0, c_assert);
    return subObjects.at(subObjects.size() - 1).getContent();
}

void DataObject::addArrayObject(spDataObject const& _obj)
{
    static const string c_assert = "m_type == DataType::NotInitialized || m_type == DataType::Array (DataObject::addArrayObject)";
    bool const notInit = type() == DataType::NotInitialized;
    _assert(notInit || type() == DataType::Array, c_assert);
    if (notInit)
        _initArray(DataType::Array);
    auto& subObjects = getSubObjectsUnsafe();
    subObjects.push_back(_obj);
    subObjects.at(subObjects.size() - 1).getContent().setAutosort(m_autosort);
}

void DataObject::renameKey(std::string const& _currentKey, std::string&& _newKey)
{
    if (m_strKey == _currentKey)
        m_strKey = _newKey;

    if (isArray())
    {
        auto& subObjectKeys = _getSubObjectKeysUnsafe();
        if (subObjectKeys.count(_currentKey))
        {
            spDataObject data = subObjectKeys.at(_currentKey);
            subObjectKeys.erase(_currentKey);
            data.getContent().setKey(_newKey);
            subObjectKeys.emplace(_newKey, data);
        }
    }
}

/// vector<element> erase method with `replace()` function
void DataObject::removeKey(std::string const& _key)
{
    static const string c_assert = "type() == DataType::Object";
    _assert(type() == DataType::Object, c_assert);
    auto& subObjects = getSubObjectsUnsafe();
    auto& subObjectKeys = _getSubObjectKeysUnsafe();

    subObjects.erase(std::remove_if(subObjects.begin(), subObjects.end(),
                         [&_key](const spDataObject& _obj) {
                             return _obj->getKey() == _key;
                         }), subObjects.end());
    subObjectKeys.erase(_key);
}

void DataObject::clear(DataType _type)
{
    m_strKey = "";
    if (isArray())
        clearSubobjects();
    DataVariant emptyVariant;
    std::swap(m_value, emptyVariant);
    if (_type == DataType::Object || _type == DataType::Array)
        _initArray(_type);
}

void DataObject::performModifier(void (*f)(DataObject&), ModifierOption _opt, std::set<string> const& _exceptionKeys)
{
    if (!_exceptionKeys.count(getKey()))
    {
        f(*this);
        if (_opt == ModifierOption::RECURSIVE && isArray())
        {
            auto& subObjects = getSubObjectsUnsafe();
            for (auto& el : subObjects)
                el.getContent().performModifier(f, _opt, _exceptionKeys);
        }
    }
}

bool DataObject::performSearch(bool (*f)(DataObject const&)) const
{
    if (isArray())
    {
        auto const& subObjects = getSubObjects();
        for (auto const& el : subObjects)
        {
            if (el->performSearch(f))
                return true;
        }
    }
    return f(*this);
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
        if (this->isArray())
        {
            auto const& subObjects = getSubObjects();
            for (std::vector<spDataObject>::const_iterator it = subObjects.begin();
                 it < subObjects.end(); it++)
            {
                if ((*it).isEmpty())
                    out << "NaN";
                else
                    out << (*it)->asJson(level + 1, pretty);
                if (it + 1 != subObjects.end())
                    out << ",";
                if (pretty)
                    out << std::endl;
            }
        }
    };

    string buffer;
    switch (type())
    {
    case DataType::NotInitialized:
        printLevel();
        if (!m_strKey.empty() && !nokey)
        {
            if (pretty)
                out << "\"" << m_strKey << "\" : ";
            else
                out << "\"" << m_strKey << "\":";
        }
        out << "notinit";
        break;
    case DataType::Null:
        printLevel();
        if (!m_strKey.empty() && !nokey)
        {
            if (pretty)
                out << "\"" << m_strKey << "\" : ";
            else
                out << "\"" << m_strKey << "\":";
        }
        out << "null";
        //out << "{}";  // why???
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
    {
        printLevel();
        if (!m_strKey.empty() && !nokey)
        {
            if(pretty)
                out << "\"" << m_strKey << "\" : ";
            else
                out << "\"" << m_strKey << "\":";
        }

        //  threat special chars
        char ch_before = 0;
        for (auto const& ch: asString())
        {
            if (ch == 10)
                buffer += "\\n";
            else if (ch == 9)
                buffer += "\\t";
            else if (ch == '"' && ch_before != '\\')
            {
                buffer += "\\";
                buffer += "\"";
            }
            else
                buffer += ch;
            ch_before = ch;
        }
        out << "\"" << buffer << "\"";
        break;
    }
    case DataType::Double:
        printLevel();
        if (!m_strKey.empty() && !nokey)
        {
            if (pretty)
                out << "\"" << m_strKey << "\" : ";
            else
                out << "\"" << m_strKey << "\":";
        }
        out << doubleToStringWithPrecision(std::get<double>(m_value), 4);
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
        out << std::get<int>(m_value);
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
        if (std::get<bool>(m_value))
            out << "true";
        else
            out << "false";
        break;
    default:
        out << "unknown " << dataTypeAsString(type()) << std::endl;
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
    case Double:
        return "double";
    case Array:
        return "array";
    case Bool:
        return "bool";
    case Object:
        return "object";
    case NotInitialized:
        return "notinit";
    case Null:
        return "null";
    default:
        break;
    }
    return "";
}

size_t dataobject::findOrderedKeyPosition(string const& _key, vector<spDataObject> const& _objects)
{
    if (_objects.size() == 0)
        return 0;

    size_t m = 0;
    int L = 0;
    int R = _objects.size() - 1;
    while (L <= R)
    {
        m = floor((L + R) / 2);
        if (_objects.at(m)->getKey() < _key)
            L = m + 1;
        else if (_objects.at(m)->getKey() > _key)
            R = m - 1;
        else
            return m;
    }

    if (_objects.at(m)->getKey() > _key)
        return m;
    else
        return m + 1;
}

DataObject& DataObject::_addSubObject(spDataObject const& _obj, string&& _keyOverwrite)
{
    if (type() == DataType::NotInitialized)
        _initArray(DataType::Object);

    size_t pos;
    string const key = _keyOverwrite.empty() ? _obj->getKey() : _keyOverwrite;
    if (key.empty() || !m_autosort)
    {
        auto& subObjects = getSubObjectsUnsafe();
        subObjects.push_back(_obj);
        pos = subObjects.size() - 1;
        if (!_keyOverwrite.empty())
            setSubObjectKey(pos, std::forward<string&&>(_keyOverwrite));
        else
            setSubObjectKey(pos, string(_obj->getKey()));
        subObjects.at(pos).getContent().setAutosort(m_autosort);
    }
    else
    {
        // find ordered position to insert key
        // better use it only when export as ordered json !!!
        auto& subObjects = getSubObjectsUnsafe();
        pos = findOrderedKeyPosition(key, subObjects);
        if (pos == subObjects.size())
            subObjects.push_back(_obj);
        else
            subObjects.insert(subObjects.begin() + pos, 1, _obj);

        if (!_keyOverwrite.empty())
            subObjects.at(pos).getContent().setKey(std::forward<string&&>(_keyOverwrite));
        else
            subObjects.at(pos).getContent().setKey(string(_obj->getKey()));
        subObjects.at(pos).getContent().setAutosort(m_autosort);
    }
    auto& subObjects = getSubObjectsUnsafe();
    auto& subObjectKeys = _getSubObjectKeysUnsafe();
    if (!key.empty())
        subObjectKeys.emplace(key, subObjects.at(pos));
    return subObjects.at(pos).getContent();
}

void DataObject::_assert(bool _flag, std::string const& _comment) const
{
    if (!_flag)
    {
        std::ostringstream out;
        out << "Error in DataObject: " << std::endl;
        out << " key: '" << m_strKey << "'";
        out << " type: '" << dataTypeAsString(type()) << "'" << std::endl;
        out << " assert: " << _comment << std::endl;
        out << asJson() << std::endl;
        throw DataObjectException(out.str());
    }
}

void DataObject::setString(string&& _value)
{
    static const string c_assert = "In DataObject=(string) DataObject must be string or NotInitialized!";
    _assert(type() == DataType::String || type() == DataType::NotInitialized, c_assert);
    m_value = std::move(_value);
}

void DataObject::setInt(int _value)
{
    static const string c_assert = "In DataObject=(int) DataObject must be int or NotInitialized!";
    _assert(type() == DataType::Integer || type() == DataType::NotInitialized, c_assert);
    m_value = _value;
}

void DataObject::setDouble(double _value)
{
    static const string c_assert = "In DataObject=(double) DataObject must be double or NotInitialized!";
    _assert(type() == DataType::Double || type() == DataType::NotInitialized, c_assert);
    m_value = _value;
}

void DataObject::setBool(bool _value)
{
    static const string c_assert = "In DataObject:setBool(bool) DataObject must be bool or NotInitialized!";
    _assert(type() == DataType::Bool || type() == DataType::NotInitialized, c_assert);
    m_value = (bool)_value;
}

void DataObject::copyFrom(DataObject const& _other)
{
    clear();
    if (!_other.getKey().empty())
        m_strKey = _other.getKey();

    switch (_other.type())
    {
    case String: m_value = _other.asString(); break;
    case Integer: m_value = _other.asInt(); break;
    case Double: m_value = _other.asDouble(); break;
    case Bool: m_value = _other.asBool(); break;
    case Array:
        _initArray(DataType::Array);
        for (auto const& el : _other.getSubObjects())
        {
            spDataObject copy = el->copy();
            addArrayObject(copy);
        }
        break;
    case Object:
        _initArray(DataType::Object);
        for (auto const& el : _other.getSubObjects())
        {
            spDataObject copy = el->copy();
            addSubObject(copy);
        }
        break;
    case Null: m_value = DataNull(); break;
    case NotInitialized: break;
    }
}

spDataObject DataObject::copy() const
{
    spDataObject c = sDataObject(type());
    if (!m_strKey.empty())
        (*c).setKey(string(m_strKey));
    switch(type())
    {
    case String: (*c).setString(string(asString())); break;
    case Integer: (*c).setInt(asInt()); break;
    case Double: (*c).setDouble(asDouble()); break;
    case Bool: (*c).setBool(asBool()); break;
    case Array:
        for (size_t i = 0; i < getSubObjects().size(); i++)
        {
            spDataObject copy = getSubObjects().at(i)->copy();
            (*c).addArrayObject(copy);
        }
    break;
    case Object:
        for (size_t i = 0; i < getSubObjects().size(); i++)
        {
            spDataObject copy = getSubObjects().at(i)->copy();
            (*c).addSubObject(copy);
        }
    break;
    case Null:
        break;
    case NotInitialized: break;
    }
    return c;
}

/*DataObject& DataObject::operator=(spDataObject const& _value)
{
    m_myself = _value;

}*/

/*DataObject& DataObject::operator=(DataObject const& _value)
{
    // So not to overwrite the existing data
    // Do not replace the key. Assuming that key is set upon calling DataObject[key] =
    if (!m_allowOverwrite && !m_autosort)
        _assert(m_type == DataType::NotInitialized,
            "m_type == DataType::NotInitialized (DataObject& operator=). Overwriting dataobject that is "
            "not NotInitialized");

    if (m_type != DataType::NotInitialized)
        replace(_value);  // overwrite value and key
    else
    {
        // keep the key "newkey" for object["newkey"] = object2;  declarations when object["newkey"] is NotInitialized;
        string const currentKey = m_strKey;
        replace(_value);
        m_strKey = currentKey;
    }
    return *this;
}*/

bool DataObject::operator==(DataObject const& _value) const
{
    if (type() != _value.type() ||
        (isArray() && getSubObjects().size() != _value.getSubObjects().size()))
        return false;
    bool equal = true;
    switch (type())
    {
    case DataType::Bool:
        equal = asBool() == _value.asBool();
        break;
    case DataType::Integer:
        equal = asInt() == _value.asInt();
        break;
    case DataType::Double:
        equal = asStringAnyway() == _value.asStringAnyway();
        break;
    case DataType::String:
        equal = asString() == _value.asString();
        break;
    case DataType::Array:
        for (size_t i = 0; i < getSubObjects().size(); i++)
        {
            equal = getSubObjects().at(i).getCContent() == _value.getSubObjects().at(i).getCContent();
            if (!equal)
                break;
        }
        break;
    case DataType::Object:
        for (size_t i = 0; i < getSubObjects().size(); i++)
        {
            equal = getSubObjects().at(i).getCContent() == _value.getSubObjects().at(i).getCContent();
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

DataObject& DataObject::operator=(size_t _value)
{
    setInt(_value);
    return *this;
}

DataObject& DataObject::operator=(int _value)
{
    setInt(_value);
    return *this;
}

DataObject& DataObject::operator=(double _value)
{
    setDouble(_value);
    return *this;
}

DataObject& DataObject::operator[](std::string const& _key)
{
    static const string c_assert = "m_type == DataType::NotInitialized || m_type == DataType::Object (DataObject& operator[])";
    _assert(type() == DataType::NotInitialized || type() == DataType::Object, c_assert);

    if (isArray() && getSubObjectKeys().count(_key))
        return _getSubObjectKeysUnsafe().at(_key).getContent();

    spDataObject newObj = sDataObject(DataType::NotInitialized);
    newObj.getContent().setKey(string(_key));
    return _addSubObject(newObj);  // !could change the item order!
}

DataObject& DataObject::operator[](std::string&& _key)
{
    static const string c_assert = "m_type == DataType::NotInitialized || m_type == DataType::Object (DataObject& operator[])";
    _assert(type() == DataType::NotInitialized || type() == DataType::Object, c_assert);

    if (isArray() && getSubObjectKeys().count(_key))
        return _getSubObjectKeysUnsafe().at(_key).getContent();

    spDataObject newObj = sDataObject(DataType::NotInitialized);
    newObj.getContent().setKey(std::forward<string&&>(_key));
    return _addSubObject(newObj);  // !could change the item order!
}

DataObject& DataObject::operator=(std::string&& _value)
{
    static const string c_assert = "In DataObject=(string) DataObject must be string or NotInitialized!";
    _assert(type() == DataType::String || type() == DataType::NotInitialized, c_assert);
    m_value = std::move(_value);
    return *this;
}

DataObject& DataObject::operator=(std::string const& _value)
{
    static const string c_assert = "In DataObject=(string) DataObject must be string or NotInitialized!";
    _assert(type() == DataType::String || type() == DataType::NotInitialized, c_assert);
    m_value = _value;
    return *this;
}

spDataObjectMove dataobject::move(spDataObject& _obj)
{
    spDataObjectMove m;
    m.assignPointer(_obj);
    _obj.null();
    return m;
}

void DataObject::clearSubobjects(DataType _t)
{
    if(isArray())
    {
        getSubObjectsUnsafe().clear();
        _getSubObjectKeysUnsafe().clear();
    }
    if (_t == DataType::NotInitialized)
    {
        DataVariant emptyVariant;
        std::swap(m_value, emptyVariant);
        return;
    }
    switch (_t) {
    case DataType::Null: m_value = DataNull(); break;
    case DataType::Object: _initArray(_t); break;
    case DataType::Array: _initArray(_t); break;
    default: _assert(false); break;
    }
}

void DataObject::_initArray(DataType _t)
{
    VecSpData vec;
    MapKeyToObject map;
    if (_t == DataType::Object)
        m_value = std::make_pair(vec, map);
    else if (_t == DataType::Array)
        m_value = std::make_tuple(vec, map);
    else
        _assert(false, "_initArray got wrong DataType: " + dataTypeAsString(_t));
}
