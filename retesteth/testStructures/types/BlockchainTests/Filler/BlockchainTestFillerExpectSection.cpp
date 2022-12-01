#include "BlockchainTestFillerExpectSection.h"
#include <TestHelper.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
using namespace test::compiler;

namespace test::teststruct
{
BlockchainTestFillerExpectSection::BlockchainTestFillerExpectSection(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "BlockchainTestFillerExpectSection " + _data->getKey(),
            {{"network", {{DataType::Array, DataType::String}, jsonField::Required}},
             {"result", {{DataType::Object}, jsonField::Required}}});

        // get allowed networks for this expect section
        std::set<string> forks;
        parseJsonStrValueIntoSet(_data->atKey("network"), forks);
        ETH_ERROR_REQUIRE_MESSAGE(forks.size() > 0, "Expect section `network` is empty!");

        // Parse >=Frontier into  Frontier, Homestead, ... Constantinople according to current config
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_forks = cfg.translateNetworks(forks);

        convertDecStateToHex((*_data).atKeyPointerUnsafe("result"), solContracts(), StateToHex::NOCOMPILECODE);
        m_result = GCP_SPointer<StateIncomplete>(new StateIncomplete(MOVE(_data, "result")));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerExpectSection parse error: ") + _ex.what());
    }
}

}  // namespace teststruct
