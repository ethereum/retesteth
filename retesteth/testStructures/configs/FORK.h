#pragma once
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
// FORK network configuration (string wrapper)
// Keep Fork names in this structure to distinguish string variable from strings that represent forks
struct FORK : GCP_SPointerBase
{
    FORK(char const* _s) : m_data(string(_s)) {}
    FORK(std::string const& _s) : m_data(_s) {}
    FORK(DataObject const&);
    string const& asString() const { return m_data.asString(); }

    // std container operations
    inline bool operator==(FORK const& rhs) const { return asString() == rhs.asString(); }
    inline bool operator!=(FORK const& rhs) const { return !(*this == rhs); }
    inline bool operator!=(string const& rhs) const { return !(this->asString() == rhs); }
    inline bool operator!=(const char* rhs) const { return !(this->asString() == string(rhs)); }
    inline bool operator<(FORK const& rhs) const { return asString() < rhs.asString(); }

private:
    FORK() {}
    DataObject m_data;
};

enum class CMP
{
    lt,
    gt,
    le,
    ge
};
bool compareFork(FORK const& _left, CMP _t, FORK const& _right);

typedef GCP_SPointer<FORK> spFORK;

}  // namespace teststruct
}  // namespace test
