#include "Compiler.h"
#include "TestHelper.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
using namespace dev;
using namespace test;
using namespace dataobject;

namespace
{
// parse array info in abi
struct arrayLengths
{
    size_t typeSize;
    size_t elementsCount;
};

arrayLengths parseArray(string const& _input, size_t _maxElementSize)
{
    arrayLengths info;

    // staff like bytes3[2] into  typeSize: `3` and  elementsSize: `2`
    size_t const arrayBracer = _input.find("[");
    size_t const arrayBracerEND = _input.find("]");
    if (arrayBracerEND == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi: expected `]` bracer in header encoding!");

    size_t arrayElementSize = atoi(_input.substr(0, arrayBracer).c_str());
    string const arrayElements = _input.substr(arrayBracer + 1, arrayBracerEND - arrayBracer - 1);
    size_t arrayLength = arrayElements.size() > 0 ? atoi(arrayElements.c_str()) : 0;
    if (arrayElementSize > _maxElementSize)
        ETH_ERROR_MESSAGE("encodeAbi: Array header argument can not be > " + test::fto_string(_maxElementSize) + " bytes!");

    info.typeSize = arrayElementSize;
    info.elementsCount = arrayLength;
    return info;
}

std::vector<string> parseArgumentArray(string const& _input)
{
    // Format check ["abc", "def"]
    if (_input.find('[') == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi: expected `[` bracer in array argument");
    if (_input.find(']') == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi: expected `]` bracer in array argument");

    string const argumentsStr = _input.substr(1, _input.length() - 2);  // remove `[]`
    return test::explode(argumentsStr, ',');
}

// Encode "dave" into bytestring padded32
string encodeByteString(string const& _input, size_t _expectedSize = 0)
{
    if (_input.find('"') == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi:encodeByteString: expected `\"` quote in argument");
    string const dequotedArg = _input.substr(1, _input.length() - 2);

    // Check the argument with the provided header
    if (dequotedArg.length() > 32)
        ETH_ERROR_MESSAGE("encodeAbi: Array argument can not be > 32 bytes!");
    if (dequotedArg.length() != _expectedSize && _expectedSize != 0)
        ETH_ERROR_MESSAGE("encodeAbi: Array argument size is different from the one described in the header!");

    string encodedArg;
    for (size_t i = 0; i < 32; i++)
    {
        if (i < dequotedArg.size())
            encodedArg += dev::toCompactHex(dev::u256(dequotedArg.at(i)));
        else
            encodedArg += "00";
    }
    return encodedArg;
}
}  // namespace

string const& test::compiler::solContracts::getCode(string const& _contractName) const
{
    if (!m_solContracts.count(_contractName))
        ETH_ERROR_MESSAGE("solContracts::getCode: error contract name: `" + _contractName +
                          "` not found! (Specify contract in \"solidity\": section of the test filler)");
    return m_solContracts.atKey(_contractName).asString();
}

// Encode Solidity abi
string test::compiler::utiles::encodeAbi(string const& _code)
{
    string abi, abiSuffix;
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
        else if (argType.find("uint256[") != string::npos)
        {
            // uint array
            string const SBytes = argType.substr(argType.find("uint256") + 4);
            arrayLengths arrayHeader = parseArray(SBytes, 256);
            std::vector<string> arguments = parseArgumentArray(args.at(i));

            // Put the pointer
            abi += dev::toCompactHex(dev::u256(types.size() * 32 + (i - 1) * 32), 32);
            abiSuffix += dev::toCompactHex(dev::u256(arguments.size()), 32);
            for (auto const& el : arguments)
                abiSuffix += dev::toCompactHex(dev::u256(el), 32);
            (void)arrayHeader;
        }
        else if (argType.find("bytes") != string::npos)
        {
            string const SBytes = argType.substr(argType.find("bytes") + 5);

            if (SBytes.find("[") != string::npos)  // bytes array here
            {
                arrayLengths arrayHeader = parseArray(SBytes, 32);
                std::vector<string> arguments = parseArgumentArray(args.at(i));

                if (arguments.size() != arrayHeader.elementsCount)
                    ETH_ERROR_MESSAGE("encodeAbi: arguments provided to the array do not match ");
                for (auto const& el : arguments)
                    abi += encodeByteString(el, arrayHeader.typeSize);
            }
            else
            {
                // Dynamic bytes array, need to put a pointer
                abi += dev::toCompactHex(dev::u256(types.size() * 32 + (i - 1) * 32), 32);
                size_t encodingSize = args.at(i).size() - 2;
                abiSuffix += dev::toCompactHex(dev::u256(encodingSize), 32) + encodeByteString(args.at(i));
            }
        }
    }
    ETH_LOG("0x" + abi + abiSuffix, 7);
    return "0x" + abi + abiSuffix;
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
