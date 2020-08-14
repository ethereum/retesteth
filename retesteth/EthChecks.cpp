#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <csignal>
#include <iostream>
#include <thread>

namespace test {
void eth_warning_message(std::string const& _message, unsigned _verbosity)
{
    if (Options::get().logVerbosity >= _verbosity)
        std::cout << cYellow << "WARNING: " << _message << "\x1b[0m" << std::endl;
}

void eth_stdout_message(std::string const& _message, std::string const& _color)
{
    if (_color.empty())
        std::cout <<  _message << std::endl;
    else
        std::cout << _color << _message << "\x1b[0m" << std::endl;
}

void eth_stderror_message(std::string const& _message)
{
    std::cerr << cRed << _message << "\x1b[0m" << std::endl;
}

void eth_log_message(std::string const& _message, unsigned _verbosity, LogColor _color)
{
    if (Options::get().logVerbosity >= _verbosity)
    {
        string s_pre;
        switch (_color)
        {
        case LogColor::YELLOW:
            s_pre = cYellow;
            break;
        case LogColor::LIME:
            s_pre = "\x1b[32m";
            break;
        case LogColor::DEFAULT:
            break;
        default:
            break;
        }
        std::cout << s_pre << _message << "\x1b[0m" << std::endl;
    }
}

void eth_error(std::string const& _message)
{
    if (!ExitHandler::receivedExitSignal())
    {
        // if the exception is not allowed, then throw an exception
        if (TestOutputHelper::get().markError(_message))
            throw EthError() << _message;
    }
}

void eth_mark_error(std::string const& _message)
{
    TestOutputHelper::get().markError(_message);
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
    TestOutputHelper::get().markError(_message);
    if (!ExitHandler::receivedExitSignal())
        std::raise(SIGABRT);
    throw std::runtime_error(_message);
}
}
