#pragma once
#include "Exception.h"
#include "SPointer.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <variant>

namespace dataobject
{

enum DataType
{
    NotInitialized,
    Bool,
    String,
    Integer,
    Double,
    Object,
    Array,
    Null
};

class DataObjectK;
class GCP_SPointerDataObject;
typedef GCP_SPointerDataObject spDataObject;

class SafeBool
{
public:
    SafeBool(bool value) : m_value(value) {}
    SafeBool(int) = delete;
    operator bool() const { return m_value; }

private:
    bool m_value;
};

/// DataObject
/// A data sturcture to manage data from json, yml
class DataObject : public GCP_SPointerBase
{
public:
    DataObject();
    DataObject(DataObject const&) = delete;
    explicit DataObject(DataType _type);
    explicit DataObject(DataType _type, SafeBool _bool);
    explicit DataObject(DataType _type, int _bool);
    explicit DataObject(DataType _type, double _bool);

    explicit DataObject(double _int);
    explicit DataObject(int _int);
    explicit DataObject(std::string&& _str);
    explicit DataObject(std::string const& _str);

    // DataObject(key)
    DataObject(std::string&& _key, std::string&& _str);
    DataObject(std::string const& _key, std::string const& _str);
    DataObject(std::string&& _key, int _val);


    DataType type() const;
    void setKey(std::string&& _key);
    void setKey(std::string const& _key);
    std::string const& getKey() const;
    std::string& getKeyUnsafe();

    std::vector<spDataObject> const& getSubObjects() const;
    std::map<std::string, spDataObject> const& getSubObjectKeys() const;
    std::vector<spDataObject>& getSubObjectsUnsafe();

    void addArrayObject(spDataObject const& _obj);
    DataObject& addSubObject(spDataObject const& _obj);

    DataObject& addSubObject(std::string&& _key, spDataObject const& _obj);
    DataObject& addSubObject(std::string const& _key, spDataObject const& _obj);
    void setSubObjectKey(size_t _index, std::string&& _key);
    void setKeyPos(std::string const& _key, size_t _pos);

    bool count(std::string const& _key) const;

    std::string const& asString() const;
    std::string& asStringUnsafe();
    std::string const asStringAnyway() const;

    int asInt() const;
    double asDouble() const;
    bool asBool() const;

    bool operator==(bool _value) const;
    bool operator==(DataObject const& _value) const;
    DataObject& operator[](std::string const& _key);
    DataObject& operator[](std::string&& _key);
    DataObject& operator=(DataObject const& _value) = delete;
    //DataObject& operator=(spDataObject const& _value);
    spDataObject copy() const;
    void copyFrom(DataObject const& _other);
    DataObject& operator=(std::string&& _value);
    DataObject& operator=(std::string const& _value);
    DataObject& operator=(size_t _value);
    DataObject& operator=(int _value);
    DataObject& operator=(double _value);

    void setString(std::string&& _value);
    void setInt(int _value);
    void setDouble(double _value);
    void setBool(bool _value);
    void replace(DataObject const& _value);
    void renameKey(std::string const& _currentKey, std::string&& _newKey);
    void removeKey(std::string const& _key);  // vector<element> erase method with `replace()` function

    DataObject const& atKey(std::string const& _key) const;
    DataObjectK atKeyPointer(std::string const& _key);
    spDataObject& atKeyPointerUnsafe(std::string const& _key);
    DataObject& atKeyUnsafe(std::string const& _key);
    DataObject const& at(size_t _pos) const;
    DataObject& atUnsafe(size_t _pos);
    DataObject const& atLastElement() const;
    DataObject& atLastElementUnsafe();

    enum ModifierOption
    {
        RECURSIVE,
        NONRECURSIVE
    };
    void performModifier(void (*f)(DataObject&), ModifierOption _opt = ModifierOption::RECURSIVE,
        std::set<std::string> const& _exceptionKeys = {});
    bool performSearch(bool (*f)(DataObject const&)) const;

    void clear(DataType _type = DataType::NotInitialized);

    std::string asJsonNoFirstKey() const;
    std::string asJson(int level = 0, bool pretty = true, bool nokey = false) const;
    static std::string dataTypeAsString(DataType _type);

    constexpr void setAutosort(bool _sort) { m_autosort = _sort; }
    constexpr bool isAutosort() const { return m_autosort; }
    bool isArray() const { return type() == DataType::Object || type() == DataType::Array; }
    void clearSubobjects(DataType _t = DataType::NotInitialized);

private:
    DataObject& _addSubObject(spDataObject const& _obj, std::string&& _keyOverwrite = std::string());
    void _assert(bool _flag, std::string const& _comment = std::string()) const;
    void _initArray(DataType _type);
    constexpr bool _isNotInit() const;
    std::map<std::string, spDataObject>& _getSubObjectKeysUnsafe();

    std::string m_strKey;
    bool m_autosort = false;

    typedef std::vector<spDataObject> VecSpData;
    typedef std::map<std::string, spDataObject> MapKeyToObject;
    typedef std::pair<VecSpData, MapKeyToObject> DataObjecto;
    typedef std::tuple<VecSpData, MapKeyToObject> DataArray;
    struct DataNull {};
    typedef std::variant<std::monostate, bool, std::string, int, double, DataObjecto, DataArray, DataNull> DataVariant;
    DataVariant m_value;
};

// Default DataObject pointer allocator so not to type 'new DataObject()' each time
class GCP_SPointerDataObject : public GCP_SPointer<DataObject>
{
public:
    GCP_SPointerDataObject() : GCP_SPointer<DataObject>(new DataObject()){}
    GCP_SPointerDataObject(DataObject* _data) : GCP_SPointer<DataObject>(_data){}
};

// The key assigner, assign left pointer to DataObjectK's host m_data[key]
class DataObjectK
{
public:
    DataObjectK(std::string const& _key, DataObject& _container) : m_key(_key), m_data(_container) {}
    DataObjectK& operator=(spDataObject const& _value);
    DataObject& getContent() { return m_data; }
    operator spDataObject() { return m_data.atKeyPointerUnsafe(m_key); }

private:
    std::string m_key;
    DataObject& m_data;
};


template <class T>
spDataObject sDataObject(T _arg) { return spDataObject(new DataObject(_arg)); }
template <class T, class T2>
spDataObject sDataObject(T _arg, T2 _arg2) { return spDataObject(new DataObject(_arg, _arg2)); }

// Can help to keep incapsulation
// DataObject move requester, require the memory pointer to be irreversably moved into it
class spDataObjectMove
{
public:
    spDataObjectMove() {}
    void assignPointer(spDataObject const& _obj) { m_obj = _obj; }
    spDataObject& getPointer() { return m_obj; }
    operator DataObject const&() const { return m_obj.getCContent(); }

private:
    spDataObject m_obj;
};

// Move memory from _obj to spDataObjectMove and flush _obj pointer
spDataObjectMove move(spDataObject& _obj);

// Find index that _key should take place in when being added to ordered _objects by key
// Heavy function, use only on export when need to construct json with sorted keys
size_t findOrderedKeyPosition(std::string const& _key, std::vector<spDataObject> const& _objects);

}
