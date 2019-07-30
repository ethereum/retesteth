#include "scheme_state.h"
#include "../expectSection/scheme_expectState.h"
#include "scheme_postState.h"
#include <retesteth/TestOutputHelper.h>
#include <mutex>
using namespace  std;

namespace test {

mutex g_staticDeclaration;
DataObject scheme_state::getDataForRPC(std::string const& _network) const
{
    // Cpp specific precompiled declaraion for genesis
    static DataObject ecrecoverObj;
    static DataObject sha256Obj;
    static DataObject ripemd160Obj;
    static DataObject identityObj;
    static DataObject alt_bn128_G1_addObj;
    static DataObject alt_bn128_G1_mulObj;
    {
        std::lock_guard<std::mutex> lock(g_staticDeclaration);
        if (!ecrecoverObj.count("name"))
        {
            ecrecoverObj["name"] = "ecrecover";
            ecrecoverObj["linear"]["base"] = 3000;
            ecrecoverObj["linear"]["word"] = 0;

            sha256Obj["name"] = "sha256";
            sha256Obj["linear"]["base"] = 60;
            sha256Obj["linear"]["word"] = 12;

            ripemd160Obj["name"] = "sha256";
            ripemd160Obj["linear"]["base"] = 600;
            ripemd160Obj["linear"]["word"] = 120;

            identityObj["name"] = "identity";
            identityObj["linear"]["base"] = 15;
            identityObj["linear"]["word"] = 3;

            alt_bn128_G1_addObj["name"] = "alt_bn128_G1_add";
            alt_bn128_G1_addObj["linear"]["base"] = 500;
            alt_bn128_G1_addObj["linear"]["word"] = 0;

            alt_bn128_G1_mulObj["name"] = "alt_bn128_G1_mul";
            alt_bn128_G1_mulObj["linear"]["base"] = 40000;
            alt_bn128_G1_mulObj["linear"]["word"] = 0;
        }
    }

    bool networkChecked = false;
    if (_network == "Frontier" || _network == "Homestead" || _network == "EIP150" || _network == "EIP158")
        networkChecked = true;

    DataObject ret = m_data;
    ret["0x0000000000000000000000000000000000000001"]["precompiled"] = ecrecoverObj;
    ret["0x0000000000000000000000000000000000000002"]["precompiled"] = sha256Obj;
    ret["0x0000000000000000000000000000000000000003"]["precompiled"] = ripemd160Obj;
    ret["0x0000000000000000000000000000000000000004"]["precompiled"] = identityObj;

    if (_network == "Byzantium" || _network == "Constantinople" || _network == "ConstantinopleFix")
    {
        networkChecked = true;
        ret["0x0000000000000000000000000000000000000005"]["precompiled"]["name"] = "modexp";
        ret["0x0000000000000000000000000000000000000006"]["precompiled"] = alt_bn128_G1_addObj;
        ret["0x0000000000000000000000000000000000000007"]["precompiled"] = alt_bn128_G1_mulObj;
        ret["0x0000000000000000000000000000000000000008"]["precompiled"]["name"] =
            "alt_bn128_pairing_product";
    }

    if (!networkChecked)
        ETH_FAIL_MESSAGE("Unhandled network: " + _network + " (scheme_state::getDataForRPC)");
    return ret;
}
} // namespace test
