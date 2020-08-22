#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <string>
using namespace dataobject;

namespace test
{
// Compile solidity source code into solidity information
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

solContracts compileSolidity(string const& _code);

/// compile LLL / wasm or other src code into bytecode
std::string replaceCode(std::string const& _code, solContracts const& _preSolidity = solContracts());


}  // namespace test
