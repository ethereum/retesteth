#include "Constants.h"
using namespace test;
using namespace std;

namespace test {

const string C_WITHDRAWALS_EMPTY_ROOT = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
const string C_EMPTY_STR = string();
const string C_EMPTY_LIST_HASH = "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
const string C_BIGINT_PREFIX = "0x:bigint ";

namespace teststruct {

const FH8 C_FH8_ZERO("0x0000000000000000");
const FH20 C_FH20_ZERO("0x0000000000000000000000000000000000000000");
const FH32 C_FH32_ZERO("0x0000000000000000000000000000000000000000000000000000000000000000");
const FH256 C_FH256_ZERO(
    "0x0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000000000000000000000000000000");

namespace constnames {

const string c_author = "author";
const string c_miner = "miner";
const string c_coinbase = "coinbase";
const string c_difficulty = "difficulty";
const string c_extraData = "extraData";
const string c_gasLimit = "gasLimit";
const string c_gasUsed = "gasUsed";
const string c_hash = "hash";
const string c_logsBloom = "logsBloom";
const string c_bloom = "bloom";
const string c_mixHash = "mixHash";
const string c_nonce = "nonce";
const string c_number = "number";
const string c_parentHash = "parentHash";
const string c_receiptsRoot = "receiptsRoot";
const string c_receiptTrie = "receiptTrie";
const string c_sha3Uncles = "sha3Uncles";
const string c_uncleHash = "uncleHash";
const string c_stateRoot = "stateRoot";
const string c_timestamp = "timestamp";
const string c_transactionsRoot = "transactionsRoot";
const string c_transactionsTrie = "transactionsTrie";
const string c_baseFeePerGas = "baseFeePerGas";
const string c_withdrawalsRoot = "withdrawalsRoot";

// Transactions
const string c_data = "data";
const std::string c_gasPrice = "gasPrice";
const std::string c_value = "value";
const std::string c_to = "to";
const std::string c_secretKey = "secretKey";
const std::string c_sender = "sender";
const std::string c_v = "v";
const std::string c_r = "r";
const std::string c_s = "s";

}}}

