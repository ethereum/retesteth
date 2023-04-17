#include <retesteth/session/ToolBackend/ToolChainManager.h>
#include <retesteth/testStructures/basetypes.h>
#include <retesteth/Constants.h>
using namespace std;
using namespace test;
using namespace test::debug;
using namespace toolimpl;
namespace fs = boost::filesystem;


namespace  {

mutex g_DifficultyStatic_Access;

// Prepare data for ToolChainManager::test_calculateDifficulty
struct DifficultyStatic
{
    spBlockHeader blockA;
    FH32 loghash = FH32("0x1122334455667788991011121314151617181920212223242526272829303132");
    spState state;
};

}
DifficultyStatic const& prepareEthereumBlockStateTemplate();

VALUE ToolChainManager::test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
    VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber,
    fs::path const& _toolPath, fs::path const& _tmpDir)
{
    DifficultyStatic const& data = prepareEthereumBlockStateTemplate();

    // Constructor has serialization from data.blockA
    EthereumBlockState blockA(data.blockA, data.state, data.loghash);
    EthereumBlockState blockB(data.blockA, data.state, data.loghash);

    BlockHeader& headerA = blockA.headerUnsafe().getContent();
    headerA.setDifficulty(_parentDifficulty);
    if (_blockNumber == 0)
        ETH_ERROR_MESSAGE("ToolChainManager::test_calculateDifficulty calculating difficulty for blocknumber 0!");
    headerA.setNumber(_blockNumber - 1);
    headerA.setTimestamp(_parentTimestamp);

    // Set uncle hash to non empty
    if (_uncleNumber > 0)
        headerA.setUnclesHash(FH32("0x2dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347"));
    else
        headerA.setUnclesHash(FH32(C_EMPTY_LIST_HASH));

    BlockHeader& headerB = blockB.headerUnsafe().getContent();
    headerB.setTimestamp(_currentTimestamp);
    headerB.setNumber(_blockNumber);
    headerB.setParentHash(headerA.hash());

    ToolChain chain(blockA, blockB, _fork, _toolPath, _tmpDir);
    return chain.lastBlock().header()->difficulty();
}

// Prepare data for ToolChainManager::test_calculateDifficulty
DifficultyStatic const& prepareEthereumBlockStateTemplate()
{
    std::lock_guard<std::mutex> lock(g_DifficultyStatic_Access);
    static DifficultyStatic data;
    if (data.blockA.isEmpty())
    {
        spDataObject accountData;
        (*accountData).setKey("0x1122334455667788991011121314151617181920");
        (*accountData)["balance"] = "0x1000";
        (*accountData)["code"] = "0x";
        (*accountData)["nonce"] = "0x1000";
        (*accountData).atKeyPointer("storage") = spDataObject(new DataObject(DataType::Object));
        std::map<FH20, spAccountBase> state = {
            {FH20("0x1122334455667788991011121314151617181920"), spAccountBase(new State::Account(accountData))}};

        DataObject headerAData;
        headerAData["bloom"] =
            "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            "0000000000000000000000000000000000000000000000000000000000";
        headerAData["coinbase"] = "0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba";
        headerAData["extraData"] = "0x00";
        headerAData["gasLimit"] = "0x0f4240";
        headerAData["gasUsed"] = "0x5d70";
        headerAData["hash"] = "0x668c5d6c12a905b58fc597b3146d29b9d12280c1891449b06e19f9edee0fe86f";
        headerAData["mixHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
        headerAData["nonce"] = "0x0000000000000000";
        headerAData["parentHash"] = "0xd86c2d4e6439870452a6c52b71fc6f06d9f98e53c5f9c47acd0ab3c04e73ef7b";
        headerAData["receiptTrie"] = "0xb34b65874cf7cb8358930db57604b2f6610f98c25f2b5822cc25adee7f181ff5";
        headerAData["stateRoot"] = "0xf17157ed407eb909de943044b712ab5db7f48c3557e3a3d8e17b1d8b426a1916";
        headerAData["transactionsTrie"] = "0x174ccc4f3050aca8eb96ee492d7f77c48698de3988b3a422e2bc6974348182c0";
        headerAData["uncleHash"] = C_EMPTY_LIST_HASH;
        headerAData["difficulty"] = "0x131415";
        headerAData["number"] = "0x01";
        headerAData["timestamp"] = "0x00";
        headerAData["baseFeePerGas"] = "0x10";

        data.blockA = spBlockHeader(new BlockHeader1559(headerAData));
        data.state = spState(new State(state));
    }
    return data;
}

