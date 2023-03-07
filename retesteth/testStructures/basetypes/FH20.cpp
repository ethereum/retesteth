#include "FH20.h"
#include "Constants.h"
#include <algorithm>
#include <random>
using namespace test::teststruct;

FH20 const& FH20::zero()
{
    return C_FH20_ZERO;
}

FH20 FH20::random()
{
    std::string initStr = "0x";
    std::vector<std::string> hex{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
    auto rng = std::default_random_engine{};
    for (size_t i = 0; i < 40; i++)
    {
        std::shuffle(hex.begin(), hex.end(), rng);
        initStr += hex.at(5);
    }
    return FH20(DataObject(std::move(initStr)));
}
