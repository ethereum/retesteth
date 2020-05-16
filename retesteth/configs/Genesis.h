#include <string>

// Fork configs
extern const std::string default_Frontier_config;
extern const std::string default_Homestead_config;
extern const std::string default_EIP150_config;
extern const std::string default_EIP158_config;
extern const std::string default_Byzantium_config;
extern const std::string default_Constantinople_config;
extern const std::string default_ConstantinopleFix_config;
extern const std::string default_Istanbul_config;
// t8ntool
extern const std::string t8ntool_config;
extern const std::string t8ntool_Frontier_config;
extern const std::string t8ntool_Homestead_config;
extern const std::string t8ntool_EIP150_config;
extern const std::string t8ntool_EIP158_config;
extern const std::string t8ntool_Byzantium_config;
extern const std::string t8ntool_Constantinople_config;
extern const std::string t8ntool_ConstantinopleFix_config;
extern const std::string t8ntool_Istanbul_config;

// Transition test configs
extern const std::string default_FrontierToHomesteadAt5_config;
extern const std::string default_HomesteadToDaoAt5_config;
extern const std::string default_HomesteadToEIP150At5_config;
extern const std::string default_EIP158ToByzantiumAt5_config;
extern const std::string default_ByzantiumToConstantinopleFixAt5_config;
// t8ntool
extern const std::string t8ntool_FrontierToHomesteadAt5_config;
extern const std::string t8ntool_HomesteadToDaoAt5_config;
extern const std::string t8ntool_HomesteadToEIP150At5_config;
extern const std::string t8ntool_EIP158ToByzantiumAt5_config;
extern const std::string t8ntool_ByzantiumToConstantinopleFixAt5_config;

// Correct mining reward config for state tests
extern const std::string default_correctMiningReward_config;

// Clients Config
extern const std::string default_config;        // Default geth retesteth
extern const std::string t8ntool_config;        // geth state_t8n tool
extern const std::string besu_config;           // besu config
extern const std::string alethTCP_config;       // aleth via TCP (not supported)
extern const std::string alethIPCDebug_config;  // aleth IPC for test debugging
extern const std::string aleth_config;          // aleth IPC for test run
extern const std::string aleth_config_sh;       // (shell for aleth_config)
