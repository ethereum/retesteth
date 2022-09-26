#pragma once

#if defined(_WIN32)
#include <windows.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#endif

#include <boost/noncopyable.hpp>
#include <string>

namespace test::session
{
class SocketResponseValidator
{
public:
    virtual void acceptResponse(std::string const& _response) = 0;
    virtual bool completeResponse() const = 0;
    virtual std::string const& getResponse() const = 0;
};

class JsonObjectValidator : public SocketResponseValidator
{
public:
    JsonObjectValidator();
    void acceptResponse(std::string const& _response) override;
    bool completeResponse() const override;
    std::string const& getResponse() const override;

private:
    std::string m_response;
    bool m_status;
    int m_bracersCount;
};

#if defined(_WIN32)
class Socket : public boost::noncopyable
{
public:
    explicit Socket(SocketType _type, std::string const& _path);
    std::string sendRequest(std::string const& _req);
    ~Socket() { CloseHandle(m_socket); }

    std::string const& path() const { return m_path; }

private:
    std::string m_path;
    HANDLE m_socket;
    TCHAR m_readBuf[512000];
};
#else
class Socket : public boost::noncopyable
{
public:
    enum SocketType
    {
        IPC,
        TCP
    };
    explicit Socket(SocketType _type, std::string const& _path);
    std::string sendRequest(std::string const& _req, SocketResponseValidator& _responseValidator);
    ~Socket() { close(m_socket); }

    std::string const& path() const { return m_path; }
    SocketType type() const { return m_socketType; }

private:
    std::string m_path;
    int m_socket;
    SocketType m_socketType;
    /// Socket read timeout in milliseconds. Needs to be large because the key generation routine
    /// might take long.
    unsigned static constexpr m_readTimeOutMS = 130000;
    char m_readBuf[512000];
    std::string sendRequestIPC(std::string const& _req, SocketResponseValidator& _val);
};
#endif

}  // namespace test::session
