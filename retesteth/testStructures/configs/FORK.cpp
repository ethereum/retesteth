#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
using namespace std;
using namespace test;
using namespace test::teststruct;

namespace
{
size_t getForkIndex(test::teststruct::FORK const& _f)
{
    auto const& forks = Options::getCurrentConfig().cfgFile().forks();
    for (size_t i = 0; i < forks.size(); i++)
    {
        if (forks.at(i) == _f)
            return i;
    }
    string const sConfigPath = Options::getCurrentConfig().cfgFile().path().c_str();
    ETH_FAIL_MESSAGE("Fork name `" + _f.asString() + "` not found in current config 'Forks'! (" + sConfigPath + ")");
    return 0;
}
}  // namespace

namespace test
{
namespace teststruct
{
// If to not use FORK in currentConfig init code.
// Would be possible to calculate the indexes at constructor time
// Given the FORK array is small and usage of compares are single lines it shall be fine
FORK::FORK(DataObject const& _data) : m_data(_data.asString()) {}

bool compareFork(FORK const& _left, CMP _t, FORK const& _right)
{
    switch (_t)
    {
    case CMP::ge:
        return getForkIndex(_left) >= getForkIndex(_right);
        break;
    case CMP::le:
        return getForkIndex(_left) <= getForkIndex(_right);
        break;
    case CMP::gt:
        return getForkIndex(_left) > getForkIndex(_right);
        break;
    case CMP::lt:
        return getForkIndex(_left) < getForkIndex(_right);
        break;
    }
    return 0;
}


}  // namespace teststruct
}  // namespace test
