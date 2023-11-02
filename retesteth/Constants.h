#pragma once
#include <string>
#include <testStructures/basetypes.h>

namespace test {

extern const std::string C_WITHDRAWALS_EMPTY_ROOT;
extern const std::string C_EMPTY_LIST_HASH;
extern const std::string C_EMPTY_STR;
extern const std::string C_BIGINT_PREFIX;

namespace teststruct {

extern const FH8 C_FH8_ZERO;
extern const FH20 C_FH20_ZERO;
extern const FH32 C_FH32_ZERO;
extern const FH256 C_FH256_ZERO;

namespace constnames {

extern const std::string c_author;
extern const std::string c_miner;
extern const std::string c_coinbase;
extern const std::string c_difficulty;
extern const std::string c_extraData;
extern const std::string c_gasLimit;
extern const std::string c_gasUsed;
extern const std::string c_hash;
extern const std::string c_logsBloom;
extern const std::string c_bloom;
extern const std::string c_mixHash;
extern const std::string c_nonce;
extern const std::string c_number;
extern const std::string c_parentHash;
extern const std::string c_receiptsRoot;
extern const std::string c_receiptTrie;
extern const std::string c_sha3Uncles;
extern const std::string c_uncleHash;
extern const std::string c_stateRoot;
extern const std::string c_timestamp;
extern const std::string c_transactionsRoot;
extern const std::string c_transactionsTrie;
extern const std::string c_baseFeePerGas;
extern const std::string c_withdrawalsRoot;
extern const std::string c_parentBeaconBlockRoot;
extern const std::string c_excessBlobGas;
extern const std::string c_blobGasUsed;
extern const std::string c_currentExcessBlobGas;
extern const std::string c_currentBeaconRoot;
extern const std::string c_parentExcessBlobGas;
extern const std::string c_currentBlobGasUsed;
extern const std::string c_parentBlobGasUsed;

// Transactions
extern const std::string c_data;
extern const std::string c_gasPrice;
extern const std::string c_value;
extern const std::string c_to;
extern const std::string c_secretKey;
extern const std::string c_sender;
extern const std::string c_v;
extern const std::string c_r;
extern const std::string c_s;
extern const std::string c_maxFeePerGas;
extern const std::string c_maxPriorityFeePerGas;
extern const std::string c_maxFeePerBlobGas;
extern const std::string c_blobVersionedHashes;
extern const std::string c_accessLists;
extern const std::string c_accessList;

// State
extern const std::string c_code;
extern const std::string c_balance;
extern const std::string c_storage;
extern const std::string c_transactionHash;
extern const std::string c_blockHash;

}}}
