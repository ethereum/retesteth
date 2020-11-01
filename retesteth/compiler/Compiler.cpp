#include "Compiler.h"
#include "Options.h"
#include "TestHelper.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/testStructures/Common.h>
#include <boost/filesystem.hpp>
using namespace dev;
using namespace test;
using namespace dataobject;
namespace fs = boost::filesystem;

namespace
{
string compileLLL(string const& _code)
{
#if defined(_WIN32)
    BOOST_ERROR("LLL compilation only supported on posix systems.");
    return "";
#else
    fs::path path(fs::temp_directory_path() / fs::unique_path());
    string cmd = string("lllc ") + path.string();
    writeFile(path.string(), _code);
    string result = executeCmd(cmd);
    fs::remove_all(path);
    result = "0x" + result;
    test::compiler::utiles::checkHexHasEvenLength(result);
    return result;
#endif
}
}  // namespace

namespace test
{
namespace compiler
{
namespace utiles
{
void checkHexHasEvenLength(string const& _hex)
{
    ETH_ERROR_REQUIRE_MESSAGE(_hex.length() % 2 == 0,
        TestOutputHelper::get().testName() + ": Hex field is expected to be of odd length: '" + _hex + "'");
}

}  // namespace utiles

/// This function is called for every account "code" : field in Fillers
/// And transaction "data" filed in Fillers
string replaceCode(string const& _code, solContracts const& _preSolidity)
{
    if (_code == "")
        return "0x";

    if (_code.substr(0, 2) == "0x" && _code.size() >= 2)
    {
        utiles::checkHexHasEvenLength(_code);
        if (Options::get().filltests && _code.size() > 2)
            ETH_WARNING("Filling raw bytecode ('" + _code.substr(0, 10) + "..'), please provide the source!" +
                        TestOutputHelper::get().testInfo().errorDebug());
        return _code;
    }

    string compiledCode;
    string const c_rawPrefix = ":raw";
    string const c_abiPrefix = ":abi";
    string const c_solidityPrefix = ":solidity";
    if (_code.find("pragma solidity") != string::npos)
    {
        solContracts const contracts = compileSolidity(_code);
        if (contracts.Contracts().size() > 1)
            ETH_ERROR_MESSAGE("Compiling solc: Only one solidity contract is allowed per address!");
        compiledCode = contracts.Contracts().at(0).asString();
    }
    else if (_code.find(c_solidityPrefix) != string::npos)
    {
        size_t const pos = _code.find(c_solidityPrefix);
        string const contractName = _code.substr(pos + c_solidityPrefix.length() + 1);
        compiledCode = _preSolidity.getCode(contractName);
    }
    else if (_code.find(c_rawPrefix) != string::npos)
    {
        size_t const pos = _code.find(c_rawPrefix);
        compiledCode = _code.substr(pos + c_rawPrefix.length() + 1);
        utiles::checkHexHasEvenLength(compiledCode);
    }
    else if (_code.find(c_abiPrefix) != string::npos)
    {
        size_t const pos = _code.find(c_abiPrefix);
        string const abiCode = _code.substr(pos + c_abiPrefix.length() + 1);
        compiledCode = utiles::encodeAbi(abiCode);
        utiles::checkHexHasEvenLength(compiledCode);
    }
    else if (_code.find('{') != string::npos || _code.find("(asm") != string::npos )
        compiledCode = compileLLL(_code);
    else
        ETH_ERROR_MESSAGE("Trying to compile code of unknown type (missing 0x prefix?): `" + _code);

    if (_code.size() > 0)
        ETH_FAIL_REQUIRE_MESSAGE(
            compiledCode.size() > 0, "Bytecode is missing! '" + _code + "' " + TestOutputHelper::get().testName());
    return compiledCode;
}
}  // namespace compiler
}  // namespace test
