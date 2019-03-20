#pragma once
#include <retesteth/TestOutputHelper.h>
#include <string>
namespace  test {
void eth_error_message(std::string const& _message);
void eth_log_message(std::string const& _message, unsigned _verbosity);
void eth_require(bool _flag);
void eth_check_message(bool _flag, std::string const& _message);
void eth_require_message(bool _flag, std::string const& _message);
void eth_fail(std::string const& _message);
void eth_error(std::string const& _message);

// Prints output to stderr/cout (depending on --verbosity option)
#define ETH_STDERROR_MESSAGE(message) test::eth_error_message(message)
#define ETH_TEST_MESSAGE(message) test::eth_log_message(message, 6)
#define ETH_LOG(message, verbosity) test::eth_log_message(message, verbosity)

// refactor exceptions !!!
// 1) FATAL ERROR. STOP TEST. STOP RETESTETH
// 2) LOCAL TEST ERROR. STOP TEST. WAIT OTHER TO FINISH. CONTINUE SUITE EXEC

// Notice an error during test execution, but continue other tests
#define ETH_CHECK_MESSAGE(flag, message) test::eth_check_message(flag, message)
#define ETH_ERROR(message) test::eth_error(message)

// Stop test suite execution, continue other tests
#define ETH_REQUIRE_MESSAGE(flag, message) test::eth_require_message(flag, message)
#define ETH_REQUIRE(flag) test::eth_require(flag)

// Stop retesteth execution rise sigabrt
#define ETH_FAIL(message) test::eth_fail(message)


// Helpers
template <class T>
void eth_check_equal(T a, T b, std::string const& _message)
{
    eth_check_message(a == b, _message + test::expButGot(b, a));
}
#define ETH_CHECK_EQUAL(val1, val2, message) test::eth_check_equal(val1, val2, message)
}
