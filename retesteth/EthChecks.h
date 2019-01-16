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

#define ETH_ERROR_MESSAGE(message) test::eth_error_message(message)
#define ETH_TEST_MESSAGE(message) test::eth_log_message(message, 6)
#define ETH_LOG(message, verbosity) test::eth_log_message(message, verbosity)
#define ETH_CHECK_MESSAGE(flag, message) test::eth_check_message(flag, message)
#define ETH_REQUIRE_MESSAGE(flag, message) test::eth_require_message(flag, message)
#define ETH_REQUIRE(flag) test::eth_require(flag)
#define ETH_FAIL(message) test::eth_fail(message)
#define ETH_ERROR(message) test::eth_error(message)


// Helpers
template <class T>
void eth_check_equal(T a, T b, std::string const& _message)
{
    eth_check_message(a == b, _message + test::expButGot(b, a));
}
#define ETH_CHECK_EQUAL(val1, val2, message) test::eth_check_equal(val1, val2, message)
}
