#include <libdevcore/Log.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <iostream>
#include <csignal>
#include <thread>

namespace test {

void eth_test_message(std::string const& _message)
{
    if (g_logVerbosity > 6)
        std::cerr << _message << std::endl;
}

void eth_require(bool _flag)
{
    if (!_flag)
    {
        std::raise(SIGABRT);
        TestOutputHelper::get().markError("Flag error");
    }
}

void eth_check_message(bool _flag, std::string const& _message)
{
    if (!_flag)
    {
        std::cerr << _message << std::endl;
        TestOutputHelper::get().markError(_message);
    }
}

void eth_require_message(bool _flag, std::string const& _message)
{
    if (!_flag)
        eth_fail(_message);
}

void eth_fail(std::string const& _message)
{
    std::cerr << _message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::raise(SIGABRT);
    TestOutputHelper::get().markError(_message);
    throw std::exception();
}

void eth_error(std::string const& _message)
{
    std::cerr << _message << std::endl;
    TestOutputHelper::get().markError(_message);
}

}
