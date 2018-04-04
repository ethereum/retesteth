#include <testeth/EthChecks.h>
#include <iostream>

namespace test {

void eth_test_message(std::string const& _message)
{
    (void) _message;
    //std::cout << _message << std::endl;
}

void eth_require(bool _flag)
{
    if (!_flag)
        exit(-1);
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
        exit(-1);
    }
}


}
