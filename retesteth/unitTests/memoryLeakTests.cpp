#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/unit_test.hpp>

#include <retesteth/dataObject/ConvertFile.h>
#include <retesteth/testStructures/structures.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::compiler;

BOOST_FIXTURE_TEST_SUITE(MemoryLeak, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(recalculateHash)
{
    string str = R"(
    {
        "bloom" : "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "coinbase" : "0x8888f1f195afa192cfee860698584c030f4c9db1",
        "difficulty" : "0x020000",
        "extraData" : "0x42",
        "gasLimit" : "0x7fffffffffffffff",
        "gasUsed" : "0x5208",
        "hash" : "0x847536e7d3904bda73f001fe77aca7367b7c315bef7c700f61f2e05f5b471f69",
        "mixHash" : "0x0000000000000000000000000000000000000000000000000000000000000000",
        "nonce" : "0x0000000000000000",
        "number" : "0x01",
        "parentHash" : "0xef2e504cf630cee6a2dc9005096c1b069c480e94d0e7ba0ef0b5265ab63d5ddb",
        "receiptTrie" : "0x056b23fbba480696b65fe5a59b8f2148a1299103c4f57df839233af2cf4ca2d2",
        "stateRoot" : "0xaf6f8d5679bb2df0688ff6067ed389928ca945569e5e22b3433fce09bb8f5e54",
        "timestamp" : "0x54c99069",
        "transactionsTrie" : "0xc33a0be2fd6c2ee1701d2adbba07b9eb9d7e3e881f2b5cae34d3379f2ce31301",
        "uncleHash" : "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347",
        "baseFeePerGas" : "0x0e"
    })";
    DataObject data = dataobject::ConvertJsoncppStringToData(str);
    spBlockHeader1559 header(new BlockHeader1559(data));
    header.getContent().recalculateHash();
}

BOOST_AUTO_TEST_CASE(smartPointer)
{
    spVALUE A(new VALUE(12));
    spVALUE B;

    B = A;
    BOOST_CHECK(B->asBigInt() == 12);
    BOOST_CHECK(B->asBigInt() == A->asBigInt());

    B.getContent() += 12;
    BOOST_CHECK(A->asBigInt() == 24);
}

BOOST_AUTO_TEST_CASE(smartPointerVector)
{
    spVALUE A(new VALUE(12));
    std::vector<spVALUE> vec;
    vec.push_back(A);

    BOOST_CHECK(vec.at(0)->asBigInt() == 12);
    A.getContent() += 12;

    BOOST_CHECK(vec.at(0)->asBigInt() == 24);

    spVALUE B(new VALUE(13));
    vec.insert(vec.begin(), B);

    BOOST_CHECK(vec.at(0)->asBigInt() == 13);
    BOOST_CHECK(vec.at(1)->asBigInt() == 24);
    A.getContent() += 12;
    BOOST_CHECK(vec.at(1)->asBigInt() == 36);
}


BOOST_AUTO_TEST_SUITE_END()
