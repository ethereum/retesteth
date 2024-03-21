#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{

genevmonecfg::genevmonecfg()
{

string const evmone_config = R"({
    "name" : "EVMONE on StateTool",
    "socketType" : "tranition-tool",
    "socketAddress" : "start.sh",
    "initializeTime" : "0",
    "checkDifficulty" : true,
    "calculateDifficulty" : true,
    "checkBasefee" : true,
    "calculateBasefee" : false,
    "checkLogsHash" : true,
    "support1559" : true,
    "supportBigint" : false,
    "transactionsAsJson" : true,
    "tmpDir" : "/dev/shm",
    "defaultChainID" : 1,
    "customCompilers" : {
        ":yul" : "yul.sh"
    },
    "forks" : [
        "Frontier",
        "Homestead",
        "EIP150",
        "EIP158",
        "Byzantium",
        "Constantinople",
        "ConstantinopleFix",
        "Istanbul",
        "Berlin",
        "London",
        "Paris",
        "Shanghai",
        "Cancun",
        "Prague"
    ],
    "additionalForks" : [
        "FrontierToHomesteadAt5",
        "HomesteadToEIP150At5",
        "EIP158ToByzantiumAt5",
        "ByzantiumToConstantinopleFixAt5",
        "BerlinToLondonAt5",
        "ArrowGlacier",
        "ArrowGlacierToParisAtDiffC0000",
        "GrayGlacier",
        "ParisToShanghaiAtTime15k",
        "ShanghaiToCancunAtTime15k"
    ],
    "fillerSkipForks" : [
        "HomesteadToDaoAt5"
    ],
    "exceptions" : {
      "PYSPECS_EXCEPTIONS" : "",
      "TransactionException.INSUFFICIENT_ACCOUNT_FUNDS" : "insufficient funds for gas * price + value",
      "TransactionException.INSUFFICIENT_MAX_FEE_PER_GAS" : "max fee per gas less than block base fee",
      "TransactionException.TYPE_3_TX_ZERO_BLOBS" : "blob transaction missing blob hashes",
      "TransactionException.TYPE_3_TX_INVALID_BLOB_VERSIONED_HASH" : "has invalid hash version",
      "TransactionException.TYPE_3_TX_PRE_FORK|TransactionException.TYPE_3_TX_ZERO_BLOBS" : "blob tx used but field env.ExcessBlobGas missing",
      "TransactionException.TYPE_3_TX_PRE_FORK" : "blob tx used but field env.ExcessBlobGas missing",
      "TransactionException.TYPE_3_TX_BLOB_COUNT_EXCEEDED" : "would exceed maximum",
      "TransactionException.TYPE_3_TX_CONTRACT_CREATION" : "input string too short for common.Address",
      "TransactionException.TYPE_3_TX_MAX_BLOB_GAS_ALLOWANCE_EXCEEDED" : "Block has invalid number of blobs in txs >=7! would exceed maximum",
      "TransactionException.INSUFFICIENT_MAX_FEE_PER_BLOB_GAS" : "blob tx used but field env.ExcessBlobGas missing",
      "TransactionException.INTRINSIC_GAS_TOO_LOW" : "intrinsic gas too low",
      "TransactionException.INITCODE_SIZE_EXCEEDED" : "max initcode size exceeded",
      "BlockException.INCORRECT_EXCESS_BLOB_GAS" : "Error in field: excessBlobGas",
      "BlockException.INCORRECT_BLOB_GAS_USED" : "Error in field: blobGasUsed",
      "BlockException.BLOB_GAS_USED_ABOVE_LIMIT|BlockException.INCORRECT_BLOB_GAS_USED" : "Error in field: blobGasUsed",
      "BlockException.INCORRECT_BLOCK_FORMAT" : "[retesteth]: Error importing raw rlp block: readBlockHeader(RLP): unknown block type!",

      "Transaction without funds" : "insufficient funds for gas * price + value",
      "AddressTooShort" : "input string too short for common.Address",
      "AddressTooLong" : "rlp: input string too long for common.Address, decoding into (types.Transaction)(types.LegacyTx).To",
      "NonceMax" : "nonce exceeds 2^64-1",
      "NonceTooLong" : "rlp: input string too long for uint64, decoding into (types.Transaction)(types.LegacyTx).Nonce",
      "InvalidVRS" : "invalid transaction v, r, s values",
      "InvalidV" : "rlp: expected input string or byte for *big.Int, decoding into (types.Transaction)(types.LegacyTx).V",
      "InvalidR" : "rlp: expected input string or byte for *big.Int, decoding into (types.Transaction)(types.LegacyTx).R",
      "InvalidS" : "rlp: expected input string or byte for *big.Int, decoding into (types.Transaction)(types.LegacyTx).S",
      "InvalidChainID" : "invalid chain id for signer",
      "ECRecoveryFail" : "recovery failed",
      "ExtraDataTooBig" : "Header extraData > 32 bytes",
      "InvalidData" : "rlp: expected input string or byte for []uint8, decoding into (types.Transaction)(types.LegacyTx).Data",
      "InvalidDifficulty" : "Invalid difficulty:",
      "InvalidDifficulty2" : "Error in field: difficulty",
      "InvalidWithdrawals" : "Error in field: withdrawalsRoot",
      "InvalidDifficulty_TooLarge" : "Blockheader parse error: VALUE  >u256",
      "InvalidGasLimit" : "Header gasLimit > 0x7fffffffffffffff",
      "InvalidGasLimit2" : "Invalid gaslimit:",
      "InvalidGasLimit3" : "GasLimit must be < 0x7fffffffffffffff",
      "InvalidGasLimit4" : "rlp: input string too long for uint64, decoding into (types.Transaction)(types.LegacyTx).Gas",
      "InvalidGasLimit5" : "rlp: expected input string or byte for uint64, decoding into (types.Transaction)(types.LegacyTx).Gas",
      "InvalidValue" : "value exceeds 256 bits",
      "InvalidGasPrice" : "gasPrice exceeds 256 bits",
      "InvalidMaxPriorityFeePerGas" : "maxPriorityFeePerGas exceeds 256 bits",
      "InvalidMaxFeePerGas" : "maxFeePerGas exceeds 256 bits",
      "InvalidNonce" : "rlp: expected input string or byte for uint64, decoding into (types.Transaction)(types.LegacyTx).Nonce",
      "InvalidTo" : "rlp: expected input string or byte for common.Address, decoding into (types.Transaction)(types.LegacyTx).To",
      "GasLimitPriceProductOverflow" : "gas * gasPrice exceeds 256 bits",
      "TooMuchGasUsed" : "Invalid gasUsed:",
      "TooMuchGasUsed2" : "Error importing raw rlp block: t8ntool didn't return a transaction with hash",
      "LeadingZerosGasLimit" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Transaction)(types.LegacyTx).Gas",
      "LeadingZerosGasPrice" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Transaction)(types.LegacyTx).GasPrice",
      "LeadingZerosValue" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Transaction)(types.LegacyTx).Value",
      "LeadingZerosNonce" : "rlp: non-canonical integer (leading zero bytes) for uint64, decoding into (types.Transaction)(types.LegacyTx).Nonce",
      "LeadingZerosR" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Transaction)(types.LegacyTx).R",
      "LeadingZerosS" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Transaction)(types.LegacyTx).S",
      "LeadingZerosV" : "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Transaction)(types.LegacyTx).V",
      "LeadingZerosDataSize" : "rlp: non-canonical size information for []uint8, decoding into (types.Transaction)(types.LegacyTx).Data",
      "LeadingZerosNonceSize" : "rlp: non-canonical size information for uint64, decoding into (types.Transaction)(types.LegacyTx).Nonce",
      "InvalidNumber" : "BlockHeader number != parent.number + 1",
      "InvalidTimestampEqualParent" : "timestamp equals parent's",
      "InvalidTimestampOlderParent" : "BlockHeader timestamp is less or equal then it's parent block!",
      "InvalidLogBloom" : "Error in field: bloom",
      "InvalidStateRoot" : "Error in field: stateRoot",
      "InvalidGasUsed" : "Error in field: gasUsed",
      "InvalidGasUsed2" : "t8ntool didn't return a transaction with hash",
      "InvalidBlockMixHash" : "invalid mix digest",
      "InvalidBlockNonce" : "",
      "UnknownParent" : "unknown parent hash",
      "UnknownParent2" : "unknown parent hash",
      "InvalidReceiptsStateRoot" : "Error in field: receiptTrie",
      "InvalidTransactionsRoot" : "Error in field: transactionsTrie",
      "InvalidUnclesHash" : "Error in field: uncleHash",
      "InvalidUncleParentHash" : "Parent block hash not found:",
      "UncleInChain" : "Block is already in chain!",
      "UncleIsAncestor" : "Block is already in chain!",
      "UncleParentIsNotAncestor" : "Uncle number is wrong!",
      "TooManyUncles" : "Too many uncles!",
      "UncleIsBrother" : "Uncle is brother!",
      "OutOfGas" : "out of gas",
      "SenderNotEOA" : "sender not an eoa:",
      "SenderNotEOAorNoCASH" : "sender not an eoa:",
      "IntrinsicGas" : "intrinsic gas too low",
      "ExtraDataIncorrectDAO" : "BlockHeader require Dao ExtraData!",
      "InvalidTransactionVRS" : "t8ntool didn't return a transaction with hash",
      "BLOCKHEADER_VALUE_TOOLARGE" : "Blockheader parse error: VALUE  >u256",
      "TRANSACTION_VALUE_TOOLARGE" : "TransactionLegacy convertion error: VALUE  >u256",
      "TRANSACTION_VALUE_TOOSHORT" : "t8ntool didn't return a transaction with hash",
      "TR_NonceHasMaxValue" : "nonce has max value:",
      "OVERSIZE_RLP" : "Error importing raw rlp block: OversizeRLP",
      "RLP_BadCast" : "BadCast",
      "RLP_ExpectedAsList" : "expected to be list",
      "RLP_TooFewElements" : "rlp: too few elements ",
      "RLP_TooManyElements" : "rlp: input list has too many elements ",
      "RLP_InputContainsMoreThanOneValue" : "Error importing raw rlp block: OversizeRLP",
      "RLP_VALUESIZE_MORE_AVAILABLEINPUTLENGTH" : "Error importing raw rlp block: UndersizeRLP",
      "RLP_ELEMENT_LARGER_CONTAININGLIST_UNDERSIZE" : "Error importing raw rlp block: UndersizeRLP",
      "RLP_ELEMENT_LARGER_CONTAININGLIST_OVERSIZE" : "Error importing raw rlp block: OversizeRLP",
      "RLP_ExpectedInputList_EXTBLOCK" : "Error importing raw rlp block: RLP is expected to be list",
      "RLP_InvalidArg0_UNMARSHAL_BYTES" : "Error importing raw rlp block: BadCast",
      "RLP_ExpectedInputList_HEADER_DECODEINTO_BLOCK_EXTBLOCK" : "Error importing raw rlp block: BlockHeader RLP is expected to be list",
      "RLP_InputList_TooManyElements_HEADER_DECODEINTO_BLOCK_EXTBLOCK_HEADER" : "Error importing raw rlp block: Uncleheader RLP is expected to be list",
      "RLP_InputList_TooManyElements_TXDATA_DECODEINTO_BLOCK_EXTBLOCK_TXS0" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooShort_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooShort_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE2" : "Blockheader parse error: Key `coinbase` is not hash20",
      "RLP_InputString_TooShort_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TXS0_RECIPIENT" : "TransactionLegacy convertion error: Key `to` is not hash20",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT2" : "Blockheader parse error: Key `stateRoot` is not hash32",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST2" : "Blockheader parse error: Key `mixHash` is not hash32",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH2" : "Blockheader parse error: Key `parentHash` is not hash32",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH2" : "Blockheader parse error: Key `receiptTrie` is not hash32",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH" : "Blockheader parse error: Key `transactionsTrie` is not hash32",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooLong_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH2" : "Blockheader parse error: Key `uncleHash` is not hash32",
      "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT" : "Blockheader parse error: VALUE  >u256",
      "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED" : "Blockheader parse error: VALUE  >u256",
      "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME" : "Blockheader parse error: VALUE  >u256",
      "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT" : "TransactionLegacy convertion error: VALUE  >u256",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH2" : "Blockheader parse error: Key `receiptTrie` is not hash32",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT" : "Blockheader parse error: Key `stateRoot` is not hash32",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST2" : "Blockheader parse error: Key `mixHash` is not hash32",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH2" : "Blockheader parse error: Key `parentHash` is not hash32",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH2" : "Blockheader parse error: Key `uncleHash` is not hash32",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooShort_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH2" : "Blockheader parse error: Key `transactionsTrie` is not hash32",
      "RLP_InputString_TooShort_BLOOM_DECODEINTO_BLOCK_EXTBLOCK_HEADER_BLOOM" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_DIFFICULTY" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_DIFFICULTY2" : "Blockheader parse error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT2" : "Blockheader parse error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED2" : "Blockheader parse error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME2" : "Blockheader parse error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT2" : "TransactionLegacy convertion error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NUMBER" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NUMBER2" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_PRICE" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_R" : "TransactionLegacy convertion error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_S" : "TransactionLegacy convertion error: VALUE has leading 0",
      "RLP_InputString_TooLong_BLOOM_DECODEINTO_BLOCK_EXTBLOCK_HEADER_BLOOM" : "Blockheader parse error: Key `bloom` is not hash256",
      "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_PARENTHASH" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_RECEIPTHASH" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_ROOT" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_MIXDIGEST" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TXHASH" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_HASH_DECODEINTO_BLOCK_EXTBLOCK_HEADER_UNCLEHASH" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TX0_RECIPIENT" : "Error importing raw rlp block: Transaction RLP field is not data!",
      "RLP_InputString_TooLong_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_HEADER_COINBASE" : "Blockheader parse error: Key `coinbase` is not hash20",
      "RLP_InputString_TooLong_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TXS0_RECIPIENT" : "TransactionLegacy convertion error: Key `to` is not hash20",
      "RLP_ExpectedInputString_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_DIFFICULTY" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXR" : "Error importing raw rlp block: Transaction RLP field is not data!",
      "RLP_ExpectedInputString_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXS" : "Error importing raw rlp block: Transaction RLP field is not data!",
      "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASLIMIT" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_GASUSED" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_HEADER_TIME" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT" : "Error importing raw rlp block: Transaction RLP field is not data!",
      "RLP_ExpectedInputString_NONCE_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NONCE" : "Error importing raw rlp block: Blockheader RLP field is not data!",
      "RLP_ExpectedInputString_UINT8_DECODEINTO_BLOCK_EXTBLOCK_TXS0_PAYLOAD" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooLong_BLOCKNONCE_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NONCE" : "Error importing raw rlp block: OversizeRLP",
      "RLP_InputString_TooLong_BLOCKNONCE_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NONCE2" : "Blockheader parse error: Key `nonce` is not hash8",
      "RLP_NonCanonical_SizeInfo_EXTBLOCK" : "Error importing raw rlp block: BadRLP",
      "RLP_ExpectedInputList_TRANSACTION_DECODEINTO_BLOCK_EXTBLOCK_TXS" : "Error importing raw rlp block: BadCast",
      "RLP_ExpectedInputList_HEADER_DECODEINTO_BLOCK_EXTBLOCK_UNCLES" : "Error importing raw rlp block: OversizeRLP",
      "RLP_ExpectedInputList_TXDATA_DECODEINTO_BLOCK_EXTBLOCK_TXS0" : "Error importing raw rlp block: Transaction RLP is expected to be list",
      "RLP_Error_EOF" : "ERROR(11): unexpected EOF",
      "RLP_Error_RLP_Size" : "ERROR(11): rlp: value size exceeds available input length",
      "RLP_Error_Size_Information" : "ERROR(11): rlp: non-canonical size information",
      "LegacyBlockImportImpossible" : "Legacy block import is impossible",
      "LegacyBlockImportImpossible2" : "Legacy block can only be on top of LegacyBlock",
      "LegacyBlockBaseFeeTransaction" : "BaseFee transaction in a Legacy blcok",
      "1559BlockImportImpossible_HeaderIsLegacy" : "1559 block must be on top of 1559",
      "1559BlockImportImpossible_BaseFeeWrong": "Error in field: baseFeePerGas",
      "1559BlockImportImpossible_InitialBaseFeeWrong": "Initial baseFee must be 1000000000",
      "1559BlockImportImpossible_TargetGasLow": "gasTarget decreased too much",
      "1559BlockImportImpossible_TargetGasHigh": "gasTarget increased too much",
      "1559BlockImportImpossible_InitialGasLimitInvalid": "Invalid block1559: Initial gasLimit must be",
      "MergeBlockImportImpossible" : "Trying to import Merge block on top of Shanghai block after transition",
      "ParisBlockImportImpossible" : "Trying to import Paris block on top of Shanghai block after transition",
      "ShanghaiBlockImportImpossible" : "Trying to import Shanghai block on top of block that is not Shanghai!!",
      "TR_IntrinsicGas" : "intrinsic gas too low",
      "TR_NoFunds" : "insufficient funds for gas * price + value",
      "TR_NoFundsX" : "insufficient funds for gas * price + value",
      "TR_NoFundsValue" : "insufficient funds for transfer",
      "TR_NoFundsOrGas" : "insufficient funds for gas * price + value",
      "TR_FeeCapLessThanBlocks" : "max fee per gas less than block base fee",
      "TR_GasLimitReached" : "gas limit reached",
      "TR_FeeCapLessThanBlocksORGasLimitReached" : "gas limit reached",
      "TR_FeeCapLessThanBlocksORNoFunds" : "max fee per gas less than block base fee",
      "TR_NonceTooHigh" : "nonce too high",
      "TR_NonceTooLow" : "nonce too low",
      "TR_TypeNotSupported" : "transaction type not supported",
      "TR_TipGtFeeCap": "max priority fee per gas higher than max fee per gas",
      "TR_TooShort": "typed transaction too short",
      "TR_InitCodeLimitExceeded" : "max initcode size exceeded",
      "1559BaseFeeTooLarge": "TransactionBaseFee convertion error: VALUE  >u256",
      "1559PriorityFeeGreaterThanBaseFee": "maxFeePerGas \u003c maxPriorityFeePerGas",
      "2930AccessListAddressTooLong": "rlp: input string too long for common.Address, decoding into (types.Transaction)(types.AccessListTx).AccessList[0].Address",
      "2930AccessListAddressTooShort": "rlp: input string too short for common.Address, decoding into (types.Transaction)(types.AccessListTx).AccessList[0].Address",
      "1559LeadingZerosBaseFee": "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Transaction)(types.DynamicFeeTx).GasFeeCap",
      "1559LeadingZerosPriorityFee":  "rlp: non-canonical integer (leading zero bytes) for *big.Int, decoding into (types.Transaction)(types.DynamicFeeTx).GasTipCap",
      "2930AccessListStorageHashTooShort": "rlp: input string too short for common.Hash, decoding into (types.Transaction)(types.AccessListTx).AccessList[0].StorageKeys[0]",
      "2930AccessListStorageHashTooLong": "rlp: input string too long for common.Hash, decoding into (types.Transaction)(types.AccessListTx).AccessList[0].StorageKeys[0]",
      "3675PoWBlockRejected" : "Invalid block1559: Chain switched to PoS!",
      "3675PoSBlockRejected" : "Parent (transition) block has not reached TTD",
      "3675PreParis1559BlockRejected" : "Trying to import 1559 block on top of PoS block",
      "INPUT_UNMARSHAL_ERROR" : "cannot unmarshal hex",
      "INPUT_UNMARSHAL_SIZE_ERROR" : "failed unmarshaling",
      "RLP_BODY_UNMARSHAL_ERROR" : "Rlp structure is wrong",
      "EOF_InvalidPrefix" : "err: invalid_prefix",
      "EOF_UnknownVersion" : "err: eof_version_unknown",
      "EOF_IncompleteSectionSize" : "err: incomplete_section_size",
      "EOF_IncompleteSectionNumber": "err: incomplete_section_number",
      "EOF_HeaderTerminatorMissing": "err: header_terminator_missing",
      "EOF_TypeSectionMissing": "err: type_section_missing",
      "EOF_CodeSectionMissing": "err: code_section_missing",
      "EOF_DataSectionMissing": "err: data_section_missing",
      "EOF_ZeroSectionSize": "err: zero_section_size",
      "EOF_SectionHeadersNotTerminated": "err: section_headers_not_terminated",
      "EOF_InvalidSectionBodiesSize": "err: invalid_section_bodies_size",
      "EOF_UnreachableCodeSections" : "err: unreachable_code_sections",
      "EOF_UndefinedInstruction": "err: undefined_instruction",
      "EOF_TruncatedImmediate": "err: truncated_instruction",
      "EOF_InvalidJumpDestination": "err: invalid_rjump_destination",
      "EOF_TooManyCodeSections": "err: too_many_code_sections",
      "EOF_InvalidTypeSectionSize": "err: invalid_type_section_size",
      "EOF_InvalidFirstSectionType": "err: invalid_first_section_type",
      "EOF_InvalidMaxStackHeight": "err: invalid_max_stack_height",
      "EOF_InvalidCodeTermination": "err: no_terminating_instruction",
      "EOF_ConflictingStackHeight": "err: stack_height_mismatch",
      "EOF_InvalidNumberOfOutputs": "err: stack_higher_than_outputs_required",
      "EOF_MaxStackHeightExceeded": "err: max_stack_height_above_limit",
      "EOF_UnreachableCode": "err: unreachable_instructions",
      "EOF_InputsOutputsNumAboveLimit": "err: inputs_outputs_num_above_limit",
      "EOF_StackUnderflow": "err: stack_underflow",
      "EOF_StackOverflow": "err: stack_overflow",
      "EOF_InvalidCodeSectionIndex": "err: invalid_code_section_index",
      "EOF_InvalidDataloadnIndex": "err: invalid_dataloadn_index",
      "EOF_JumpfDestinationIncompatibleOutputs": "err: jumpf_destination_incompatible_outputs",
      "EOF_CallfToNonReturningFunction": "err: callf_to_non_returning_function",
      "EOF_TooManyContainerSections": "err: too_many_container_sections",
      "EOF_InvalidContainerSectionIndex": "err: invalid_container_section_index",
      "EOF_EofCreateWithTruncatedContainer": "err: eof_create_with_truncated_container",
      "EOF_InvalidNonReturningFlag" : "err: invalid_non_returning_flag",
      "PostParisUncleHashIsNotEmpty" : "block.uncleHash != empty",
      "PostParisDifficultyIsNot0" : "block.difficulty must be 0"
    }
})";

