#include "DebugStorageRangeAt.h"
#include <retesteth/EthChecks.h>

namespace test
{
namespace teststruct
{
DebugStorageRangeAt::DebugStorageRangeAt(DataObject const& _data)
{
    try
    {
        /*
         *  "complete" : true,
            "storage":{
                "0x290decd9548b62a8d60345a988386fc84ba6bc95484008f6362f93160ef3e563":{
                        "key":"0x00",
                        "value":"0x02"
                 },
                "0xbcf871628f1983295a390f19bd04e30059c38e1c029e3ee86e7b105aef438a30":{
                        "key":"0x0157",
                        "value":"0x9a"
                }
             }
             "nextKey" : "" ???
        */

        // Convert to test storage representation
        DataObject storageCorrect;
        for (auto const& record : _data.atKey("storage").getSubObjects())
            storageCorrect[record.atKey("key").asString()] = record.atKey("value").asString();
        m_storage = spStorage(new Storage(storageCorrect));
        if (_data.count("nextKey"))
            m_nextKey = spFH32(new FH32(_data.atKey("nextKey")));
        else
        {
            ETH_WARNING_TEST("DebugStorageRangeAt key `nextKey` is not set, use `0x00..00` instead.", 6);
            m_nextKey = spFH32(new FH32(FH32::zero()));
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DebugStorageRangeAt unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
