#pragma once
#include "../../basetypes.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
// Structure for RPC response test_rawTransaction
struct TestRawTransaction
{
    TestRawTransaction(DataObject const&);
    FH20 const& sender() const { return m_sender; }
    string const& error() const { return m_error; }

private:
    spFH20 m_sender;
    string m_error;
};

}  // namespace teststruct
}  // namespace test
