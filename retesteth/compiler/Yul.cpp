#include "TestHelper.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
using namespace dev;
using namespace test;
using namespace dataobject;
using namespace std;

namespace test
{
namespace compiler
{
string compileYul(string const& _code)
{
#if defined(_WIN32)
    BOOST_ERROR("Yul compilation only supported on posix systems.");
    return "";
#else
    fs::path const path(fs::temp_directory_path() / fs::unique_path());

    string const cmd = string("solc --assemble ") + path.string();
    writeFile(path.string(), _code);
    string result = executeCmd(cmd);
    string c_findString = "Binary representation:";
    size_t codePos = result.find(c_findString);
    if (codePos != string::npos)
    {
        size_t codeStartPos = codePos + c_findString.size() + 1;
        size_t codeEndPos = result.find('\n', codeStartPos + 1);
        if (codeEndPos == string::npos)
            ETH_FAIL_MESSAGE("YUL compile error!");

        string const yulCode = result.substr(codeStartPos, codeEndPos - codeStartPos);
        return "0x" + yulCode;
    }
    else
        ETH_FAIL_MESSAGE("Error compiling YUL!");
    return "";
#endif
}
}  // namespace compiler
}  // namespace test
