#pragma once
#include <exception>
#include <string>
#include <vector>

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
    DataObjectException& operator<<(std::string const& _t)
    {
        setMessage(_t);
        return *this;
    }

private:
    std::string m_message;
};


struct ExpectedButGot : public DataObjectException
{
    ExpectedButGot(std::string const& _expected, std::string const& _got, bool _quotes = true)
    {
        if (_quotes)
            setMessage(m_ErrorMessage + "'" + _expected + "', but got: '" + _got + "'");
        else
            setMessage(m_ErrorMessage + _expected + ", but got: " + _got);
    }

    ExpectedButGot(bool _expected, bool _got)
    {
        std::string expected = _expected ? "'true'" : "'false'";
        std::string got = _got ? "'true'" : "'false'";
        setMessage(m_ErrorMessage + expected + ", but got: " + got);
    }

    ExpectedButGot(int _expected, int _got)
    {
        setMessage(m_ErrorMessage + "'" + std::to_string(_expected) + "', but got: '" +
                   std::to_string(_got) + "'");
    }

private:
    std::string const m_ErrorMessage = "DataObject mismatch! Expected: ";
};
}
