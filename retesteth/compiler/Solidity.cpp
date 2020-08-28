#include "Compiler.h"
#include "TestHelper.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
using namespace dev;
using namespace test;
using namespace dataobject;

// Encode Solidity abi
string test::compiler::utiles::encodeAbi(string const& _code)
{
    string abi;
    std::vector<string> args = test::explode(_code, ' ');
    if (args.size() == 0)
        ETH_ERROR_MESSAGE("encodeAbi expected at least 1 argument!");

    std::vector<string> types;
    for (size_t i = 0; i < args.size(); i++)
    {
        if (i == 0)
        {
            // hash of the function call arg
            abi += dev::sha3(args.at(0)).hex().substr(0, 8);

            // read the function arg types
            size_t pos = args.at(0).find("(");
            if (pos == string::npos)
                ETH_ERROR_MESSAGE("encodeAbi: `(` not found in function name!");
            string argTypes = args.at(0).substr(pos + 1);
            argTypes = argTypes.substr(0, argTypes.length() - 1);
            types = test::explode(argTypes, ',');

            if (types.size() != args.size() - 1)
                ETH_ERROR_MESSAGE("encodeAbi: wrong number of function arguments provided!");
            continue;
        }

        string const& argType = types.at(i - 1);
        if (argType == "bool")
        {
            // convert argument to int to be sure of input values
            u256 val(args.at(i));
            if (val != 0 && val != 1)
                ETH_ERROR_MESSAGE("encodeAbi: bool argument must be `0` or `1`, provided: `" + args.at(i) + "`");
            abi += test::stoCompactHex(val, 32);
        }
        else if (argType == "uint32" || argType == "uint256")
        {
            u256 val(args.at(i));
            abi += test::stoCompactHex(val, 32);
        }
        else if (argType.find("bytes") != string::npos)
        {
            string const SBytes = argType.substr(argType.find("bytes") + 5);
            size_t const arrayBracer = SBytes.find("[");
            size_t const arrayBracerEND = SBytes.find("]");

            if (arrayBracerEND == string::npos)
                ETH_ERROR_MESSAGE("encodeAbi: expected `]` bracer in encoding!");
            if (arrayBracer != string::npos)  // bytes array here
            {
                int arrayElementSize = atoi(SBytes.substr(0, arrayBracer).c_str());
                string const arrayElements = SBytes.substr(arrayBracer + 1, arrayBracerEND - arrayBracer - 1);
                size_t arrayLength = atoi(arrayElements.c_str());
                if (arrayElementSize > 32)
                    ETH_ERROR_MESSAGE("encodeAbi: Array argument can not be > 32 bytes!");

                // Format check ["abc", "def"]
                if (args.at(i).find('[') == string::npos)
                    ETH_ERROR_MESSAGE("encodeAbi: expected `[` bracer in array argument");
                if (args.at(i).find(']') == string::npos)
                    ETH_ERROR_MESSAGE("encodeAbi: expected `]` bracer in array argument");

                string const argumentsStr = args.at(i).substr(1, args.at(i).length() - 2);  // remove `[]`
                std::vector<string> arguments = test::explode(argumentsStr, ',');
                if (arguments.size() != arrayLength)
                    ETH_ERROR_MESSAGE("encodeAbi: arguments provided to the array do not match ");
                for (auto const& el : arguments)
                {
                    if (args.at(i).find('"') == string::npos)
                        ETH_ERROR_MESSAGE("encodeAbi: expected `\"` quote in array argument");
                    string dequotedArg = el.substr(1, el.length() - 2);
                    if (dequotedArg.length() > 32)
                        ETH_ERROR_MESSAGE("encodeAbi: Array argument can not be > 32 bytes!");
                    string encodedArg;
                    for (size_t j = 0; j < 32; j++)
                    {
                        if (j < dequotedArg.size())
                            encodedArg += dev::toCompactHex(dev::u256(dequotedArg.at(j)));
                        else
                            encodedArg += "00";
                    }
                    abi += encodedArg;
                }
            }
        }
    }
    ETH_LOG(abi, 7);
    return "0x" + abi;
}

namespace test
{
namespace compiler
{
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
            test::compiler::utiles::checkHexHasEvenLength(code);
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
}  // namespace compiler
}  // namespace test
