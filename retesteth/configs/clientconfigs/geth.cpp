#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

string const geth_config = R"({
    "name" : "Ethereum GO on TCP",
    "socketType" : "tcp",
    "socketAddress" : [
        "127.0.0.1:8545",
        "127.0.0.1:8546",
        "127.0.0.1:8547",
        "127.0.0.1:8548",
        "127.0.0.1:8549",
        "127.0.0.1:8550",
        "127.0.0.1:8551",
        "127.0.0.1:8552"
    ],
    "initializeTime" : "1",
    "forks" : [
      "Frontier",
      "Homestead",
      "EIP150",
      "EIP158",
      "Byzantium",
      "Constantinople",
      "ConstantinopleFix",
      "Istanbul"
    ],
    "additionalForks" : [
      "FrontierToHomesteadAt5",
      "HomesteadToEIP150At5",
      "EIP158ToByzantiumAt5",
      "HomesteadToDaoAt5",
      "ByzantiumToConstantinopleFixAt5"
    ],
    "exceptions" : {
        "ExtraDataTooBig" : "extra-data too long",
        "InvalidDifficulty" : "invalid difficulty",
        "InvalidDifficulty2" : "invalid difficulty",
        "InvalidDifficulty_TooLarge" : "invalid difficulty",
        "InvalidGasLimit" : "invalid gasLimit:",
        "TooMuchGasUsed" : "invalid gasUsed:",
        "TooMuchGasUsed2" : "invalid gasUsed:",
        "InvalidNumber" : "invalid block number",
        "InvalidTimestampEqualParent" : "timestamp equals parent's",
        "InvalidTimestampOlderParent" : "timestamp older than parent",
        "InvalidLogBloom" : "invalid bloom (remote:",
        "InvalidStateRoot" : "invalid merkle root (remote:",
        "InvalidGasLimit2" : "invalid gas limit:",
        "InvalidGasUsed" : "invalid gas used (remote:",
        "InvalidGasUsed2" : "invalid gas used (remote:",
        "InvalidBlockMixHash" : "invalid mix digest",
        "InvalidBlockNonce" : "",
        "UnknownParent" : "unknown ancestor",
        "UnknownParent2" : "unknown ancestor",
        "InvalidReceiptsStateRoot" : "invalid receipt root hash (remote:",
        "InvalidTransactionsRoot" : "transaction root hash mismatch: have",
        "InvalidUnclesHash" : "uncle root hash mismatch: have",
        "InvalidUncleParentHash" : "uncle's parent is not ancestor",
        "UncleInChain" : "duplicate uncle",
        "UncleIsAncestor" : "uncle is ancestor",
        "UncleIsBrother" : "duplicate uncle",
        "UncleParentIsNotAncestor" : "uncle's parent is not ancestor",
        "TooManyUncles" : "too many uncles",
        "OutOfGas" : "out of gas",
        "IntrinsicGas" : "intrinsic gas too low",
        "ExtraDataIncorrectDAO" : "bad DAO pro-fork extra-data",
        "InvalidTransactionVRS" : "invalid transaction v, r, s values",
        "BLOCKHEADER_VALUE_TOOLARGE" : "Blockheader parse error: VALUE  >u256",
        "TRANSACTION_VALUE_TOOLARGE" : "transaction root hash mismatch: have",
        "TRANSACTION_VALUE_TOOSHORT" : "transaction root hash mismatch: have",
        "OVERSIZE_RLP" : "Error importing raw rlp block: OversizeRLP",
        "RLP_InputContainsMoreThanOneValue" : "rlp: input contains more than one value",
        "RLP_VALUESIZE_MORE_AVAILABLEINPUTLENGTH" : "rlp: value size exceeds available input length",
        "RLP_ELEMENT_LARGER_CONTAININGLIST_UNDERSIZE" : "rlp: element is larger than containing list",
        "RLP_ELEMENT_LARGER_CONTAININGLIST_OVERSIZE" : "rlp: element is larger than containing list",
        "RLP_ExpectedInputList_EXTBLOCK" : "rlp: expected input list for types.extblock",
        "RLP_InvalidArg0_UNMARSHAL_BYTES" : "invalid argument 0: json: cannot unmarshal invalid hex string into Go value of type hexutil.Bytes",
        "RLP_ExpectedInputList_HEADER_DECODEINTO_BLOCK_EXTBLOCK" : "rlp: expected input list for types.Header, decoding into (types.Block)(types.extblock).Header",
        "RLP_InputList_TooManyElements_HEADER_DECODEINTO_BLOCK_EXTBLOCK_HEADER" : "rlp: input list has too many elements for types.Header, decoding into (types.Block)(types.extblock).Header",
        "RLP_InputList_TooManyElements_TXDATA_DECODEINTO_BLOCK_EXTBLOCK_TXS0" : "rlp: input list has too many elements for types.txdata, decoding into (types.Block)(types.extblock).Txs[0]",
        "RLP_InputString_TooShort_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE" : "rlp: input string too short for common.Address, decoding into (types.Block)(types.extblock).Header.Coinbase",
        "RLP_InputString_TooShort_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE2" : "rlp: input string too short for common.Address, decoding into (types.Block)(types.extblock).Header.Coinbase",
        "RLP_InputString_TooShort_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TXS0_RECIPIENT" : "rlp: input string too short for common.Address, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).Recipient",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.Root",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT2" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.Root",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.MixDigest",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST2" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.MixDigest",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.ParentHash",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH2" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.ParentHash",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.ReceiptHash",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH2" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.ReceiptHash",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.TxHash",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.UncleHash",
        "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH2" : "rlp: input string too long for common.Hash, decoding into (types.Block)(types.extblock).Header.UncleHash",
        "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT" : "rlp: input string too long for uint64, decoding into (types.Block)(types.extblock).Header.GasLimit",
        "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED" : "rlp: input string too long for uint64, decoding into (types.Block)(types.extblock).Header.GasUsed",
        "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME" : "rlp: input string too long for uint64, decoding into (types.Block)(types.extblock).Header.Time",
        "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT" : "rlp: input string too long for uint64, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).GasLimit",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.ReceiptHash",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH2" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.ReceiptHash",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.Root",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.MixDigest",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST2" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.MixDigest",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.ParentHash",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH2" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.ParentHash",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.UncleHash",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH2" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.UncleHash",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.TxHash",
        "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH2" : "rlp: input string too short for common.Hash, decoding into (types.Block)(types.extblock).Header.TxHash",
        "RLP_InputString_TooShort_BLOOM_DECODEINTO_BLOCK_EXTBLOCK_HEADER_BLOOM" : "rlp: input string too short for types.Bloom, decoding into (types.Block)(types.extblock).Header.Bloom",
        "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_DIFFICULTY" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Block)(types.extblock).Header.Difficulty",
        "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_DIFFICULTY2" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Block)(types.extblock).Header.Difficulty",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Header.GasLimit",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT2" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Header.GasLimit",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Header.GasUsed",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED2" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Header.GasUsed",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Header.Time",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME2" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Header.Time",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).GasLimit",
        "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT2" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).GasLimit",
        "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NUMBER" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Block)(types.extblock).Header.Number",
        "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NUMBER2" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Block)(types.extblock).Header.Number",
        "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_PRICE" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).Price",
        "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_R" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).R",
        "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_S" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).S",
        "RLP_InputString_TooLong_BLOOM_DECODEINTO_BLOCK_EXTBLOCK_HEADER_BLOOM" : "rlp: input string too long for types.Bloom, decoding into (types.Block)(types.extblock).Header.Bloom",
        "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH" : "rlp: expected input string or byte for common.Hash, decoding into (types.Block)(types.extblock).Header.ParentHash",
        "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH" : "rlp: expected input string or byte for common.Hash, decoding into (types.Block)(types.extblock).Header.ReceiptHash",
        "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT" : "rlp: expected input string or byte for common.Hash, decoding into (types.Block)(types.extblock).Header.Root",
        "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST" : "rlp: expected input string or byte for common.Hash, decoding into (types.Block)(types.extblock).Header.MixDigest",
        "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH" : "rlp: expected input string or byte for common.Hash, decoding into (types.Block)(types.extblock).Header.TxHash",
        "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH" : "rlp: expected input string or byte for common.Hash, decoding into (types.Block)(types.extblock).Header.UncleHash",
        "RLP_ExpectedInputString_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE" : "rlp: expected input string or byte for common.Address, decoding into (types.Block)(types.extblock).Header.Coinbase",
        "RLP_ExpectedInputString_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TX0_RECIPIENT" : "rlp: expected input string or byte for common.Address, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).Recipient",
        "RLP_InputString_TooLong_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE" : "rlp: input string too long for common.Address, decoding into (types.Block)(types.extblock).Header.Coinbase",
        "RLP_InputString_TooLong_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TXS0_RECIPIENT" : "rlp: input string too long for common.Address, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).Recipient",
        "RLP_ExpectedInputString_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_DIFFICULTY" : "rlp: expected input string or byte for *big.Int, decoding into (types.Block)(types.extblock).Header.Difficulty",
        "RLP_ExpectedInputString_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXR" : "rlp: expected input string or byte for *big.Int, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).R",
        "RLP_ExpectedInputString_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXS" : "rlp: expected input string or byte for *big.Int, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).S",
        "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT" : "rlp: expected input string or byte for uint64, decoding into (types.Block)(types.extblock).Header.GasLimit",
        "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED" : "rlp: expected input string or byte for uint64, decoding into (types.Block)(types.extblock).Header.GasUsed",
        "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME" : "rlp: expected input string or byte for uint64, decoding into (types.Block)(types.extblock).Header.Time",
        "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT" : "rlp: expected input string or byte for uint64, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).GasLimit",
        "RLP_ExpectedInputString_NONCE_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NONCE" : "rlp: expected input string or byte for types.BlockNonce, decoding into (types.Block)(types.extblock).Header.Nonce",
        "RLP_ExpectedInputString_UINT8_DECODEINTO_BLOCK_EXTBLOCK_TXS0_PAYLOAD" : "rlp: expected input string or byte for []uint8, decoding into (types.Block)(types.extblock).Txs[0](types.txdata).Payload",
        "RLP_InputString_TooLong_BLOCKNONCE_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NONCE" : "rlp: input string too long for types.BlockNonce, decoding into (types.Block)(types.extblock).Header.Nonce",
        "RLP_InputString_TooLong_BLOCKNONCE_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NONCE2" : "rlp: input string too long for types.BlockNonce, decoding into (types.Block)(types.extblock).Header.Nonce",
        "RLP_NonCanonical_SizeInfo_EXTBLOCK" : "rlp: non-canonical size information for types.extblock",
        "RLP_ExpectedInputList_TRANSACTION_DECODEINTO_BLOCK_EXTBLOCK_TXS" : "rlp: expected input list for []*types.Transaction, decoding into (types.Block)(types.extblock).Txs",
        "RLP_ExpectedInputList_HEADER_DECODEINTO_BLOCK_EXTBLOCK_UNCLES" : "rlp: expected input list for []*types.Header, decoding into (types.Block)(types.extblock).Uncles",
        "RLP_ExpectedInputList_TXDATA_DECODEINTO_BLOCK_EXTBLOCK_TXS0" : "rlp: expected input list for types.txdata, decoding into (types.Block)(types.extblock).Txs[0]"
    }
})";

