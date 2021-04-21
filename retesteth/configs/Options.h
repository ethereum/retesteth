#pragma once
#include <string>
#include <map>
#include <dataObject/DataObject.h>

extern dataobject::DataObject map_configs;

// Attached clients config
class alethcfg { public: alethcfg(); };
class alethIpcDebugcfg { public: alethIpcDebugcfg(); };
class besucfg { public: besucfg(); };
class t8ntoolcfg { public: t8ntoolcfg(); };
class oewrapcfg { public: oewrapcfg(); };

// Genesis configs for clients
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

// Transition genesis configs
class genFrontierToHomesteadCfg { public: genFrontierToHomesteadCfg(); };
class genHomesteadToDaoCfg { public: genHomesteadToDaoCfg(); };
class genHomesteadToEIP150Cfg { public: genHomesteadToEIP150Cfg(); };
class genEIP158ToByzantiumCfg { public: genEIP158ToByzantiumCfg(); };
class genByzantiumToConstantinopleFixCfg { public: genByzantiumToConstantinopleFixCfg(); };

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
        oewrapcfg oewrap;

        // Genesis configs for clients
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

        // Transition genesis configs
        genFrontierToHomesteadCfg genFrontierToHomestead;
        genHomesteadToDaoCfg genHomesteadToDao;
        genHomesteadToEIP150Cfg genHomesteadToEIP150;
        genEIP158ToByzantiumCfg genEIP158ToByzantium;
        genByzantiumToConstantinopleFixCfg genBezantiumToConstantinopleFix;
    }
};
