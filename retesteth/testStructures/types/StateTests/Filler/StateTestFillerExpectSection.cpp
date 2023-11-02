#include "StateTestFillerExpectSection.h"
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
using namespace std;
using namespace test::compiler;

namespace test::teststruct
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
                    indexes.emplace_back(i);
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
    spDataObject dataIndexes;
    (*dataIndexes).copyFrom(_dataList);
    if (dataIndexes->type() == DataType::Array)
    {
        spDataObject updatedDataIndexes;
        for (auto& el : (*dataIndexes).getSubObjectsUnsafe())
        {
            // try to replace `el` with data indexes from transaction
            // in case `el` provided is a transaction value in dataInd array
            if (el->type() == DataType::String)
            {
                spDataObject elCopy; //TODO copy???
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

StateTestFillerExpectSection::StateTestFillerExpectSection(spDataObjectMove _data, spStateTestFillerTransaction const& _gtr)
{
    try
    {
        m_initialData = _data.getPointer();
        REQUIRE_JSONFIELDS(m_initialData, "StateTestFillerExpectSection " + m_initialData->getKey(),
            {{"indexes", {{DataType::Object}, jsonField::Optional}},
                {"network", {{DataType::Array}, jsonField::Required}},
                {"expectException", {{DataType::Object}, jsonField::Optional}},
                {"result", {{DataType::Object}, jsonField::Required}}});

        parseExpectSectionIndexes(_gtr);


        // get allowed networks for this expect section
        std::set<string> forks;
        parseJsonStrValueIntoSet(m_initialData->atKey("network"), forks);
        ETH_ERROR_REQUIRE_MESSAGE(forks.size() > 0, "Expect section `network` is empty!");

        // Parse >=Frontier into  Frontier, Homestead, ... Constantinople according to current config
        ClientConfig const& cfg = Options::get().getDynamicOptions().getCurrentConfig();
        m_forks = cfg.translateNetworks(forks);

        auto& result = (*m_initialData).atKeyPointerUnsafe("result");
        result.getContent().performModifier(mod_changeValueAnyToBigint00);
        convertDecStateToHex(result, solContracts(), StateToHex::NOCOMPILECODE);
        m_result = GCP_SPointer<StateIncomplete>(new StateIncomplete(MOVE(m_initialData, "result")));

        if (m_initialData->count("expectException"))
            readExpectExceptions(m_initialData->atKey("expectException"), m_expectExceptions);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestFillerExpectSection parse error: ") + _ex.what() + "\n" + m_initialData->asJson());
    }
}

bool StateTestFillerExpectSection::checkIndexes(int _dInd, int _gInd, int _vInd) const
{
    if ((m_dataInd.count(_dInd) || m_dataInd.count(-1) || _dInd == -1) &&
        (m_gasInd.count(_gInd) || m_gasInd.count(-1) || _gInd == -1) &&
        (m_valInd.count(_vInd) || m_valInd.count(-1) || _vInd == -1))
        return true;
    return false;
}

void StateTestFillerExpectSection::correctMiningReward(FH20 const& _coinbase, VALUE const& _reward)
{
    m_result.getContent().correctMiningReward(_coinbase, _reward);
}

std::string const& StateTestFillerExpectSection::getExpectException(FORK const& _net) const
{
    if (m_expectExceptions.count(_net))
        return m_expectExceptions.at(_net);
    return C_EMPTY_STR;
}

void StateTestFillerExpectSection::parseExpectSectionIndexes(spStateTestFillerTransaction const& _gtr)
{
    if (m_initialData->count("indexes"))
    {
        if (m_initialData->atKey("indexes").count("data"))
        {
            spDataObject dataIndexes = ReplaceValueToIndexesInDataList(_gtr, m_initialData->atKey("indexes").atKey("data"));
            parseJsonIntValueIntoSet(dataIndexes, m_dataInd);
        }
        else
            m_dataInd.emplace(-1);

        if (m_initialData->atKey("indexes").count("gas"))
            parseJsonIntValueIntoSet(m_initialData->atKey("indexes").atKey("gas"), m_gasInd);
        else
            m_gasInd.emplace(-1);

        if (m_initialData->atKey("indexes").count("value"))
            parseJsonIntValueIntoSet(m_initialData->atKey("indexes").atKey("value"), m_valInd);
        else
            m_valInd.emplace(-1);
    }
    else
    {
        m_dataInd.emplace(-1);
        m_gasInd.emplace(-1);
        m_valInd.emplace(-1);
    }

    ETH_ERROR_REQUIRE_MESSAGE(m_dataInd.size() > 0, "Expect section `indexes::data` is empty!");
    ETH_ERROR_REQUIRE_MESSAGE(m_gasInd.size() > 0, "Expect section `indexes::gas` is empty!");
    ETH_ERROR_REQUIRE_MESSAGE(m_valInd.size() > 0, "Expect section `indexes::value` is empty!");
}

}  // namespace teststruct
