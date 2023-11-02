#pragma once
#include <map>
#include <string>

namespace test::debug
{
enum DC
{
    RPC,
    RPC2,
    DEFAULT,
    SOCKET,
    STATS,
    STATS2,
    STATE,
    WARNING,
    TESTLOG,
    PYSPEC,
    LOWLOG
};

class Debug
{
public:
    static Debug const& get();
    bool flag(DC _channel) const;

private:
    Debug();
    Debug(Debug const&) = delete;
    void initializeDefaultChannels();
    std::map<DC, bool> m_channels;
};

enum class LogColor
{
    DEFAULT,
    YELLOW,
    LIME
};
extern std::string const cBYellowBlack;
extern std::string const cYellow;
extern std::string const cLime;
extern std::string const cRed;
extern std::string const cDefault;
extern std::string const cZero;

namespace logmessage
{
void eth_warning_message(std::string const& _message);
void eth_stdout_message(std::string const& _message, std::string const& _color = std::string());
void eth_stderror_message(std::string const& _message);
void eth_log_message(std::string const& _message, test::debug::LogColor _logColor = test::debug::LogColor::DEFAULT);
void eth_require(bool _flag);
void eth_fail(std::string const& _message);
void eth_error(std::string const& _message);
void eth_mark_error(std::string const& _message);
int eth_getVerbosity();
}  // namespace logmessage


// Channels message
#define ETH_DC_MESSAGE(dc, message)                        \
    if (test::debug::Debug::get().flag(dc))                \
    {                                                      \
        test::debug::logmessage::eth_log_message(message); \
    }

#define ETH_DC_MESSAGEC(dc, message, color)                       \
    if (test::debug::Debug::get().flag(dc))                       \
    {                                                             \
        test::debug::logmessage::eth_log_message(message, color); \
    }

#define ETH_WARNING(message) test::debug::logmessage::eth_warning_message(message)

// Plain messages
#define ETH_STDOUT_MESSAGE(message) test::debug::logmessage::eth_stdout_message(message)
#define ETH_STDOUT_MESSAGEC(message, color) test::debug::logmessage::eth_stdout_message(message, color)
#define ETH_STDERROR_MESSAGE(message) test::debug::logmessage::eth_stderror_message(message)

// Notice an error during test execution, but continue other tests
// Throw the exception so to exit the test execution loop
#define ETH_ERROR_MESSAGE(message) test::debug::logmessage::eth_error(message)
#define ETH_ERROR_REQUIRE_MESSAGE(flag, message)     \
    if (!(flag))                                     \
    {                                                \
        test::debug::logmessage::eth_error(message); \
    }

// Notice an error during test execution, but continue current test
// Thie needed to mark multiple error checks into the log in one test
#define ETH_MARK_ERROR(message) test::debug::logmessage::eth_mark_error(message)
#define ETH_MARK_ERROR_FLAG(flag, message)                \
    if (!(flag))                                          \
    {                                                     \
        test::debug::logmessage::eth_mark_error(message); \
    }

// Stop retesteth execution rise sigabrt
#define ETH_FAIL_REQUIRE(flag) test::debug::logmessage::eth_require(flag)
#define ETH_FAIL_MESSAGE(message) test::debug::logmessage::eth_fail(message)
#define ETH_FAIL_REQUIRE_MESSAGE(flag, message)     \
    if (!(flag))                                    \
    {                                               \
        test::debug::logmessage::eth_fail(message); \
    }

/// Base class for all exceptions.
#define ETHEXCEPTION(X)                                                                                                       \
    struct X : virtual std::exception                                                                                         \
    {                                                                                                                         \
        X(std::string _message = std::string()) : m_message(std::move(_message)) {}                                           \
        const char* what() const noexcept override { return m_message.empty() ? std::exception::what() : m_message.c_str(); } \
        X& operator<<(std::string const& _what)                                                                               \
        {                                                                                                                     \
            m_message = _what;                                                                                                \
            return *this;                                                                                                     \
        }                                                                                                                     \
                                                                                                                              \
    private:                                                                                                                  \
        std::string m_message;                                                                                                \
    }

}  // namespace test::debug

namespace test
{
ETHEXCEPTION(UpwardsException);
ETHEXCEPTION(EthError);
}  // namespace test
