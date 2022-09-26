#include <TestHelper.h>
#include <TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/Options.h>

#include "EthChecks.h"
#include "DifficultyTestFiller.h"

using namespace std;
using namespace test;
using namespace test::teststruct;

DifficultyTestFiller::DifficultyTestFiller(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() >= 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain at least one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("TransactionTestFiller", el->getKey()));
            m_tests.push_back(DifficultyTestInFiller(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}


DifficultyTestInFiller::DifficultyTestInFiller(spDataObject& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "DifficultyTestInFiller " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
             {"network", {{DataType::Array}, jsonField::Required}},
             {"blocknumber", {{DataType::String, DataType::Array}, jsonField::Required}},
             {"timestampDiff", {{DataType::String, DataType::Array}, jsonField::Required}},
             {"parentDifficutly", {{DataType::String, DataType::Array}, jsonField::Required}},
             {"hasUncles", {{DataType::Array}, jsonField::Required}}
            });

        m_name = _data->getKey();
        if (_data->count("_info"))
            m_info = GCP_SPointer<InfoIncomplete>(new InfoIncomplete(MOVE(_data, "_info")));

        std::set<string> forks;
        parseJsonStrValueIntoSet(_data->atKey("network"), forks);
        ETH_ERROR_REQUIRE_MESSAGE(forks.size() > 0, "Difficulty test `network` is empty!");
        // Parse >=Frontier into  Frontier, Homestead, ... Constantinople according to current config
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_networks = cfg.translateNetworks(forks);

        m_blockNumberD = DifficultyRange(_data.getContent().atKeyUnsafe("blocknumber"));
        m_timestampD = DifficultyRange(_data.getContent().atKeyUnsafe("timestampDiff"));
        m_parentDiffD = DifficultyRange(_data.getContent().atKeyUnsafe("parentDifficutly"));

        for (auto const& el : _data->atKey("hasUncles").getSubObjects())
            m_uncles.push_back(el.getCContent().asInt());

        if (m_uncles.size() > 2)
            throw test::UpwardsException("hasUncles size is limited to 2!");
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DifficultyTestFiller convertion error: ") + _ex.what());
    }
}


DifficultyRange::DifficultyRange(DataObject& _data)
{
    if (_data.type() == DataType::Array)
    {
        for (auto& el : _data.getSubObjectsUnsafe())
        {
            mod_valueToCompactEvenHexPrefixed(el.getContent());
            m_vector.push_back(spVALUE(new VALUE(el)));
        }
    }
    else if (_data.type() == DataType::String)
    {
        // parse "blocknumber" : "[100000-200000;12]",
        string const& str = _data.asString();
        size_t posBegin = str.find('[');
        size_t posSeparator = str.find('-');
        size_t posSpace = str.find(';');
        size_t posEnd = str.find(']');

        if (posBegin == string::npos || posSeparator == string::npos ||
            posSpace == string::npos || posEnd == string::npos)
            throw test::UpwardsException("DifficultyRange `" + _data.getKey() + "` expected to be in format '[x-y;z]");

        DataObject dBegin(str.substr(posBegin + 1, posSeparator - posBegin - 1));
        DataObject dEnd(str.substr(posSeparator + 1, posSpace - posSeparator - 1));
        DataObject dStep(str.substr(posSpace + 1, posEnd - posSpace - 1));
        mod_valueToCompactEvenHexPrefixed(dBegin);
        mod_valueToCompactEvenHexPrefixed(dEnd);
        mod_valueToCompactEvenHexPrefixed(dStep);

        VALUE vBegin(dBegin);
        VALUE vEnd(dEnd);
        VALUE vStep(dStep);

        // Sanity checks
        if (vStep == 0)
            throw test::UpwardsException("Parsing DifficultyRange `" + _data.getKey() + "` the step is zero: " + vStep.asDecString());
        if (vStep > vEnd)
            throw test::UpwardsException("Parsing DifficultyRange `" + _data.getKey() + "` the step is too big: " + vStep.asDecString());
        if (vBegin > vEnd)
            throw test::UpwardsException("Parsing DifficultyRange `" + _data.getKey() + "` begin > end!");

        auto const steps = ((vEnd - vBegin) / dStep);
        if (steps > 100)
            throw test::UpwardsException("Parsing DifficultyRange `" + _data.getKey() + "` has too many steps (>100): " + steps.asDecString());

        for (VALUE i = vBegin; i <= vEnd; i+= vStep)
            m_vector.push_back(spVALUE(new VALUE(i.asBigInt())));
    }
    else
        throw test::UpwardsException("Parsing DifficultyRange `" + _data.getKey() + "` has unexpected datatype!");
}