string const geth_start = R"(#!/bin/sh
threads=1
if [ "${1:-0}" -gt 1 ]
then
  threads=$1
fi

i=0
while [ "$i" -lt $threads ]; do
    geth retesteth --rpcport $((8545+$i)) &
    i=$(( i + 1 ))
done
)";

string const geth_stop = R"(#!/bin/sh
killall geth
)";

gethcfg::gethcfg()
{
    {
        DataObject obj;
        obj["path"] = "default/config";
        obj["content"] = geth_config;
        map_configs.addArrayObject(obj);
    }
    {
        DataObject obj;
        obj["exec"] = true;
        obj["path"] = "default/start.sh";
        obj["content"] = geth_start;
        map_configs.addArrayObject(obj);
    }
    {
        DataObject obj;
        obj["exec"] = true;
        obj["path"] = "default/stop.sh";
        obj["content"] = geth_stop;
        map_configs.addArrayObject(obj);
    }

    {
        DataObject obj;
        obj["path"] = "geth/config";
        obj["content"] = geth_config;
        map_configs.addArrayObject(obj);
    }
    {
        DataObject obj;
        obj["exec"] = true;
        obj["path"] = "geth/start.sh";
        obj["content"] = geth_start;
        map_configs.addArrayObject(obj);
    }
    {
        DataObject obj;
        obj["exec"] = true;
        obj["path"] = "geth/stop.sh";
        obj["content"] = geth_stop;
        map_configs.addArrayObject(obj);
    }
}
