#pragma once

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <string>
#include <boost/noncopyable.hpp>

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
class Socket: public boost::noncopyable
{
public:
    enum SocketType
    {
        IPC,
        TCP
    };
    explicit Socket(SocketType _type, std::string const& _path);
    std::string sendRequest(std::string const& _req);
    ~Socket() { close(m_socket); }

    std::string const& path() const { return m_path; }

private:

    std::string m_path;
    int m_socket;
    /// Socket read timeout in milliseconds. Needs to be large because the key generation routine
    /// might take long.
    unsigned static constexpr m_readTimeOutMS = 30000;
    char m_readBuf[512000];
};
#endif
