#pragma once
#include <libdataobj/DataObject.h>
#include <string>

namespace test::compiler
{
namespace utiles
{
/// verify compiled code length
void checkHexHasEvenLength(std::string const& _hex);

/// ecnode abi options into bytecode
std::string encodeAbi(std::string const& _code);

}  // namespace utiles


using namespace dataobject;

/// Compile solidity source code into solidity information
class solContracts
{
public:
    solContracts() {}
    void insertCode(std::string const& _name, std::string const& _code) { m_solContracts.getContent()[_name] = _code; }
    std::string const& getCode(std::string const& _contractName) const;
    std::vector<spDataObject> const& Contracts() const { return m_solContracts->getSubObjects(); }

private:
    // Map of "contractName" -> "bytecode"
    spDataObject m_solContracts;
};

/// get solContracts information from solidity source code
solContracts compileSolidity(std::string const& _code);

/// compile LLL / wasm or other src code into bytecode
std::string replaceCode(std::string const& _code, solContracts const& _preSolidity = solContracts());

/// compile clean YUL
std::string compileYul(std::string const& _code);

}  // namespace compiler
