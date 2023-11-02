#include <retesteth/helpers/TestHelper.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/EthChecks.h>
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace dataobject;
using namespace std;
namespace fs = boost::filesystem;

namespace
{
/// Possible solidity types, that can occure in abi signature
enum class ArgSignatureType
{
    UNIT,
    BYTES,
    BOOL,
    STRING,
    ADDRESS,
    UNKNOWN
};

/// Argument signature information
struct ArgumentSignature
{
    ArgumentSignature()
    {
        typeSize = 0;
        dynamicArray = false;
        dynamicArray2 = false;
        elementsCount = 0;
        elementsCount2 = 0;
        type = ArgSignatureType::UNKNOWN;
    }
    size_t typeSize;
    bool dynamicArray;
    bool dynamicArray2;
    size_t elementsCount;
    size_t elementsCount2;
    ArgSignatureType type;
};

/// Parse (uint32,unint256[23],bytes10[3][4])
ArgumentSignature parseArgumentSignature(string const& _input)
{
    ArgumentSignature info;
    info.type = ArgSignatureType::UNKNOWN;
    if (_input.find("bytes") != string::npos)
        info.type = ArgSignatureType::BYTES;
    else if (_input.find("uint") != string::npos)
        info.type = ArgSignatureType::UNIT;
    else if (_input.find("bool") != string::npos)
        info.type = ArgSignatureType::BOOL;
    else if (_input.find("string") != string::npos)
        info.type = ArgSignatureType::STRING;
    else if (_input.find("address") != string::npos)
        info.type = ArgSignatureType::ADDRESS;

    size_t openBracerCount = std::count(_input.begin(), _input.end(), '[');
    size_t closeBracerCount = std::count(_input.begin(), _input.end(), ']');
    if (openBracerCount != closeBracerCount)
        ETH_ERROR_MESSAGE("encodeAbi unclosed [] in argument signature!");
    if (openBracerCount > 2)
        ETH_ERROR_MESSAGE("encodeAbi max 2 dimensional arrays are allowed in abi argument signature!");

    // See the type size (uint32, uint256, bytes10)
    string typeSize;
    for (size_t i = 0; i < _input.length(); i++)
    {
        if (isdigit(_input.at(i)))
            typeSize += _input.at(i);
        if (_input.at(i) == '[')
            break;
    }
    info.typeSize = typeSize.size() > 0 ? atoi(typeSize.c_str()) : 0;
    if (info.typeSize > 32 && info.type == ArgSignatureType::BYTES)
        ETH_ERROR_MESSAGE("encodeAbi: bytes type can't be > 32");

    // Read Array Sizes If any
    if (_input.find("[") != string::npos)
    {
        string arraySize;
        bool firstArray = false;
        for (size_t i = _input.find("["); i < _input.size(); i++)
        {
            if (isdigit(_input.at(i)))
                arraySize += _input.at(i);
            if (_input.at(i) == ']')
            {
                if (!firstArray)
                {
                    info.elementsCount = atoi(arraySize.c_str());
                    firstArray = true;
                    arraySize = string();
                }
                else
                    info.elementsCount2 = atoi(arraySize.c_str());
            }
        }
    }

    if (info.elementsCount == 0 && _input.find("[") != string::npos)
        info.dynamicArray = true;

    if (info.elementsCount2 == 0 && openBracerCount == 2)
        info.dynamicArray2 = true;

    // `bytes` is treated as dynamic array
    if (info.type == ArgSignatureType::BYTES && info.typeSize == 0)
        info.dynamicArray = true;

    // `string[]` is treated as dynamic array 2
    if (info.type == ArgSignatureType::STRING && info.typeSize == 0)
        info.dynamicArray2 = true;

    if (info.type == ArgSignatureType::ADDRESS)
        info.typeSize = 40;

    return info;
}

/// Parse arguments into array
std::vector<string> parseArgumentArray(ArgumentSignature const& _header, string const& _input)
{
    // Format check ["abc", "def"]
    if (_input.find('[') == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi: expected `[` bracer in array argument");
    if (_input.find(']') == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi: expected `]` bracer in array argument");

    string const argumentsStr = _input.substr(1, _input.length() - 2);  // remove `[]`
    std::vector<string> arr = test::explode(argumentsStr, ',');
    if (arr.size() != _header.elementsCount && _header.elementsCount != 0)
        ETH_ERROR_MESSAGE("encodeAbi: arguments provided to the array do not match signature");

    return arr;
}

/// Parse arguments into array of arrays
typedef std::vector<string> ARGS;
std::vector<ARGS> parseArgumentArray2(ArgumentSignature const& _header, string const& _input)
{
    // Format check [[1,2],[3]]
    std::vector<ARGS> args;
    size_t openBracerCount = std::count(_input.begin(), _input.end(), '[');
    size_t closeBracerCount = std::count(_input.begin(), _input.end(), ']');
    if (openBracerCount != closeBracerCount)
        ETH_ERROR_MESSAGE("encodeAbi unclosed `[]` in argument signature!");
    if (openBracerCount == 0)
        ETH_ERROR_MESSAGE("encodeAbi array argument must have `[]`! ");
    string const argumentsStr = _input.substr(1, _input.length() - 2);  // remove `[]`

    bool wasQuote = false;
    bool wasBracer = true;
    string subArray;
    for (size_t i = 0; i < argumentsStr.size(); i++)
    {
        char const ch = argumentsStr.at(i);
        if (ch == '"')
            wasQuote = 1 - wasQuote;
        if ((ch == ' ' || ch == ',') && !wasBracer)
            continue;

        subArray += ch;
        if (ch == ']' && !wasQuote)
        {
            wasBracer = 1 - wasBracer;
            ArgumentSignature header = _header;
            header.elementsCount = _header.elementsCount2;
            ARGS subArgs = parseArgumentArray(header, subArray);
            args.push_back(subArgs);
            subArray = string();
        }
    }
    if (args.size() != _header.elementsCount && _header.elementsCount != 0)
        ETH_ERROR_MESSAGE("encodeAbi: arguments provided to the array do not match signature");
    return args;
}

string encodeAddress(string const& _input)
{
    if (_input.find('"') == string::npos || _input.find("0x") == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi:encodeAddress: expected `\"` quote and `0x` prefix in argument: " + _input);
    string const dequotedArg = _input.substr(1, _input.length() - 2).substr(2);  // remove 0x
    if (!dev::isHex(dequotedArg))
        ETH_ERROR_MESSAGE("encodeAbi:encodeAddress: argument must be hex: " + dequotedArg);
    if (dequotedArg.size() != 40)
        ETH_ERROR_MESSAGE("encodeAbi:encodeAddress: argument must be 20byte address: " + dequotedArg);
    static string c_prefix = "000000000000000000000000";
    return c_prefix + dequotedArg;
}

// Encode "dave" into bytestring padded32
string encodeByteString(string const& _input, size_t _expectedSize = 0)
{
    if (_input.find('"') == string::npos)
        ETH_ERROR_MESSAGE("encodeAbi:encodeByteString: expected `\"` quote in argument: " + _input);
    string const dequotedArg = _input.substr(1, _input.length() - 2);

    // Check the argument with the provided header
    if (dequotedArg.length() > 32)
        ETH_ERROR_MESSAGE("encodeByteString: Argument can not be > 32 bytes! `" + dequotedArg);
    if (dequotedArg.length() != _expectedSize && _expectedSize != 0)
        ETH_ERROR_MESSAGE("encodeByteString: Argument size is different from the one described in the header! `" + dequotedArg);

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

std::vector<string> parseAbiArgumentList(string const& _input)
{
    string arg;
    bool readingString = false;
    int openBracerCount = 0;
    std::vector<string> args;
    for (size_t i = 0; i < _input.length(); i++)
    {
        if (_input[i] == ' ' && !readingString)
        {
            if (openBracerCount > 0)  // ignore spaces inside [] block
                continue;
            args.push_back(arg);
            arg = string();
            continue;
        }
        if (_input[i] == '"')
            readingString = 1 - readingString;
        if (_input[i] == '[')
            openBracerCount++;
        if (_input[i] == ']')
            openBracerCount--;
        arg += _input[i];
        if (i + 1 == _input.length())
            args.push_back(arg);
    }

    if (openBracerCount != 0)
        ETH_ERROR_MESSAGE("encodeAbi unclosed [] in argument list!");

    if (readingString)
        ETH_ERROR_MESSAGE("encodeAbi unclosed \" in argument list!");

    if (args.size() == 0)
        ETH_ERROR_MESSAGE("encodeAbi expected at least 1 argument!");

    return args;
}

/// Abi encode function
string EncodeArgument(string const& _element, ArgumentSignature const& _sig)
{
    switch (_sig.type)
    {
    case ArgSignatureType::UNIT:
        return test::stoCompactHex(u256(_element), 32);
        break;
    case ArgSignatureType::BOOL:
    {
        // Convert argument to int to be sure of input values
        u256 val(_element);
        if (val != 0 && val != 1)
            ETH_ERROR_MESSAGE("encodeAbi: bool argument must be `0` or `1`, provided: `" + _element + "`");
        return test::stoCompactHex(val, 32);
    }
    break;
    case ArgSignatureType::BYTES:
    {
        return encodeByteString(_element, _sig.typeSize);
    }
    case ArgSignatureType::STRING:
    {
        return encodeByteString(_element, _sig.typeSize);
    }
    case ArgSignatureType::ADDRESS:
    {
        return encodeAddress(_element);
    }
    break;
    default:
        ETH_ERROR_MESSAGE("EncodeArgument: unhandled ArgSignatureType!");
    }
    return string();
}

}  // namespace

string const& test::compiler::solContracts::getCode(string const& _contractName) const
{
    if (!m_solContracts->count(_contractName))
        ETH_ERROR_MESSAGE("solContracts::getCode: error contract name: `" + _contractName +
                          "` not found! (Specify contract in \"solidity\": section of the test filler)");
    return m_solContracts->atKey(_contractName).asString();
}

// Encode Solidity abi
string test::compiler::utiles::encodeAbi(string const& _code)
{
    ETH_DC_MESSAGE(DC::LOWLOG, _code);
    string abi, abiSuffix;

    try
    {
        std::vector<string> args = parseAbiArgumentList(_code);
        std::vector<string> argsSignature;
        for (size_t i = 0; i < args.size(); i++)
        {
            if (i == 0)
            {
                // Hash of the function call arg
                abi += dev::sha3(args.at(0)).hex().substr(0, 8);

                // Read the function arg types
                size_t pos = args.at(0).find("(");
                if (pos == string::npos)
                    ETH_ERROR_MESSAGE("encodeAbi: `(` not found in function name!");
                string argTypes = args.at(0).substr(pos + 1);
                argTypes = argTypes.substr(0, argTypes.length() - 1);
                argsSignature = test::explode(argTypes, ',');
                if (argsSignature.size() != args.size() - 1)
                    ETH_ERROR_MESSAGE("encodeAbi: wrong number of function arguments provided!");
                continue;
            }

            string const& argData = args.at(i);
            ArgumentSignature const argSignature = parseArgumentSignature(argsSignature.at(i - 1));

            // Encode the argument
            if (argSignature.elementsCount2 || argSignature.dynamicArray2)
            {
                if (argSignature.type == ArgSignatureType::STRING)
                {
                    // string is array2
                    std::vector<string> arguments = parseArgumentArray(argSignature, argData);
                    if (argsSignature.size() != 1)
                    {
                        abi += dev::toCompactHex(dev::u256(argsSignature.size() * 32 + abiSuffix.length() / 2), 32);
                        abiSuffix += dev::toCompactHex(dev::u256(arguments.size()), 32);
                    }
                    string subSuffix;
                    for (size_t i = 0; i < arguments.size(); i++)
                    {
                        abiSuffix += dev::toCompactHex(dev::u256(arguments.size() * 32 + subSuffix.length() / 2), 32);
                        subSuffix += dev::toCompactHex(dev::u256(arguments.at(i).size() - 2), 32);
                        subSuffix += EncodeArgument(arguments.at(i), argSignature);
                    }
                    abiSuffix += subSuffix;
                    continue;
                }

                std::vector<ARGS> arguments = parseArgumentArray2(argSignature, argData);
                if (argsSignature.size() != 1)
                {
                    // If not the last element, put a pointer and write to suffix
                    abi += dev::toCompactHex(dev::u256(argsSignature.size() * 32 + abiSuffix.length() / 2), 32);
                    abiSuffix += dev::toCompactHex(dev::u256(arguments.size()), 32);
                }

                // put offsets of the subArrays to encoding
                string subSuffix;
                for (size_t i = 0; i < arguments.size(); i++)
                {
                    abiSuffix += dev::toCompactHex(dev::u256(arguments.size() * 32 + subSuffix.length() / 2), 32);

                    // bytes/char sub arrays???

                    // Size of subArray
                    subSuffix += dev::toCompactHex(dev::u256(arguments.at(i).size()), 32);
                    for (auto const& el : arguments.at(i))
                        subSuffix += EncodeArgument(el, argSignature);
                }
                abiSuffix += subSuffix;
            }
            else if (argSignature.elementsCount || argSignature.dynamicArray)
            {
                // bytes are also an array but argument is just a "quoted string"
                if (argSignature.type == ArgSignatureType::BYTES && argSignature.typeSize == 0 &&
                    argSignature.elementsCount == 0)
                {
                    if (argsSignature.size() != 1)
                    {
                        // If not the last element, put a pointer and write to suffix
                        abi += dev::toCompactHex(dev::u256(argsSignature.size() * 32 + abiSuffix.length() / 2), 32);
                        if (argData.size() < 2)
                            ETH_ERROR_MESSAGE("encodeAbi: argument for bytes must be a \"quoted string\"!");
                        abiSuffix += dev::toCompactHex(dev::u256(argData.size() - 2), 32);
                    }
                    abiSuffix += EncodeArgument(argData, argSignature);
                    continue;
                }

                // Encode a dynamic Array or a static Array
                std::vector<string> arguments = parseArgumentArray(argSignature, argData);

                // Put the pointer
                if (argsSignature.size() != 1)
                {
                    // If not the last element, put a pointer and write to suffix
                    abi += dev::toCompactHex(dev::u256(argsSignature.size() * 32 + abiSuffix.length() / 2), 32);
                    abiSuffix += dev::toCompactHex(dev::u256(arguments.size()), 32);
                }
                for (auto const& el : arguments)
                    abiSuffix += EncodeArgument(el, argSignature);
            }
            else
                abi += EncodeArgument(argData, argSignature);
        }
    }
    catch (std::exception const& _ex)
    {
        throw test::UpwardsException(string("encodeAbi error: ") + _ex.what());
    }
    ETH_DC_MESSAGE(DC::LOWLOG, "0x" + abi + abiSuffix);
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
    string evmVersion;
    string const versionComment = "RETESTETH_SOLC_EVM_VERSION=";
    size_t pos = _code.find(versionComment);
    if (pos != string::npos)
    {
        size_t const endl = _code.find('\n', pos + versionComment.size());
        if (endl != string::npos)
        {
            evmVersion = "--evm-version ";
            evmVersion += _code.substr(pos + versionComment.size(), endl - pos - versionComment.size());
        }
    }
    fs::path const path(fs::temp_directory_path() / fs::unique_path());
    string const cmd = string("solc " + evmVersion + " --bin-runtime ") + path.string();
    writeFile(path.string(), _code);
    int exitCode;
    string result = executeCmd(cmd, exitCode);

    solContracts contracts;
    string const codeNamePrefix = "=======";
    string const codeBytePrefix = "Binary of the runtime part:";

    pos = result.find(codeNamePrefix);
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
