#include "Compiler.h"
#include "Options.h"
#include "TestHelper.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/testStructures/Common.h>
#include <boost/filesystem.hpp>
using namespace dev;
using namespace test;
using namespace dataobject;
namespace fs = boost::filesystem;

namespace
{
void checkHexHasEvenLength(string const& _hex)
{
    ETH_ERROR_REQUIRE_MESSAGE(_hex.length() % 2 == 0,
        TestOutputHelper::get().testName() + ": Hex field is expected to be of odd length: '" + _hex + "'");
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
    string result = executeCmd(cmd);
    fs::remove_all(path);
    result = "0x" + result;
    checkHexHasEvenLength(result);
    return result;
#endif
}

// Encode Solidity abi
string encodeAbi(string const& _code)
{
    string abi;
    std::vector<string> args = test::explode(_code, ' ');
    if (args.size() == 0)
        ETH_ERROR_MESSAGE("encodeAbi expected at least 1 argument!");

    for (size_t i = 0; i < args.size(); i++)
    {
        if (i == 0)
        {
            // hash of the function call arg
            abi += dev::sha3(args.at(0)).hex().substr(0, 8);
            continue;
        }

        // convert argument to int to be sure of input values
        u256 val = test::teststruct::hexOrDecStringToInt(args.at(i));
        abi += test::stoCompactHex(val, 32);
    }
    return "0x" + abi;
}

}  // namespace

namespace test
{
string const& test::solContracts::getCode(string const& _contractName) const
{
    if (!m_solContracts.count(_contractName))
        ETH_ERROR_MESSAGE("solContracts::getCode: error contract name: `" + _contractName +
                          "` not found! (Specify contract in \"solidity\": section of the test filler)");
    return m_solContracts.atKey(_contractName).asString();
}

solContracts compileSolidity(string const& _code)
{
#if defined(_WIN32)
    BOOST_ERROR("Solidity compilation only supported on posix systems.");
    return "";
#else
    fs::path const path(fs::temp_directory_path() / fs::unique_path());
    string const cmd = string("solc --bin-runtime ") + path.string();
    writeFile(path.string(), _code);
    string result = executeCmd(cmd);

    solContracts contracts;
    string const codeNamePrefix = "=======";
    string const codeBytePrefix = "Binary of the runtime part:";

    size_t pos = result.find(codeNamePrefix);
    while (pos != string::npos)
    {
        // Contract name ======= /tmp/ad01-b64d-321b-c636:TokenCreator =======
        size_t const nameBegin = result.find(':', pos);
        if (nameBegin == string::npos)
            ETH_ERROR_MESSAGE("compileSolidity: error parsing contract name, `:` not found!");
        size_t const nameEnd = result.find(' ', nameBegin);
        if (nameEnd == string::npos)
            ETH_ERROR_MESSAGE("compileSolidity: error parsing contract name, ` ` not found!");
        string const name = result.substr(nameBegin + 1, nameEnd - nameBegin - 1);
        // std::cerr << "Name: `" << name << "`" << std::endl;

        // Contract code
        pos = result.find(codeBytePrefix, pos);
        size_t const codeStart = pos + codeBytePrefix.length() + 1;  // + 1 is \n at the end
        if (codeStart == string::npos)
            ETH_ERROR_MESSAGE("compileSolidity: error parsing contract code, `" + codeBytePrefix + "` not found!");
        size_t const codeEnd = result.find_first_of("\n ", codeStart);
        string code;
        if (codeEnd == string::npos)
        {
            // last contract description
            code = result.substr(pos + codeBytePrefix.length() + 1);
            contracts.insertCode(name, "0x" + code);
            checkHexHasEvenLength(code);
            break;
        }

        code = result.substr(pos + codeBytePrefix.length() + 1, codeEnd - codeStart);
        contracts.insertCode(name, "0x" + code);
        result = result.substr(codeEnd);
        pos = result.find(codeNamePrefix);
    }

    if (contracts.Contracts().size() == 0)
        ETH_ERROR_MESSAGE("Compiling solc: bytecode prefix `" + codeNamePrefix + "` not found in the result output!");
    fs::remove_all(path);
    return contracts;
#endif
}


/// This function is called for every account "code" : field in Fillers
/// And transaction "data" filed in Fillers
string replaceCode(string const& _code, solContracts const& _preSolidity)
{
    if (_code == "")
        return "0x";

    if (_code.substr(0, 2) == "0x" && _code.size() >= 2)
    {
        checkHexHasEvenLength(_code);
        if (Options::get().filltests && _code.size() > 2)
            ETH_WARNING("Filling raw bytecode, please provide the source!" + TestOutputHelper::get().testInfo().errorDebug());
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
        checkHexHasEvenLength(compiledCode);
    }
    else if (_code.find(c_abiPrefix) != string::npos)
    {
        size_t const pos = _code.find(c_abiPrefix);
        string const abiCode = _code.substr(pos + c_abiPrefix.length() + 1);
        compiledCode = encodeAbi(abiCode);
        checkHexHasEvenLength(compiledCode);
    }
    else
        compiledCode = compileLLL(_code);

    if (_code.size() > 0)
        ETH_FAIL_REQUIRE_MESSAGE(
            compiledCode.size() > 0, "Bytecode is missing! '" + _code + "' " + TestOutputHelper::get().testName());
    return compiledCode;
}
}  // namespace test
