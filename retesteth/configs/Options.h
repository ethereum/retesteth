#pragma once
#include <libdataobj/DataObject.h>

namespace retesteth::options
{
extern dataobject::DataObject map_configs;

// Attached clients config
class alethcfg { public: alethcfg(); };
class alethIpcDebugcfg { public: alethIpcDebugcfg(); };
class besucfg { public: besucfg(); };
class t8ntoolcfg { public: t8ntoolcfg(); };
class etccfg { public: etccfg(); };
class t8ntooleipcfg { public: t8ntooleipcfg(); };
class oewrapcfg { public: oewrapcfg(); };
class ethereumjscfg { public: ethereumjscfg(); };

// Genesis configs for clients (besu/t8ntool)
class genRewardsCfg { public: genRewardsCfg(); };
class genFrontierCfg { public: genFrontierCfg(); };
class genHomesteadCfg { public: genHomesteadCfg(); };
class genEIP150Cfg { public: genEIP150Cfg(); };
class genEIP158Cfg { public: genEIP158Cfg(); };
class genByzantiumCfg { public: genByzantiumCfg(); };
class genConstantinopleCfg { public: genConstantinopleCfg(); };
class genConstantinopleFixCfg { public: genConstantinopleFixCfg(); };
class genIstanbulCfg { public: genIstanbulCfg(); };
class genBerlinCfg { public: genBerlinCfg(); };
class genLondonCfg { public: genLondonCfg(); };
class genArrowGlacierCfg { public: genArrowGlacierCfg(); };
class genGrayGlacierCfg { public: genGrayGlacierCfg(); };
class genMergeCfg { public: genMergeCfg(); };
class genArrowGlacierToMergeAtDiffC0000Cfg { public: genArrowGlacierToMergeAtDiffC0000Cfg(); };

// Genesis configs for clients (etc)
class genRewardsCfgETC { public: genRewardsCfgETC(); };
class genFrontierCfgETC { public: genFrontierCfgETC(); };
class genHomesteadCfgETC { public: genHomesteadCfgETC(); };
class genEIP150CfgETC { public: genEIP150CfgETC(); };
class genEIP158CfgETC { public: genEIP158CfgETC(); };
class genAtlantisCfgETC { public: genAtlantisCfgETC(); };
class genAghartaCfgETC { public: genAghartaCfgETC(); };
class genPhoenixCfgETC { public: genPhoenixCfgETC(); };
class genMagnetoCfgETC { public: genMagnetoCfgETC(); };
class genMystiqueCfgETC { public: genMystiqueCfgETC(); };


// T8ntooleip
class t8ntooleip_genRewardsCfg { public: t8ntooleip_genRewardsCfg(); };
class t8ntooleip_genLondon1884Cfg { public: t8ntooleip_genLondon1884Cfg(); };

// Transition genesis configs
class genFrontierToHomesteadCfg { public: genFrontierToHomesteadCfg(); };
class genHomesteadToDaoCfg { public: genHomesteadToDaoCfg(); };
class genHomesteadToEIP150Cfg { public: genHomesteadToEIP150Cfg(); };
class genEIP158ToByzantiumCfg { public: genEIP158ToByzantiumCfg(); };
class genByzantiumToConstantinopleFixCfg { public: genByzantiumToConstantinopleFixCfg(); };
class genBerlinToLondonCfg { public: genBerlinToLondonCfg(); };

// Initializer
class OptionsInit
{
public:
    OptionsInit() {

        // Attached clients config
        alethcfg aleth;
        alethIpcDebugcfg alethIpcDebug;
        besucfg besu;
        t8ntoolcfg t8ntool;
        etccfg etc;
        t8ntooleipcfg t8ntooleip;
        oewrapcfg oewrap;
        ethereumjscfg ethereumjs;

        // Genesis configs for clients (besu/t8ntool)
        genRewardsCfg rewards;
        genFrontierCfg genFrontier;
        genHomesteadCfg genHomestead;
        genEIP150Cfg genEIP150;
        genEIP158Cfg genEIP158;
        genByzantiumCfg genByzantium;
        genConstantinopleCfg genConstantinople;
        genConstantinopleFixCfg genConstantinopleFix;
        genIstanbulCfg genIstanbuil;
        genBerlinCfg genBerlin;
        genLondonCfg genLondon;
        genArrowGlacierCfg genArrowGlacier;
        genGrayGlacierCfg genGrayGlacier;
        genMergeCfg genMerge;

        // Genesis configs for clients (etc)
        genRewardsCfgETC rewardsETC;
        genFrontierCfgETC genFrontierETC;
        genHomesteadCfgETC genHomesteadETC;
        genEIP150CfgETC genEIP150ETC;
        genEIP158CfgETC genEIP158ETC;
        genAtlantisCfgETC genAtlantisETC;
        genAghartaCfgETC genAghartaETC;
        genPhoenixCfgETC genPhoenixETC;
        genMagnetoCfgETC genMagnetoETC;
        genMystiqueCfgETC genMystiqueETC;

        // T8ntooleip
        t8ntooleip_genRewardsCfg t8ntooleip_RewardsCfg;
        t8ntooleip_genLondon1884Cfg t8ntooleip_London1884Cfg;

        // Transition genesis configs
        genFrontierToHomesteadCfg genFrontierToHomestead;
        genHomesteadToDaoCfg genHomesteadToDao;
        genHomesteadToEIP150Cfg genHomesteadToEIP150;
        genEIP158ToByzantiumCfg genEIP158ToByzantium;
        genByzantiumToConstantinopleFixCfg genBezantiumToConstantinopleFix;
        genBerlinToLondonCfg genBerlinToLondon;
        genArrowGlacierToMergeAtDiffC0000Cfg genArrowGlacierToMergeAtDiffC0000;
    }
};
}  // namespace retesteth::options
