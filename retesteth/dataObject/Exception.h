#pragma once
#include <dataObject/DataObject.h>
#include <exception>
#include <string>
#include <vector>

using namespace std;
namespace dataobject
{
struct DataObjectException : virtual std::exception
{
    DataObjectException(std::string _message = std::string()) : m_message(std::move(_message)) {}
    const char* what() const noexcept override
    {
        return m_message.empty() ? std::exception::what() : m_message.c_str();
    }
    void setMessage(std::string const& _text) { m_message = _text; }
    DataObjectException& operator<<(const char* _t)
    {
        setMessage(_t);
        return *this;
    }
    DataObjectException& operator<<(string const& _t)
    {
        setMessage(_t);
        return *this;
    }

private:
    std::string m_message;
};


struct ExpectedButGot : public DataObjectException
{
    ExpectedButGot(std::string const& _expected, std::string const& _got)
    {
        setMessage(m_ErrorMessage + _expected + ", but got: " + _got);
    }

    ExpectedButGot(int _expected, int _got)
    {
        setMessage(
            m_ErrorMessage + std::to_string(_expected) + ", but got: " + std::to_string(_got));
    }

private:
    std::string const m_ErrorMessage = "DataObject mismatch! Expected: ";
};
}
