#pragma once
#include <libdataobj/DataObject.h>
#include <configs/ForEachMacro.h>

namespace retesteth::options
{
extern dataobject::DataObject map_configs;
extern std::string const yul_compiler_sh;
extern std::string const t8ntool_start;

#define REGISTER(X) \
    class gen##X { public: gen##X(); };
#define INIT(X) \
    gen##X X##instance;

#define DECLARE_T8NTOOL(X) \
  FOR_EACH(X, t8ntoolcfg, RewardsCfg, FrontierCfg, HomesteadCfg, EIP150Cfg, EIP158Cfg, ByzantiumCfg, \
    ConstantinopleCfg, ConstantinopleFixCfg, IstanbulCfg, BerlinCfg, LondonCfg, MergeCfg, \
    ShanghaiCfg, CancunCfg, \
    ArrowGlacierCfg, GrayGlacierCfg, ArrowGlacierToMergeAtDiffC0000Cfg, MergeToShanghaiAtTime15kCfg, \
    ShanghaiToCancunAtTime15kCfg, \
    FrontierToHomesteadCfg, HomesteadToDaoCfg, HomesteadToEIP150Cfg, EIP158ToByzantiumCfg, \
    ByzantiumToConstantinopleFixCfg, BerlinToLondonCfg )

#define DECLARE_ETC(X) \
  FOR_EACH(X, etccfg, RewardsCfgETC, AtlantisCfgETC, AghartaCfgETC, PhoenixCfgETC, MagnetoCfgETC, \
    MystiqueCfgETC, Mystique2CfgETC, Mystique3CfgETC)

#define DECLARE_NIMBUS(X) \
  FOR_EACH(X, nimbuscfg, RewardsCfgNIMBUS, MergeCfgNIMBUS)

#define DECLARE_PYT8N(X) \
  FOR_EACH(X, pyt8ncfg, RewardsCfgPYT8N, ConstantinopleFixCfgPYT8N)

#define DECLARE_ETCTR(X) \
  FOR_EACH(X, etctranslatecfg, RewardsCfgETCTR, ByzantiumCfgETCTR, ConstantinopleCfgETCTR, ConstantinopleFixCfgETCTR, \
    IstanbulCfgETCTR, BerlinCfgETCTR, LondonCfgETCTR, MergeCfgETCTR)

#define DECLARE_ALETH(X) \
  FOR_EACH(X, alethcfg)

#define DECLARE_ALETHIPC(X) \
  FOR_EACH(X, alethIpcDebugcfg)

#define DECLARE_BESU(X) \
  FOR_EACH(X, besucfg)

#define DECLARE_EVMONE(X) \
  FOR_EACH(X, evmonecfg)

#define DECLARE_T8NTOOL_EIP(X) \
  FOR_EACH(X, t8ntooleipcfg, t8ntooleip_genRewardsCfg, t8ntooleip_genLondon1884Cfg)

#define DECLARE_OEWRAP(X) \
  FOR_EACH(X, oewrapcfg)

#define DECLARE_ETHJS(X) \
  FOR_EACH(X, ethereumjscfg)


// Main Configs (T8NTOOL is the base config that puts in to default for other clients as well)
DECLARE_T8NTOOL(REGISTER)
DECLARE_BESU(REGISTER)
DECLARE_EVMONE(REGISTER)
DECLARE_ETC(REGISTER)
DECLARE_ETCTR(REGISTER)
DECLARE_OEWRAP(REGISTER)
DECLARE_ETHJS(REGISTER)
DECLARE_NIMBUS(REGISTER)
DECLARE_PYT8N(REGISTER)

// Example configs
DECLARE_T8NTOOL_EIP(REGISTER)
DECLARE_ALETH(REGISTER)
DECLARE_ALETHIPC(REGISTER)

// Initializer
class OptionsInit
{
public:
    OptionsInit() {
        DECLARE_T8NTOOL(INIT)
        DECLARE_BESU(INIT)
        DECLARE_EVMONE(INIT)
        DECLARE_ETC(INIT)
        DECLARE_ETCTR(INIT)
        DECLARE_OEWRAP(INIT)
        DECLARE_ETHJS(INIT)
        DECLARE_NIMBUS(INIT)
        DECLARE_PYT8N(INIT)

        DECLARE_T8NTOOL_EIP(INIT)
        DECLARE_ALETH(INIT)
        DECLARE_ALETHIPC(INIT)
    }
};
}  // namespace retesteth::options
