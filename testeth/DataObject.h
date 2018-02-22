#pragma once
#include <memory>
#include <vector>
#include <boost/test/unit_test.hpp>
#include <devcore/CommonIO.h>

namespace  test {

enum DataType
{
	String,
	Integer,
	Array,
	Object,
	Null
};

class DataObject
{
	public:
	DataObject(DataType _type) { m_type = _type; }
	DataObject(std::string const& _str) { m_type = DataType::String; m_strVal = _str; }
	DataObject(std::string const& _key, std::string const& _str) { m_type = DataType::String; m_strVal = _str; m_strKey = _key; }
	DataObject(int _int) { m_type = DataType::Integer; m_intVal = _int; }
	DataType type() const { return m_type; }
	void setKey(std::string const& _key) { m_strKey = _key; }
	std::string const& getKey() const { return m_strKey; }
	std::vector<DataObject>const& getSubObjects() const { return m_subObjects; }
	void addSubObject(DataObject const& _obj)
	{
		if (m_type == DataType::Null)
			m_type = DataType::Object;
		m_subObjects.push_back(_obj);
	}
	void setSubObjectKey(size_t _index, std::string const& _key)
	{
		assert(_index < m_subObjects.size());
		if (m_subObjects.size() > _index)
			m_subObjects.at(_index).setKey(_key);
	}
	bool count(std::string const& _key) const
	{
		for (auto const& i: m_subObjects)
			if (i.getKey() == _key)
				return true;
		return false;
	}
	std::string asString() const { return m_strVal; }
	DataObject& operator[] (std::string const& _key)
	{
		assert(count(_key));
		for (auto& i: m_subObjects)
			if (i.getKey() == _key)
				return i;
		return m_subObjects[0]; // should never hit this line
	}

	DataObject const& at(std::string const& _key) const
	{
		assert(count(_key));
		for (auto const& i: m_subObjects)
			if (i.getKey() == _key)
				return i;
		return m_subObjects[0]; // should never hit this line
	}

	void addArrayObject(DataObject const& _obj)
	{
		assert(m_type == DataType::Null || m_type == DataType::Array);
		m_type = DataType::Array;
		m_subObjects.push_back(_obj);
	}
	void print(size_t level = 0) const
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

	static std::string dataTypeAsString(DataType _type)
	{
		switch (_type) {
		case String: return "string";
		case Integer: return "int";
		case Array: return "array";
		case Object: return "object";
		case Null: return "null";
		default:
		break;
		}
		return "";
	}

	private:
	std::vector<DataObject> m_subObjects;
	DataType m_type;
	std::string m_strKey;
	std::string m_strVal;
	int m_intVal;
};
}
