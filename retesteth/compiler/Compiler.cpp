#include "Options.h"
#include <retesteth/helpers/TestHelper.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/helpers/TestOutputHelper.h>
using namespace dev;
using namespace test;
using namespace std;
using namespace dataobject;
using namespace test::compiler;
namespace fs = boost::filesystem;

namespace
{

void removeCommentsFromCode(string& _code)
{
    size_t posComment = _code.find("#");
    while(posComment != string::npos)
    {
        size_t posEndl = string::npos;
        size_t const posEndl1 = _code.find('\n');
        size_t const posEndl2 = _code.find("\\n");
        posEndl = min(posEndl1, posEndl2);

        if (posEndl != string::npos)
        {
            if (posEndl < posComment)
            {
                _code.erase(posEndl, 1);
                posComment = _code.find("#");
                continue;
            }
            _code.erase(posComment, posEndl - posComment + 1);
        }
        else
            _code.erase(posComment);
        posComment = _code.find("#");
    }
}

string compileLLL(string const& _code)
{
#if defined(_WIN32)
    BOOST_ERROR("LLL compilation only supported on posix systems.");
    return "";
#else
    fs::path path(fs::temp_directory_path() / fs::unique_path());
    string cmd = string("lllc ") + path.string();
    writeFile(path.string(), _code);
    try
    {
        int exitCode;
        string result = executeCmd(cmd, exitCode);
        fs::remove_all(path);
        result = "0x" + result;
        test::compiler::utiles::checkHexHasEvenLength(result);
        return result;
    }
    catch (EthError const& _ex)
    {
        fs::remove_all(path);
        ETH_WARNING("Error compiling lll code: " + _code.substr(0, 50) + "..");
        throw _ex;
    }
#endif
}


bool tryCustomCompiler(string const& _code, string& _compiledCode)
{
    auto const& compilers = Options::getCurrentConfig().cfgFile().customCompilers();
    for (auto const& [compilerPrefix, compilerScript] : compilers)
    {
        if (_code.find(compilerPrefix) != string::npos)
        {
            size_t const pos = _code.find(compilerPrefix);
            char afterPrefix = _code[pos + compilerPrefix.length()];
            if ((afterPrefix == ' ' || afterPrefix == '\n'))
            {
                size_t codeStartPos = pos + compilerPrefix.length() + 1;
                string arg;
                string nativeArg;
                if (afterPrefix == ' ')
                {
                    auto const argArr = parseArgsFromStringIntoArray(_code, codeStartPos);
                    for (auto const& el : argArr)
                    {
                        if (el == "object" || el == "\"C\"")
                            nativeArg += el + " "; // Special case for native yul args
                        else
                            arg += el + " ";
                    }
                }
                string const customCode = nativeArg + _code.substr(codeStartPos);
                fs::path path(fs::temp_directory_path() / fs::unique_path());
                string cmd = compilerScript.string() + " " + path.string() + " " + arg;
                writeFile(path.string(), customCode);

                int exitCode;
                _compiledCode = test::executeCmd(cmd, exitCode);
                utiles::checkHexHasEvenLength(_compiledCode);
                return true;
            }
        }
    }
    return false;
}

void tryKnownCompilers(string const& _code, solContracts const& _preSolidity, string& _compiledCode)
{
    string const c_rawPrefix = ":raw";
    string const c_abiPrefix = ":abi";
    string const c_solidityPrefix = ":solidity";

    bool found = false;
    if (_code.find("pragma solidity") != string::npos)
    {
        solContracts const contracts = compileSolidity(_code);
        if (contracts.Contracts().size() > 1)
            ETH_ERROR_MESSAGE("Compiling solc: Only one solidity contract is allowed per address!");
        _compiledCode = contracts.Contracts().at(0)->asString();
        found = true;
    }
    else if (_code.find(c_solidityPrefix) != string::npos)
    {
        size_t const pos = _code.find(c_solidityPrefix);
        const char endChar = _code[pos + c_solidityPrefix.length()];
        bool bSolidityEndline = endChar == ' ' || endChar == '\n';
        if (bSolidityEndline)
        {
            string const contractName = _code.substr(pos + c_solidityPrefix.length() + 1);
            _compiledCode = _preSolidity.getCode(contractName);
            found = true;
        }
    }
    else if (_code.find(c_rawPrefix) != string::npos)
    {
        size_t const pos = _code.find(c_rawPrefix);
        const char endChar = _code[pos + c_rawPrefix.length()];
        bool bRawEndline = endChar == ' ' || endChar == '\n';
        if (bRawEndline)
        {
            _compiledCode = _code.substr(pos + c_rawPrefix.length() + 1);
            test::removeSubChar(_compiledCode, {' ', '-'});
            removeCommentsFromCode(_compiledCode);
            test::removeSubChar(_compiledCode, {'\n', 'n', '\\'});
            utiles::checkHexHasEvenLength(_compiledCode);
            found = true;
        }
    }
    else if (_code.find(c_abiPrefix) != string::npos)
    {
        size_t const pos = _code.find(c_abiPrefix);
        const char endChar = _code[pos + c_abiPrefix.length()];
        bool bAbiEndline = endChar == ' ' || endChar == '\n';
        if (bAbiEndline)
        {
            string const abiCode = _code.substr(pos + c_abiPrefix.length() + 1);
            _compiledCode = utiles::encodeAbi(abiCode);
            utiles::checkHexHasEvenLength(_compiledCode);
            found = true;
        }
    }

    if (!found)
    {
        if (_code.find('{') != string::npos || _code.find("(asm") != string::npos )
            _compiledCode = compileLLL(_code);
        else
        {
            ETH_ERROR_MESSAGE("Trying to compile code of unknown type (missing 0x prefix?): `" + _code);
        }
    }
}
}  // namespace

namespace test::compiler
{
namespace utiles
{
void checkHexHasEvenLength(string const& _hex)
{
    ETH_ERROR_REQUIRE_MESSAGE(
        isHex(_hex), "void checkHexHasEvenLength(string const& _hex) got argument which is not a hex string: \n`" + _hex);
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
    bool customCompilerWorked = tryCustomCompiler(_code, compiledCode);
    if (!customCompilerWorked)
        tryKnownCompilers(_code, _preSolidity, compiledCode);

    if (compiledCode == "0x")
        ETH_WARNING("replaceCode returned empty bytecode `0x` trying to compile " + TestOutputHelper::get().testInfo().errorDebug() +  "\n" + _code);

    if (_code.size() > 0)
        ETH_FAIL_REQUIRE_MESSAGE(
            compiledCode.size() > 0, "Bytecode is missing! '" + _code + "' " + TestOutputHelper::get().testName());
    return compiledCode;
}
}  // namespace compiler
