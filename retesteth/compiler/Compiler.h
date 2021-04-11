#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <string>
using namespace dataobject;

namespace test
{
namespace compiler
{
namespace utiles
{
/// verify compiled code length
void checkHexHasEvenLength(string const& _hex);

/// ecnode abi options into bytecode
string encodeAbi(string const& _code);

}  // namespace utiles

/// Compile solidity source code into solidity information
class solContracts
{
public:
    void insertCode(string const& _name, string const& _code) { m_solContracts[_name] = _code; }
    string const& getCode(string const& _contractName) const;
    std::vector<DataObject> const& Contracts() const { return m_solContracts.getSubObjects(); }

private:
    // Map of "contractName" -> "bytecode"
    DataObject m_solContracts;
};

/// get solContracts information from solidity source code
solContracts compileSolidity(string const& _code);

/// compile LLL / wasm or other src code into bytecode
std::string replaceCode(std::string const& _code, solContracts const& _preSolidity = solContracts());

/// compile clean YUL
std::string compileYul(string const& _code);

}  // namespace compiler
}  // namespace test
