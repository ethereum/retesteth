#pragma once
#include <testeth/DataObject.h>

namespace test {
	class object
	{
		public:
		object(DataObject const& _json):
			m_data(_json)
		{}
		DataObject const& getData() const { return m_data; }
		protected:
		DataObject m_data;
	};
}

