#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <csignal>
#include <iostream>
#include <thread>

namespace test {
void eth_stderror_message(std::string const& _message)
{
    std::cerr << _message << std::endl;
}

void eth_log_message(std::string const& _message, unsigned _verbosity)
{
    if (Options::get().logVerbosity >= _verbosity)
        std::cout << _message << std::endl;
}

void eth_error(std::string const& _message)
{
    TestOutputHelper::get().markError(_message);
    throw EthError() << _message;
}

void eth_mark_error(bool _flag, std::string const& _message)
{
    if (!_flag)
        TestOutputHelper::get().markError(_message);
}

void eth_check_message(bool _flag, std::string const& _message)
{
    if (!_flag)
        eth_error(_message);
}

void eth_require_message(bool _flag, std::string const& _message)
{
    if (!_flag)
        eth_fail(_message);
}

void eth_require(bool _flag)
{
    if (!_flag)
        eth_fail("Flag error");
}

void eth_fail(std::string const& _message)
{
    // thread that failing with this function might be being joined in a loop
    // std::cerr << _message << std::endl;
    TestOutputHelper::get().markError(_message);
    std::raise(SIGABRT);
    throw std::runtime_error(_message);
}

}
