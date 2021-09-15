#pragma once
#include <retesteth/TestOutputHelper.h>
#include <string>
using namespace test;

namespace  test {
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

namespace logmessage
{
void eth_warning_message(std::string const& _message, unsigned _verbosity = 1);
void eth_stdout_message(std::string const& _message, std::string const& _color = std::string());
void eth_stderror_message(std::string const& _message);
void eth_log_message(std::string const& _message, test::LogColor _logColor = test::LogColor::DEFAULT);
void eth_require(bool _flag);
void eth_check_message(std::string const& _message);
void eth_require_message(std::string const& _message);
void eth_fail(std::string const& _message);
void eth_error(std::string const& _message);
void eth_mark_error(std::string const& _message);
int eth_getVerbosity();

}  // namespace logmessage

using namespace logmessage;

// Prints output to stderr/cout (depending on --verbosity option)
#define ETH_WARNING(message) eth_warning_message(message)
#define ETH_WARNING_TEST(message, verbosity)     \
    if (eth_getVerbosity() >= 6)                 \
    {                                            \
        eth_warning_message(message, verbosity); \
    }

// Plain messages
#define ETH_STDOUT_MESSAGE(message) eth_stdout_message(message)
#define ETH_STDOUT_MESSAGEC(message, color) eth_stdout_message(message, color)
#define ETH_STDERROR_MESSAGE(message) eth_stderror_message(message)

// Verbosity conditional messages
#define ETH_TEST_MESSAGE(message) \
    if (eth_getVerbosity() >= 6)  \
    {                             \
        eth_log_message(message); \
    }
#define ETH_LOG(message, verbosity)      \
    if (eth_getVerbosity() >= verbosity) \
    {                                    \
        eth_log_message(message);        \
    }
#define ETH_LOGC(message, verbosity, color) \
    if (eth_getVerbosity() >= verbosity)    \
    {                                       \
        eth_log_message(message, color);    \
    }

// Notice an error during test execution, but continue other tests
// Throw the exception so to exit the test execution loop
#define ETH_ERROR_MESSAGE(message) eth_error(message)
#define ETH_ERROR_REQUIRE_MESSAGE(flag, message) \
    if (!(flag))                                 \
    {                                            \
        eth_check_message(message);              \
    }

// Notice an error during test execution, but continue current test
// Thie needed to mark multiple error checks into the log in one test
#define ETH_MARK_ERROR(message) eth_mark_error(message)
#define ETH_MARK_ERROR_FLAG(flag, message) \
    if (!(flag))                           \
    {                                      \
        eth_mark_error(message);           \
    }

// Stop retesteth execution rise sigabrt
#define ETH_FAIL_REQUIRE(flag) eth_require(flag)
#define ETH_FAIL_MESSAGE(message) eth_fail(message)
#define ETH_FAIL_REQUIRE_MESSAGE(flag, message) \
    if (!(flag))                                \
    {                                           \
        eth_require_message(message);           \
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

ETHEXCEPTION(UpwardsException);
ETHEXCEPTION(EthError);

}  // namespace
