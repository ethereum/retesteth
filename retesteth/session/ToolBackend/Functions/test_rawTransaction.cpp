#include <retesteth/testStructures/basetypes.h>
#include <retesteth/session/SessionInterface.h>
#include <retesteth/session/ToolBackend/ToolChainManager.h>
#include <retesteth/Options.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace toolimpl;
using namespace test::session;
namespace fs = boost::filesystem;

TestRawTransaction ToolChainManager::test_rawTransaction(
    BYTES const& _rlp, FORK const& _fork, fs::path const& _toolPath, fs::path const& _tmpDir)
{
    // Prepare test_mineBlocks response structure
    DataObject out;
    out["result"] = true;

    // Prepare transaction file
    fs::path const txsPath = _tmpDir / "tx.rlp";
    fs::path const errorLog = _tmpDir / "error.txt";

    // Rlp list header builder for given data
    test::RLPStreamU txsout(1);
    if (_rlp.firstByte() < 128)
    {
        // wrap typed transactions as RLPstring in RLPStream
        txsout.appendString(_rlp.asString());
    }
    else
        txsout.appendRaw(_rlp.asString());

    // Write data with memory allocation but faster
    writeFile(txsPath.string(), string("\"") + txsout.outHeader() + _rlp.asString().substr(2) + "\"");
    ETH_DC_MESSAGE(DC::RPC, "TXS file:\n" + string("\"") + txsout.outHeader() + _rlp.asString().substr(2) + "\"");

    string cmd = _toolPath.string();
    cmd += " --input.txs " + txsPath.string();
    cmd += " --state.fork " + _fork.asString();
    cmd += " --output.errorlog " + errorLog.string();

    ETH_DC_MESSAGE(DC::RPC, cmd);
    int exitCode;
    string response = test::executeCmd(cmd, exitCode, ExecCMDWarning::NoWarningNoError);


    ETH_DC_MESSAGE(DC::RPC, "T9N Response:\n" + response);
    spDataObject res;
    bool errorCaught = false;

    try
    {
        res = dataobject::ConvertJsoncppStringToData(response);
    }
    catch (std::exception const& _ex)
    {
        if (string(_ex.what()).find("can't read json") != string::npos)
        {
            // Unable to read json. treat response as exceptional failure on wrong input
            if (Options::get().filltests)
                ETH_WARNING("t9n returned invalid json, probably failed on input!");
            res = spDataObject(new DataObject(DataType::Array));
            spDataObject errObj;
            string const outErrorContent = dev::contentsString(errorLog.string());
            (*errObj)["error"] = outErrorContent;
            (*res).addSubObject(errObj);
            ETH_DC_MESSAGE(DC::RPC, "T9N Response reconstructed:\n" + res->asJson());
            errorCaught = true;
        }
        else
            throw _ex;
    }

    string const hash = "0x" + dev::toString(dev::sha3(fromHex(_rlp.asString())));
    spDataObject tr;

    auto const& resTr = res->getSubObjects().at(0);
    if (resTr->count("intrinsicGas"))
    {
        if (resTr->atKey("intrinsicGas").type() == DataType::Integer)
            (*tr)["intrinsicGas"] = VALUE(resTr->atKey("intrinsicGas").asInt()).asString();
        else if (resTr->atKey("intrinsicGas").type() == DataType::String)
            (*tr)["intrinsicGas"] = VALUE(resTr->atKey("intrinsicGas").asString()).asString();
        else
            ETH_ERROR_MESSAGE("`intrinsicGas` field type expected to be Int or String: `" + resTr->asJson());
    }
    else
        (*tr)["intrinsicGas"] = "0x00";

    if (response.find("error") != string::npos || response.find("ERROR") != string::npos || errorCaught)
    {
        (*tr)["error"] = resTr->atKey("error").asString();
        (*tr)["sender"] = FH20::zero().asString();
        (*tr)["hash"] = hash;
        out["rejectedTransactions"].addArrayObject(tr);
    }
    else
    {
        (*tr)["sender"] = resTr->atKey("address").asString();
        (*tr)["hash"] = resTr->atKey("hash").asString();
        out["acceptedTransactions"].addArrayObject(tr);
        if (tr->atKey("hash").asString() != hash)
            ETH_ERROR_MESSAGE("t8n tool returned different tx.hash than retesteth: (t8n.hash != retesteth.hash) " + tr->atKey("hash").asString() + " != " + hash);
    }

    ETH_DC_MESSAGE(DC::RPC, "Response: test_rawTransaction `" + out.asJson());
    return TestRawTransaction(out);
}
