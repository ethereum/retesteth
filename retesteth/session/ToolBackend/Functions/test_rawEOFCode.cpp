#include <retesteth/session/ToolBackend/ToolChainManager.h>
#include <retesteth/testStructures/basetypes.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/TestHelper.h>
using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace toolimpl;
namespace fs = boost::filesystem;


string ToolChainManager::test_rawEOFCode(
    BYTES const& _code, FORK const& _fork, fs::path const& _toolPath, fs::path const& _tmpDir)
{
    (void) _fork;
    fs::path const errorLog = _tmpDir / "error.txt";

    string cmd = _toolPath.string();
    cmd += " eof";
    cmd += " --hex " + _code.asString();
    cmd += " 2>" + errorLog.string();
    //cmd += " --state.fork " + _fork.asString();

    ETH_DC_MESSAGE(DC::RPC, cmd);
    int exitCode;
    string response = test::executeCmd(cmd, exitCode, ExecCMDWarning::NoWarningNoError);
    if (exitCode != 0)
    {
        string const outErrorContent = dev::contentsString(errorLog.string());
        ETH_DC_MESSAGE(DC::RPC, "Err:\n" + outErrorContent);
        return outErrorContent;
        //throw test::UpwardsException(errorLog.empty() ? (response.empty() ? "Tool failed: " + cmd : response) : outErrorContent);
    }
    return response;
}
