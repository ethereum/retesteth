#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <unordered_set>
#include <functional>
#include <string>
#include <chrono>
#include <ctime>
#pragma warning(push)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/multiprecision/cpp_int.hpp>
#pragma warning(pop)
#pragma GCC diagnostic pop
#include "vector_ref.h"

// Quote a given token stream to turn it into a string.
#define DEV_QUOTED_HELPER(s) #s
#define DEV_QUOTED(s) DEV_QUOTED_HELPER(s)

#define DEV_IGNORE_EXCEPTIONS(X) try { X; } catch (...) {}

#define DEV_IF_THROWS(X) try{X;}catch(...)

namespace  dev {

// Binary data types.
using byte = uint8_t;
using bytes = std::vector<byte>;
using bytesRef = vector_ref<byte>;
using bytesConstRef = vector_ref<byte const>;

template <class T>
class secure_vector
{
public:
	secure_vector() {}
	secure_vector(secure_vector<T> const& /*_c*/) = default;  // See https://github.com/ethereum/libweb3core/pull/44
	explicit secure_vector(size_t _size): m_data(_size) {}
	explicit secure_vector(size_t _size, T _item): m_data(_size, _item) {}
	explicit secure_vector(std::vector<T> const& _c): m_data(_c) {}
	explicit secure_vector(vector_ref<T> _c): m_data(_c.data(), _c.data() + _c.size()) {}
	explicit secure_vector(vector_ref<const T> _c): m_data(_c.data(), _c.data() + _c.size()) {}
	~secure_vector() { ref().cleanse(); }

	secure_vector<T>& operator=(secure_vector<T> const& _c)
	{
		if (&_c == this)
			return *this;

		ref().cleanse();
		m_data = _c.m_data;
		return *this;
	}
	std::vector<T>& writable() { clear(); return m_data; }
	std::vector<T> const& makeInsecure() const { return m_data; }

	void clear() { ref().cleanse(); }

	vector_ref<T> ref() { return vector_ref<T>(&m_data); }
	vector_ref<T const> ref() const { return vector_ref<T const>(&m_data); }

	size_t size() const { return m_data.size(); }
	bool empty() const { return m_data.empty(); }

	void swap(secure_vector<T>& io_other) { m_data.swap(io_other.m_data); }

private:
	std::vector<T> m_data;
};

using bytesSec = secure_vector<byte>;

// Numeric types.
using bigint = boost::multiprecision::number<boost::multiprecision::cpp_int_backend<>>;
using u64 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<64, 64, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
using u128 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<128, 128, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
using u256 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
using s256 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void>>;
using u160 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<160, 160, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
using s160 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<160, 160, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void>>;
using u512 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<512, 512, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
using s512 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<512, 512, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void>>;
using u256s = std::vector<u256>;
using u160s = std::vector<u160>;
using u256Set = std::set<u256>;
using u160Set = std::set<u160>;

// Map types.
using StringMap = std::map<std::string, std::string>;
using BytesMap = std::map<bytes, bytes>;
using u256Map = std::map<u256, u256>;
using HexMap = std::map<bytes, bytes>;

// Hash types.
using StringHashMap = std::unordered_map<std::string, std::string>;
using u256HashMap = std::unordered_map<u256, u256>;

// String types.
using strings = std::vector<std::string>;

// Fixed-length string types.
using string32 = std::array<char, 32>;

// Null/Invalid values for convenience.
extern bytes const NullBytes;
extern u256 const Invalid256;

class Timer
{
public:
	Timer() { restart(); }

	std::chrono::high_resolution_clock::duration duration() const { return std::chrono::high_resolution_clock::now() - m_t; }
	double elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(duration()).count() / 1000000.0; }
	void restart() { m_t = std::chrono::high_resolution_clock::now(); }

private:
	std::chrono::high_resolution_clock::time_point m_t;
};

class CPUTimer
{
public:
    CPUTimer(){ restart(); }
    void restart() { m_start = std::clock(); }
    double elapsed() const
    {
        std::clock_t end = std::clock();
        double const elapsedMs =  1000.0 * (end - m_start) / CLOCKS_PER_SEC;
        return elapsedMs / 1000.0;
    }
private:
    std::clock_t m_start;
};

}
