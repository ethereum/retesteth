#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <iostream>
#include <csignal>

namespace test {

void eth_test_message(std::string const& _message)
{
    (void) _message;
    if (Options::get().logVerbosity == Verbosity::Full)
        std::cerr << _message << std::endl;
}

void eth_require(bool _flag)
{
    if (!_flag)
        std::raise(SIGABRT);
}

void eth_check_message(bool _flag, std::string const& _message)
{
    if (!_flag)
        std::cerr << _message << std::endl;
}

void eth_require_message(bool _flag, std::string const& _message)
{
    if (!_flag)
    {
        std::cerr << _message << std::endl;
        std::raise(SIGABRT);
    }
}

void eth_fail(std::string const& _message)
{
    std::cerr << _message << std::endl;
    std::raise(SIGABRT);
}

}
