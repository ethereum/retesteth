#include "Socket.h"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <retesteth/EthChecks.h>

using namespace std;

Socket::Socket(SocketType _type, string const& _path): m_path(_path)
{
#if defined(_WIN32)
    m_socket = CreateFile(
        m_path.c_str(),   // pipe name
        GENERIC_READ |  // read and write access
        GENERIC_WRITE,
        0,              // no sharing
        NULL,           // default security attribute
        OPEN_EXISTING,  // opens existing pipe
        0,              // default attributes
        NULL);          // no template file

    if (m_socket == INVALID_HANDLE_VALUE)
        ETH_FAIL("Error creating IPC socket object!");

#else
    if (_type == SocketType::IPC)
    {
        if (_path.length() >= sizeof(sockaddr_un::sun_path))
            ETH_FAIL("Error opening IPC: socket path is too long!");

        struct sockaddr_un saun;
        memset(&saun, 0, sizeof(sockaddr_un));
        saun.sun_family = AF_UNIX;
        strcpy(saun.sun_path, _path.c_str());

        // http://idletechnology.blogspot.ca/2011/12/unix-domain-sockets-on-osx.html
        //
        // SUN_LEN() might be optimal, but it seemingly affects the portability,
        // with at least Android missing this macro.  Just using the sizeof() for
        // structure seemingly works, and would only have the side-effect of
        // sending larger-than-required packets over the socket.  Given that this
        // code is only used for unit-tests, that approach seems simpler.
        #if defined(__APPLE__)
            saun.sun_len = sizeof(struct sockaddr_un);
        #endif //  defined(__APPLE__)

        if ((m_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
            ETH_FAIL("Error creating IPC socket object");

        if (connect(m_socket, reinterpret_cast<struct sockaddr const*>(&saun), sizeof(struct sockaddr_un)) < 0)
        {
            close(m_socket);
            ETH_FAIL("Error connecting to IPC socket: " + _path);
        }
    } else if (_type == SocketType::TCP)
    {

        struct sockaddr_in sin;
        sin.sin_family = AF_INET;

        size_t pos = _path.find_last_of(':');
        string address = _path.substr(0, pos);
        int port = atoi(_path.substr(pos + 1).c_str());

        sin.sin_addr.s_addr = inet_addr(address.c_str());
        sin.sin_port = htons(port);

        if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            ETH_FAIL("Error creating TCP socket object");

        if (connect(m_socket, reinterpret_cast<struct sockaddr const*>(&sin), sizeof(struct sockaddr_in)) < 0)
        {
            close(m_socket);
            ETH_FAIL("Error connecting to TCP socket: " + _path);
        }
    }
#endif
}

string Socket::sendRequest(string const& _req)
{
#if defined(_WIN32)
    // Write to the pipe.
    DWORD cbWritten;
    BOOL fSuccess = WriteFile(
        m_socket,               // pipe handle
        _req.c_str(),           // message
        _req.size(),            // message length
        &cbWritten,             // bytes written
        NULL);                  // not overlapped

    if (!fSuccess || (_req.size() != cbWritten))
        ETH_FAIL("WriteFile to pipe failed");

    // Read from the pipe.
    DWORD cbRead;
    fSuccess = ReadFile(
        m_socket,          // pipe handle
        m_readBuf,         // buffer to receive reply
        sizeof(m_readBuf), // size of buffer
        &cbRead,           // number of bytes read
        NULL);             // not overlapped

    if (!fSuccess)
        ETH_FAIL("ReadFile from pipe failed");

    return string(m_readBuf, m_readBuf + cbRead);
#else

    char buf;
    recv(m_socket, &buf, 1, MSG_PEEK | MSG_DONTWAIT);
    if (errno == ENOTCONN)
        ETH_FAIL("Socket connection error! ");

    if (send(m_socket, _req.c_str(), _req.length(), 0) != (ssize_t)_req.length())
        ETH_FAIL("Writing on socket failed.");

    auto start = chrono::steady_clock::now();
    ssize_t ret;
    string reply;
    do
    {
        ret = recv(m_socket, m_readBuf, sizeof(m_readBuf), 0);

        // Also consider closed socket an error.
        if (ret < 0)
            ETH_FAIL("Reading on socket failed!");

        // check for a long message
        if (ret != 0)
        {
            ssize_t ret2 = ret;
            do
            {
                reply += string(m_readBuf, m_readBuf + ret2);
                ret2 = recv(m_socket, m_readBuf, sizeof(m_readBuf), MSG_DONTWAIT);
                if (ret2 > 0)
                    std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for socket update
            }
            while (ret2 > 0);
        }
    }
    while (
        ret == 0 &&
        chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() < m_readTimeOutMS
    );

    if (ret == 0)
        ETH_FAIL("Timeout reading on socket.");

    return reply;
#endif
}
