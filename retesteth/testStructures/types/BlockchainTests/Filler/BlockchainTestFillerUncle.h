#pragma once
#include "../../../basetypes.h"
#include "../../../configs/FORK.h"
#include "../../Ethereum/BlockHeaderIncomplete.h"
#include "../../Ethereum/Transaction.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>

namespace test
{
namespace teststruct
{
enum class UncleType
{
    SameAsBlock,
    PopulateFromBlock,
    SameAsPreviousBlockUncle,
    SameAsPreviousSibling
};


struct BlockchainTestFillerUncle : GCP_SPointerBase
{
    BlockchainTestFillerUncle(DataObject const&);
    UncleType uncleType() const { return m_typeOfUncleSection; }
    size_t sameAsPreviousBlockUncle() const { return m_sameAsPreviousBlockUncle; }
    size_t populateFromBlock() const { return m_populateFromBlock; }
    size_t sameAsBlock() const { return m_sameAsBlock; }
    size_t sameAsPreviousSibling() const { return m_sameAsPreviousSibling; }

    std::string const& chainname() const { return m_chainName; }
    bool hasRelTimestampFromPopulateBlock() const { return m_hasRelTimestampFromPopulateBlock; }
    long long int relTimestampFromPopulateBlock() const { return m_relTimestampFromPopulateBlock; }

    bool hasOverwriteHeader() const { return !m_headerIncomplete.isEmpty(); }
    BlockHeaderIncomplete const& overwriteHeader() const { return m_headerIncomplete; }

private:
    BlockchainTestFillerUncle() {}

    UncleType m_typeOfUncleSection;
    size_t m_sameAsPreviousBlockUncle;
    size_t m_populateFromBlock;
    size_t m_sameAsBlock;
    size_t m_sameAsPreviousSibling;

    bool m_hasRelTimestampFromPopulateBlock = false;
    long long int m_relTimestampFromPopulateBlock;
    std::string m_chainName;

    // Header incomplete that maps which fields to overwrite in uncle header
    spBlockHeaderIncomplete m_headerIncomplete;
};

}  // namespace teststruct
}  // namespace test
