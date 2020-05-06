#include <dataObject/DataObject.h>
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
std::vector<DataObject> const& DataObject::getSubObjects() const
{
    return m_subObjects;
}

/// Get ref vector of subobjects
std::vector<DataObject>& DataObject::getSubObjectsUnsafe()
{
    return m_subObjects;
}

/// Add new subobject
DataObject& DataObject::addSubObject(DataObject const& _obj)
{
    return _addSubObject(_obj);
}

/// Add new subobject and set it's key
DataObject& DataObject::addSubObject(std::string const& _key, DataObject const& _obj)
{
    return _addSubObject(_obj, _key);
}

/// Set key for subobject _index
void DataObject::setSubObjectKey(size_t _index, std::string const& _key)
{
    _assert(_index < m_subObjects.size(), "_index < m_subObjects.size() (DataObject::setSubObjectKey)");
    if (m_subObjects.size() > _index)
        m_subObjects.at(_index).setKey(_key);
}

/// look if there is a subobject with _key
bool DataObject::count(std::string const& _key) const
{
    for (auto const& i : m_subObjects)
        if (i.getKey() == _key)
            return true;
    return false;
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
    _assert(_pos < m_subObjects.size(), "_pos < m_subObjects.size()");
    _assert(count(_key), "count(_key) _key = " + _key + " (DataObject::setKeyPos)");
    _assert(!_key.empty(), "!_key.empty() (DataObject::setKeyPos)");

    size_t elementPos = 0;
    for (size_t i = 0; i < m_subObjects.size(); i++)
        if (m_subObjects.at(i).getKey() == _key)
        {
            if (i == _pos)
                return;  // item already at _pos;
            else
            {
                elementPos = i;
                break;
            }
        }

    setOverwrite(true);
    DataObject data = m_subObjects.at(elementPos);
    m_subObjects.erase(m_subObjects.begin() + elementPos);
    m_subObjects.insert(m_subObjects.begin() + _pos, 1, data);
    setOverwrite(false);
}


/// replace this object with _value
void DataObject::replace(DataObject const& _value)
{
    m_strKey = _value.getKey();
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
    default:
        break;
    }

    m_type = _value.type();
    m_subObjects.clear();
    m_subObjects = _value.getSubObjects();
    m_allowOverwrite = _value.isOverwritable();
    setAutosort(_value.isAutosort());
}

DataObject const& DataObject::atKey(std::string const& _key) const
{
    _assert(count(_key), "count(_key) _key=" + _key + " (DataObject::at)");
    for (auto const& i : m_subObjects)
        if (i.getKey() == _key)
            return i;
    _assert(false, "item not found! (DataObject::at)");
    return m_subObjects.at(0);
}

DataObject& DataObject::atKeyUnsafe(std::string const& _key)
{
    _assert(count(_key), "count(_key) _key=" + _key + " (DataObject::at)");
    for (auto& i : m_subObjects)
        if (i.getKey() == _key)
            return i;
    _assert(false, "item not found! (DataObject::at)");
    return m_subObjects.at(0);
}

DataObject const& DataObject::at(size_t _pos) const
{
    _assert((size_t)_pos < m_subObjects.size(), "DataObject::at(int) out of range!");
    return m_subObjects[_pos];
}

DataObject& DataObject::atUnsafe(size_t _pos)
{
    _assert((size_t)_pos < m_subObjects.size(), "DataObject::atUnsafe(int) out of range!");
    return m_subObjects[_pos];
}

DataObject const& DataObject::atLastElement() const
{
    _assert(m_subObjects.size() > 0, "atLastElement()");
    return m_subObjects.at(m_subObjects.size() - 1);
}

DataObject& DataObject::atLastElementUnsafe()
{
    _assert(m_subObjects.size() > 0, "atLastElementUnsafe()");
    return m_subObjects.at(m_subObjects.size() - 1);
}

void DataObject::addArrayObject(DataObject const& _obj)
{
    _assert(m_type == DataType::Null || m_type == DataType::Array,
        "m_type == DataType::Null || m_type == DataType::Array (DataObject::addArrayObject)");
    m_type = DataType::Array;
    m_subObjects.push_back(_obj);
    m_subObjects.at(m_subObjects.size() - 1).setAutosort(m_autosort);
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
    for (std::vector<DataObject>::const_iterator it = m_subObjects.begin();
         it != m_subObjects.end(); it++)
    {
        if ((*it).getKey() == _key)
        {
            setOverwrite(true);
            m_subObjects.erase(it);
            setOverwrite(false);
            break;
        }
    }

    /*
    bool startReplace = false;
    for (std::vector<DataObject>::iterator it = m_subObjects.begin(); it != m_subObjects.end();
         it++)
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
    }*/
}

void DataObject::clear(DataType _type)
{
    m_intVal = 0;
    m_strKey = "";
    m_strVal = "";
    m_subObjects.clear();
    m_type = _type;
}

void DataObject::performModifier(void (*f)(DataObject&))
{
    for (auto& el : m_subObjects)
        el.performModifier(f);
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
        for (std::vector<DataObject>::const_iterator it = this->m_subObjects.begin();
             it < this->m_subObjects.end(); it++)
        {
            out << (*it).asJson(level + 1, pretty);
            if (it + 1 != this->m_subObjects.end())
                out << ",";
            if (pretty)
                out << std::endl;
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
        // Make it an exception!
        std::cerr << "Error in DataObject: " << std::endl;
        std::cerr << " key: '" << m_strKey << "'";
        std::cerr << " type: '" << dataTypeAsString(m_type) << "'" << std::endl;
        std::cerr << " assert: " << _comment << std::endl;
        std::cerr << asJson() << std::endl;
        assert(_flag);
    }
}
