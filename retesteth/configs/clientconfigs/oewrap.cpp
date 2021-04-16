#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

string const oewrap_config = R"({
    "name" : "Openethereum-evm wrapper as StateTool",
    "socketType" : "tranition-tool",
    "socketAddress" : "start.sh",
    "forks" : [
        "Frontier",
        "Homestead",
        "EIP150",
        "EIP158",
        "Byzantium",
        "Constantinople",
        "ConstantinopleFix",
        "Istanbul",
        "Berlin"
    ],
    "additionalForks" : [
        "FrontierToHomesteadAt5",
        "HomesteadToEIP150At5",
        "EIP158ToByzantiumAt5",
        "HomesteadToDaoAt5",
        "ByzantiumToConstantinopleFixAt5"
    ],
    "exceptions" : {
      "InvalidStateRoot" : "",
      "ExtraDataTooBig" : "Error importing raw rlp block: Header extraData > 32 bytes",
      "InvalidDifficulty" : "Invalid difficulty:",
      "InvalidDifficulty2" : "Error in field: difficulty",
      "InvalidDifficulty_TooLarge" : "Blockheader parse error: VALUE  >u256",
      "InvalidGasLimit" : "Header gasLimit > 0x7fffffffffffffff",
      "TooMuchGasUsed" : "Invalid gasUsed:",
      "TooMuchGasUsed2" : "Error importing raw rlp block: t8ntool didn't return a transaction with hash",
      "InvalidNumber" : "BlockHeader number != parent.number + 1",
      "InvalidTimestampEqualParent" : "timestamp equals parent's",
      "InvalidTimestampOlderParent" : "BlockHeader timestamp is less then it's parent block!",
      "InvalidLogBloom" : "Error in field: bloom",
      "InvalidStateRoot" : "Error in field: stateRoot",
      "InvalidGasLimit2" : "Invalid gaslimit:",
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
      "IntrinsicGas" : "t8ntool didn't return a transaction with hash",
      "ExtraDataIncorrectDAO" : "BlockHeader require Dao ExtraData!",
      "InvalidTransactionVRS" : "t8ntool didn't return a transaction with hash",
      "BLOCKHEADER_VALUE_TOOLARGE" : "Blockheader parse error: VALUE  >u256",
      "TRANSACTION_VALUE_TOOLARGE" : "Transaction convertion error: VALUE  >u256",
      "TRANSACTION_VALUE_TOOSHORT" : "t8ntool didn't return a transaction with hash",
      "OVERSIZE_RLP" : "Error importing raw rlp block: OversizeRLP",
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
      "RLP_InputString_TooShort_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TXS0_RECIPIENT" : "Transaction convertion error: Key `to` is not hash20",
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
      "RLP_InputString_TooLong_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT" : "Transaction convertion error: VALUE  >u256",
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
      "RLP_NonCanonicalINT_LeadingZeros_UINT64_DECODEINTO_BLOCK_EXTBLOCK_TXS0_GASLIMIT2" : "Transaction convertion error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NUMBER" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_HEADER_NUMBER2" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_PRICE" : "Error importing raw rlp block: OversizeRLP",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_R" : "Transaction convertion error: VALUE has leading 0",
      "RLP_NonCanonicalINT_LeadingZeros_BIGINT_DECODEINTO_BLOCK_EXTBLOCK_TXS0_TXDATA_S" : "Transaction convertion error: VALUE has leading 0",
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
      "RLP_InputString_TooLong_ADDRESS_DECODEINTO_BLOCK_EXTBLOCK_TXS0_RECIPIENT" : "Transaction convertion error: Key `to` is not hash20",
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
      "RLP_ExpectedInputList_TXDATA_DECODEINTO_BLOCK_EXTBLOCK_TXS0" : "Error importing raw rlp block: Transaction RLP is expected to be list"
    }
})";

string const oewrap_wrapper = R"(#! /usr/bin/node

const yargs = require("yargs")
const fs = require('fs')
const { exec } = require("child_process")
const ethTx = require("ethereumjs-tx").Transaction
const keccak256 = require("keccak256")

// Convert fork names from the t8ntool tool version to the openethereum
// version
const convertFork = t8nName => {
  if (t8nName === "YOLOv3") return "Berlin"
  return t8nName
}

// These need to change so we can run multiple tests at the same time
const testFile = "/tmp/test.json"
const resFile = "/tmp/res.jsnol"


const options = yargs.argv

console.log(options)

const alloc = JSON.parse(fs.readFileSync(options.input.alloc).toString())
const txs = JSON.parse(fs.readFileSync(options.input.txs).toString())
const env = JSON.parse(fs.readFileSync(options.input.env).toString())


