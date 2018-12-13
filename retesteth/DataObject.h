#pragma once
#include <memory>
#include <vector>
#include <boost/test/unit_test.hpp>
#include <libdevcore/CommonIO.h>

namespace  test {
enum DataType
{
    String,
    Integer,
    Bool,
    Array,
    Object,
    Null
};

class DataObject
{
	public:
	DataObject();
	DataObject(DataType _type);
    DataObject(DataType _type, bool _bool);
    DataObject(std::string const& _str);
	DataObject(std::string const& _key, std::string const& _str);
	DataObject(int _int);
	DataType type() const;
	void setKey(std::string const& _key);
	std::string const& getKey() const;
	std::vector<DataObject>const& getSubObjects() const;
	std::vector<DataObject>& getSubObjectsUnsafe();
	void addSubObject(DataObject const& _obj);
	void addSubObject(std::string const& _key, DataObject const& _obj);
	void setSubObjectKey(size_t _index, std::string const& _key);

	bool count(std::string const& _key) const;
    std::string const& asString() const;
    int asInt() const;
	bool asBool() const;

	void setKeyPos(std::string const& _key, size_t _pos);
	DataObject& operator[] (std::string const& _key)
	{
        _assert(m_type == DataType::Null || m_type == DataType::Object, "m_type == DataType::Null || m_type == DataType::Object");
		for (auto& i: m_subObjects)
			if (i.getKey() == _key)
				return i;
		DataObject obj(DataType::Null);
		obj.setKey(_key);
		_addSubObject(obj);
		return m_subObjects[m_subObjects.size() - 1];
	}

	DataObject& operator = (std::string const& _value)
	{
		assert(m_type == DataType::String || m_type == DataType::Null);
		m_type = DataType::String;
		m_strVal = _value;
		return *this;
	}

	DataObject& operator = (int _value)
	{
		assert(m_type == DataType::Integer || m_type == DataType::Null);
		m_type = DataType::Integer;
		m_intVal = _value;
		return *this;
	}

	DataObject& operator = (DataObject const& _value)
	{
		assert(m_type == DataType::Null); // So not to overwrite the existing data
		// Do not replace the key. Assuming that key is set upon calling DataObject[key] =
		m_type = _value.type();
		switch(_value.type())
		{
			case DataType::Integer:
				m_intVal = _value.asInt(); break;
			case DataType::String:
				m_strVal = _value.asString(); break;
			case DataType::Bool:
				m_boolVal = _value.asBool(); break;
			default:
				break;
		}
		m_subObjects = _value.getSubObjects();
		return *this;
	}

	void replace(DataObject const& _value);

	DataObject const& at(std::string const& _key) const;

	void addArrayObject(DataObject const& _obj);

	void renameKey(std::string const& _currentKey, std::string const& _newKey);

    /// vector<element> erase method with `replace()` function
    void removeKey(std::string const& _key);

    void clear();

    std::string asJson(int level = 0, bool pretty = true) const;
    static std::string dataTypeAsString(DataType _type);

	private:
	void _addSubObject(DataObject const& _obj);
	void _checkDoubleKeys() const;
	void _assert(bool _flag, std::string const& _comment = "") const;

	std::vector<DataObject> m_subObjects;
	DataType m_type;
	std::string m_strKey;
	std::string m_strVal;
	bool m_boolVal;
	int m_intVal;
};
}
