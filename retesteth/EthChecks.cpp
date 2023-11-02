#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <boost/algorithm/string.hpp>
#include <csignal>
#include <mutex>
using namespace std;

mutex g_debugFlagAccess;
namespace test::debug
{

Debug const& Debug::get()
{
    static Debug instance;
    #if defined(UNITTESTS) || defined(__DEBUG__)
    {
        if (TestOptions::isOverride())
        {
            static Debug* instance = nullptr;
            // Reinitialize debug with test options
            if (instance != nullptr)
                delete instance;
            instance = new Debug();
            return *instance;
        }
    }
    #endif
    return instance;
}

Debug::Debug()
{
    for (int channel = DC::RPC; channel != DC::LOWLOG; channel++)
    {
        DC channelType = static_cast<DC>(channel);
        m_channels[channelType] = false;
    }
    m_channels[DC::LOWLOG] = false;
    if (!Options::get().logVerbosity.str.empty())
    {
        std::vector<string> elements;
        boost::split(elements, Options::get().logVerbosity.str, boost::is_any_of("|"));
        for (auto const& flag : elements)
        {
            m_channels[DC::DEFAULT] = true;
            if (flag == "RPC")
            {
                m_channels[DC::RPC] = true;
                m_channels[DC::WARNING] = true;
            }
            else if (flag == "RPC2")
            {
                m_channels[DC::RPC2] = true;
                m_channels[DC::WARNING] = true;
            }
            else if (flag == "STATS")
                m_channels[DC::STATS] = true;  // Default test execution stats
            else if (flag == "STATS2")
                m_channels[DC::STATS2] = true;
            else if (flag == "STATE")
                m_channels[DC::STATE] = true;  // Poststate output
            else if (flag == "SOCKET")
                m_channels[DC::SOCKET] = true;  // Socket debug information
            else if (flag == "TESTLOG")
                m_channels[DC::TESTLOG] = true;  // Additional test execution info
            else if (flag == "LOWLOG")
                m_channels[DC::LOWLOG] = true;  // Peculiar debug info
            else if (flag == "WARNING")
                m_channels[DC::WARNING] = true;  // Warning messages
            else if (flag == "PYSPEC")
            {
                m_channels[DC::PYSPEC] = true;  // Python script errorlog
                m_channels[DC::WARNING] = true;
            }
            else
                ETH_STDOUT_MESSAGEC("WARNING: Debug channel `" + flag + "` not found!", cYellow);
        }
    }
    else
        initializeDefaultChannels();
};

bool Debug::flag(DC _channel) const
{
    std::lock_guard<std::mutex> lock(g_debugFlagAccess);
    return m_channels.at(_channel);
}

void Debug::initializeDefaultChannels()
{
    // Default log channel initialization
    auto const& verb = Options::get().logVerbosity.val;
    m_channels[DC::DEFAULT] = true;
    if (verb >= 1)
    {
        m_channels[DC::STATS] = true;
        m_channels[DC::WARNING] = true;
    }
    if (verb >= 2)
        m_channels[DC::STATS2] = true;
    if (verb >= 3)
        m_channels[DC::TESTLOG] = true; 
    if (verb >= 5)
        m_channels[DC::STATE] = true;
    if (verb >= 6)
        m_channels[DC::RPC] = true;
    if (verb >= 7)
    {
        m_channels[DC::SOCKET] = true;
        m_channels[DC::LOWLOG] = true;
        m_channels[DC::RPC2] = true;
        m_channels[DC::PYSPEC] = true;
    }
    if (Options::get().poststate.initialized() || Options::get().statediff.initialized())
        m_channels[DC::STATE] = true;
}


std::string const cBYellowBlack = "\x1b[43m\x1b[30m";
std::string const cYellow = "\x1b[33m";
std::string const cLime = "\x1b[32m";
std::string const cRed = "\x1b[0;31m";
std::string const cDefault = "\x1b[0m";
std::string const cZero = "";


namespace logmessage
{
void eth_warning_message(std::string const& _message)
{
    TestOutputHelper::get().markWarning(_message);
    if (Options::get().nologcolor)
    {
        ETH_DC_MESSAGE(DC::WARNING, "WARNING: " + _message);
    }
    else
    {
        ETH_DC_MESSAGE(DC::WARNING, cYellow + "WARNING: " + _message + "\x1b[0m");
    }
}

void eth_stdout_message(std::string const& _message, std::string const& _color)
{
    if (_color.empty() || Options::get().nologcolor)
        std::cout <<  _message << std::endl;
    else
        std::cout << _color << _message << "\x1b[0m" << std::endl;
}

void eth_stderror_message(std::string const& _message)
{
    if (Options::get().nologcolor)
        std::cerr << _message << std::endl;
    else
        std::cerr << cRed << _message << "\x1b[0m" << std::endl;
}

inline string const& LogColorToColor(LogColor _color)
{
    switch (_color)
    {
    case LogColor::YELLOW: return cYellow;
    case LogColor::LIME: return cLime;
    case LogColor::DEFAULT: return cZero;
    default: return cZero;
    }
    return cZero;
}

void eth_log_message(std::string const& _message, LogColor _color)
{
    if (Options::get().nologcolor || _color == LogColor::DEFAULT)
        std::cout << _message << std::endl;
    else
        std::cout << LogColorToColor(_color) << _message << cDefault << std::endl;
}

void eth_error(std::string const& _message)
{
    // Do not mark errors if exiting the program by emergency
    if (ExitHandler::receivedExitSignal())
        throw EthError() << _message;
    else
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

int eth_getVerbosity()
{
    return Options::get().logVerbosity.val;
}

}  // namespace logmessage

}  // namespace test::debug