if (txs.length == 0) {
  // This is the first call, which "mines" the genesis block. There are no
  // transactions

  // Extra fields which are ignored
  const outAlloc = alloc
  const out = {
    stateRoot:   "0xDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeef",
    txRoot:      "0xDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeef",
    receiptRoot: "0xDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeef",
    logsHash:    "0xDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeef",
    logsBloom:   "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    receipts: []
  }

  fs.writeFileSync(`${options.output.basedir}/${options.output.result}`,
        JSON.stringify(out, null, 2))
  fs.writeFileSync(`${options.output.basedir}/${options.output.alloc}`,
        JSON.stringify(outAlloc, null, 2))

  process.exit(0)
}

// If we got here, this is the test block, the one with a transaction
const tx = txs[0]
const txHash = `0x${keccak256(new ethTx(tx).serialize()).toString('hex')}`

// Some values need to be added programatically
let test = {
   t8ntoolTest: {
      "_info": {},
      env: env,
      pre: alloc,
      transaction: {
         data: [tx.input],
         gasLimit: [tx.gas],
         gasPrice: tx.gasPrice,
         nonce: tx.nonce,
         secretKey: tx.secretKey,
         to: tx.to,
         value: [tx.value]
      },
      post: {}
   }
}

test.t8ntoolTest.post[convertFork(options.state.fork)] = [{
   "indexes": { "data":0, "gas":0, "value":0},
   "hash": "0x0000000000000000000000000000000000000000000000000000000000000000"
}]

// Write the test file we created
fs.writeFileSync(testFile, JSON.stringify(test, null, 2))

// Process the test results
const processResult = (root, outAlloc) => {

  const out = {
    stateRoot:   root,

    // These fields are required, but their values are ignored
    receiptRoot: "0xDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeef",
    txRoot: "0xDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeefDEADBEEFdeadbeef",

    // Hash and bloom for not having logs. This tool will fail on tests that produce
    // event logs
    logsHash:    "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347",
    logsBloom:   "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",

    receipts: [
      {
         // This has to be the real hash of the transaction bytes
         "transactionHash": txHash,

         // These fields have to be there, but their value doesn't matter
         "root": "0x",
         "status": "0x",
         "cumulativeGasUsed": "0x",
         "logsBloom": "0x",
         "logs": null,
         "contractAddress": "0x",
         "gasUsed": "0x01",
         "blockHash": "0x0000000000000000000000000000000000000000000000000000000000000000",
         "transactionIndex": "0x0"
      }
    ]
  }

  fs.writeFileSync(`${options.output.basedir}/${options.output.result}`,
        JSON.stringify(out, null, 2))
  fs.writeFileSync(`${options.output.basedir}/${options.output.alloc}`,
        JSON.stringify(outAlloc, null, 2))

  process.exit(0)
}  // processResults

// Run the test file and interpret the results
exec(`openethereum-evm state-test ${testFile} --std-dump-json`,
         (err, stdout, stderr) => {
           if (err) console.log(`error: ${err.message}`)

           // The last line in stderr is the output we care about
           if (stderr) {
              const lines = stderr.split("\n")
              const res = JSON.parse(lines[lines.length-2])

              processResult(res.root, res.accounts)
           }
}) // exec("openethereum-env ...
)";

string const oewrap_package = R"(
{
  "name": "oe_tool",
  "version": "1.0.0",
  "description": "",
  "main": "t8n_oe.js",
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "author": "",
  "license": "ISC",
  "dependencies": {
    "ethereumjs-tx": "^2.1.2",
    "keccak256": "^1.0.2",
    "yargs": "^13.2.4"
  }
})";

string const trickystr = "SCRIPTPATH=\"$( cd -- \"$(dirname \"$0\")\" >/dev/null 2>&1 ; pwd -P )\"";
string const oewrap_start = R"(#!/bin/sh
)" + trickystr + R"(
cd $SCRIPTPATH
if [ ! -d "node_modules" ]; then
    npm install
fi

if [ $1 = "-v" ]; then
    ./t8n_oe.js -v
else
    ./t8n_oe.js t8n $1 $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 --verbosity 2
fi
)";

oewrapcfg::oewrapcfg()
{
    {
        DataObject obj;
        obj["path"] = "oewrap/config";
        obj["content"] = oewrap_config;
        map_configs.addArrayObject(obj);
    }
    {
        DataObject obj;
        obj["exec"] = true;
        obj["path"] = "oewrap/start.sh";
        obj["content"] = oewrap_start;
        map_configs.addArrayObject(obj);
    }
    {
        DataObject obj;
        obj["exec"] = true;
        obj["path"] = "oewrap/t8n_oe.js";
        obj["content"] = oewrap_wrapper;
        map_configs.addArrayObject(obj);
    }
    {
        DataObject obj;
        obj["exec"] = false;
        obj["path"] = "oewrap/package.json";
        obj["content"] = oewrap_package;
        map_configs.addArrayObject(obj);
    }
}
