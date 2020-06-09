#include "BlockchainTestFillerExpectSection.h"
#include <TestHelper.h>
#include <retesteth/testStructures/Common.h>

#include <retesteth/Options.h>
namespace test
{
namespace teststruct
{
BlockchainTestFillerExpectSection::BlockchainTestFillerExpectSection(DataObject const& _data)
{
    try
    {
        // get allowed networks for this expect section
        std::set<string> forks;
        parseJsonStrValueIntoSet(_data.atKey("network"), forks);
        ETH_ERROR_REQUIRE_MESSAGE(forks.size() > 0, "Expect section `network` is empty!");

        // Parse >=Frontier into  Frontier, Homestead, ... Constantinople according to current config
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_forks = cfg.translateNetworks(forks);
        m_result = GCP_SPointer<StateIncomplete>(new StateIncomplete(_data.atKey("result"), DataRequier::ALLOWDEC));
        requireJsonFields(_data, "BlockchainTestFillerExpectSection " + _data.getKey(),
            {{"network", {{DataType::Array, DataType::String}, jsonField::Required}},
                {"result", {{DataType::Object}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerExpectSection parse error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
