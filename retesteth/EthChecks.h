#pragma once
#include <string>

namespace  test {

void eth_test_message(std::string const& _message);
void eth_require(bool _flag);
void eth_check_message(bool _flag, std::string const& _message);
void eth_require_message(bool _flag, std::string const& _message);
void eth_fail(std::string const& _message);
void eth_error(std::string const& _message);

#define ETH_TEST_MESSAGE(message) test::eth_test_message(message)
#define ETH_CHECK_MESSAGE(flag, message) test::eth_check_message(flag, message)
#define ETH_REQUIRE_MESSAGE(flag, message) test::eth_require_message(flag, message)
#define ETH_REQUIRE(flag) test::eth_require(flag)
#define ETH_FAIL(message) test::eth_fail(message)
#define ETH_ERROR(message) test::eth_error(message)

}
