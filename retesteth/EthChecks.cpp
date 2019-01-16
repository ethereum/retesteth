#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <iostream>
#include <csignal>
#include <thread>

namespace test {
void eth_error_message(std::string const& _message)
{
    std::cerr << _message << std::endl;
}

void eth_log_message(std::string const& _message, unsigned _verbosity)
{
    if (Options::get().logVerbosity >= _verbosity)
        std::cout << _message << std::endl;
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
        //        std::cerr << _message << std::endl;
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
    std::cerr << "--------" << std::endl;
    std::cerr << _message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::raise(SIGABRT);
    TestOutputHelper::get().markError(_message);
    throw std::exception();
}

void eth_error(std::string const& _message)
{
    TestOutputHelper::get().markError(_message);
}

}
