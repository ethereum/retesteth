#include "StateTestFillerExpectSection.h"
#include <TestHelper.h>
#include <retesteth/testStructures/Common.h>

#include <retesteth/Options.h>
namespace test
{
namespace teststruct
{
// Look at expect section data indexes filter and try to replace string values
// into indexes of transaction data array (searching by label)
spDataObject ReplaceValueToIndexesInDataList(spStateTestFillerTransaction const& _gtr, DataObject const& _dataList)
{
    auto findIndexOfValueAndReplace = [&_gtr](DataObject& _data) {
        if (_data.type() == DataType::String)
        {
            size_t i = 0;
            std::vector<int> indexes;
            const vector<StateTestTransactionBase::Databox>& dVector = _gtr->databoxVector();
            for (auto const& el : dVector)
            {
                if (el.m_dataLabel == _data.asString())
                    indexes.push_back(i);
                i++;
            }

            if (indexes.size() == 1)
            {
                _data.clear();
                _data.setInt(indexes.at(0));
            }
            if (indexes.size() > 1)
            {
                _data.clear();
                for (auto const& el : indexes)
                    _data.addArrayObject(spDataObject(new DataObject(el)));
            }
            if (indexes.size() == 0 && _data.asString().find(":label") != string::npos)
                ETH_ERROR_MESSAGE("Label not found in tx data: `" + _data.asString() + "`");
        }
    };
    // Check if dataIndexes contain values of transaction data vector
    // Find those values and vector and replace by indexes
    spDataObject dataIndexes(new DataObject());
    (*dataIndexes).copyFrom(_dataList);
    if (dataIndexes->type() == DataType::Array)
    {
        spDataObject updatedDataIndexes(new DataObject());
        for (auto& el : (*dataIndexes).getSubObjectsUnsafe())
        {
            // try to replace `el` with data indexes from transaction
            // in case `el` provided is a transaction value in dataInd array
            if (el->type() == DataType::String)
            {
                spDataObject elCopy(new DataObject());
                (*elCopy).copyFrom(el);
                findIndexOfValueAndReplace(elCopy.getContent());
                if (elCopy->type() == DataType::Integer)
                {
                    el.getContent().replace(elCopy);
                    (*updatedDataIndexes).addArrayObject(elCopy);
                }
                else if (elCopy->type() == DataType::Array)
                {
                    for (auto const& el2 : elCopy->getSubObjects())
                        (*updatedDataIndexes).addArrayObject(el2);
                }
                else
                    (*updatedDataIndexes).addArrayObject(el);
            }
            else
                (*updatedDataIndexes).addArrayObject(el);
        }
        dataIndexes = updatedDataIndexes;
    }
    else if (dataIndexes->type() == DataType::String)
        findIndexOfValueAndReplace(dataIndexes.getContent());
    (*dataIndexes).setKey(_dataList.getKey());
    return dataIndexes;
}

StateTestFillerExpectSection::StateTestFillerExpectSection(DataObject const& _data, spStateTestFillerTransaction const& _gtr)
{
    try
    {
        m_initialData = spDataObject(new DataObject());
        (*m_initialData).atKeyPointer("indexes").getContent().copyFrom(_data.atKey("indexes"));
        (*m_initialData).atKeyPointer("network").getContent().copyFrom(_data.atKey("network"));

        spDataObject dataIndexes = ReplaceValueToIndexesInDataList(_gtr, _data.atKey("indexes").atKey("data"));
        parseJsonIntValueIntoSet(dataIndexes, m_dataInd);
        parseJsonIntValueIntoSet(_data.atKey("indexes").atKey("gas"), m_gasInd);
        parseJsonIntValueIntoSet(_data.atKey("indexes").atKey("value"), m_valInd);

        ETH_ERROR_REQUIRE_MESSAGE(m_dataInd.size() > 0, "Expect section `indexes::data` is empty!");
        ETH_ERROR_REQUIRE_MESSAGE(m_gasInd.size() > 0, "Expect section `indexes::gas` is empty!");
        ETH_ERROR_REQUIRE_MESSAGE(m_valInd.size() > 0, "Expect section `indexes::value` is empty!");

        // get allowed networks for this expect section
        std::set<string> forks;
        parseJsonStrValueIntoSet(_data.atKey("network"), forks);
        ETH_ERROR_REQUIRE_MESSAGE(forks.size() > 0, "Expect section `network` is empty!");

        // Parse >=Frontier into  Frontier, Homestead, ... Constantinople according to current config
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_forks = cfg.translateNetworks(forks);
        m_result = GCP_SPointer<StateIncomplete>(new StateIncomplete(_data.atKey("result"), DataRequier::ALLOWDEC));

        if (_data.count("expectException"))
            readExpectExceptions(_data.atKey("expectException"), m_expectExceptions);

        requireJsonFields(_data, "StateTestFillerExpectSection " + _data.getKey(),
            {{"indexes", {{DataType::Object}, jsonField::Required}},
             {"network", {{DataType::Array}, jsonField::Required}},
             {"expectException", {{DataType::Object}, jsonField::Optional}},
             {"result", {{DataType::Object}, jsonField::Required}}});
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerExpectSection parse error: ") + _ex.what() + "\n" + _data.asJson());
    }
}

bool StateTestFillerExpectSection::checkIndexes(size_t _dInd, size_t _gInd, size_t _vInd) const
{
    if ((m_dataInd.count(_dInd) || m_dataInd.count(-1)) &&
        (m_gasInd.count(_gInd) || m_gasInd.count(-1)) &&
        (m_valInd.count(_vInd) || m_valInd.count(-1)))
        return true;
    return false;
}

void StateTestFillerExpectSection::correctMiningReward(FH20 const& _coinbase, VALUE const& _reward)
{
    m_result.getContent().correctMiningReward(_coinbase, _reward);
}

}  // namespace teststruct
}  // namespace test