string const evmone_start = R"(#!/bin/sh

wevm=$(which evmone-t8n)
if [ -z $wevm ]; then
   >&2 echo "Can't find EvmOne's 'evmone-t8n' executable alias in the system path!"
   exit 1
fi

if [ $1 = "t8n" ] || [ $1 = "b11r" ]; then
    evmone-t8n $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26
elif [ $1 = "-v" ]; then
    evmone-t8n -v
elif [ $1 = "eof" ]; then
    wevm=$(which evmone-eofparse)
    if [ -z $wevm ]; then
       >&2 echo "Can't find EvmOne's 'evmone-eofparse' executable alias in the system path!"
       exit 1
    fi
    result=$(echo $5 | evmone-eofparse)
    echo "$result"
else
    stateProvided=0
    readErrorLog=0
    errorLogFile=""
    cmdArgs=""
    for index in ${1} ${2} ${3} ${4} ${5} ${6} ${7} ${8} ${9} ${10} ${11} ${12} ${13} ${14} ${15} ${16} ${17} ${18} ${19} ${20} ${21} ${22} ${23} ${24} ${25} ${26}; do
        if [ $index = "--input.alloc" ]; then
            stateProvided=1
        fi
        if [ $readErrorLog -eq 1 ]; then
            errorLogFile=$index
            readErrorLog=0
            continue
        fi
        if [ $index = "--output.errorlog" ]; then
            readErrorLog=1
            continue
        fi
        cmdArgs=$cmdArgs" "$index
    done
    if [ $stateProvided -eq 1 ]; then
        if [ -z $errorLogFile ]; then
            evmone-t8n $cmdArgs --verbosity 2
        else
            evmone-t8n $cmdArgs --verbosity 2 2> $errorLogFile
        fi
    else
        echo "Error: evmone-t9n not supported"
        exit 2
    fi
fi
)";


    {
        spDataObject obj;
        (*obj)["path"] = "evmone/config";
        (*obj)["content"] = evmone_config;
        map_configs.addArrayObject(obj);
    }
    {
        spDataObject obj;
        (*obj)["exec"] = true;
        (*obj)["path"] = "evmone/start.sh";
        (*obj)["content"] = evmone_start;
        map_configs.addArrayObject(obj);
    }
    {
        spDataObject obj;
        (*obj)["exec"] = true;
        (*obj)["path"] = "evmone/yul.sh";
        (*obj)["content"] = yul_compiler_sh;
        map_configs.addArrayObject(obj);
    }
}
}  // namespace retesteth::options
