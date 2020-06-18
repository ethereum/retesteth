#pragma once
#include <retesteth/TestOutputHelper.h>
#include <string>
namespace  test {
enum class LogColor
{
    DEFAULT,
    YELLOW,
    LIME
};
std::string const cYellow = "\x1b[33m";
std::string const cRed = "\x1b[31m";
void eth_warning_message(std::string const& _message, unsigned _verbosity = 1);
void eth_stdout_message(std::string const& _message);
void eth_stderror_message(std::string const& _message);
void eth_log_message(
    std::string const& _message, unsigned _verbosity, LogColor _logColor = LogColor::DEFAULT);
void eth_require(bool _flag);
void eth_check_message(bool _flag, std::string const& _message);
void eth_require_message(bool _flag, std::string const& _message);
void eth_fail(std::string const& _message);
void eth_error(std::string const& _message);
void eth_mark_error(bool _flag, std::string const& _message);
void eth_mark_error(std::string const& _message);

// Prints output to stderr/cout (depending on --verbosity option)
#define ETH_WARNING(message) test::eth_warning_message(message)
#define ETH_WARNING_TEST(message, verbosity) test::eth_warning_message(message, verbosity)

#define ETH_STDOUT_MESSAGE(message) test::eth_stdout_message(message)
#define ETH_STDERROR_MESSAGE(message) test::eth_stderror_message(message)
#define ETH_TEST_MESSAGE(message) test::eth_log_message(message, 6)
#define ETH_LOG(message, verbosity) test::eth_log_message(message, verbosity)
#define ETH_LOGC(message, verbosity, color) test::eth_log_message(message, verbosity, color)

// Notice an error during test execution, but continue other tests
// Throw the exception so to exit the test execution loop
#define ETH_ERROR_MESSAGE(message) test::eth_error(message)
#define ETH_ERROR_REQUIRE_MESSAGE(flag, message) test::eth_check_message(flag, message)

// Notice an error during test execution, but continue current test
// Thie needed to mark multiple error checks into the log in one test
#define ETH_MARK_ERROR(message) test::eth_mark_error(message)
#define ETH_MARK_ERROR_FLAG(flag, message) test::eth_mark_error(flag, message)

// Stop retesteth execution rise sigabrt
#define ETH_FAIL_REQUIRE(flag) test::eth_require(flag)
#define ETH_FAIL_MESSAGE(message) test::eth_fail(message)
#define ETH_FAIL_REQUIRE_MESSAGE(flag, message) test::eth_require_message(flag, message)

// Helpers
template <class T>
void eth_check_equal(T a, T b, std::string const& _message)
{
    eth_check_message(a == b, _message + test::expButGot(b, a));
}
#define ETH_CHECK_EQUAL(val1, val2, message) test::eth_check_equal(val1, val2, message)

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
