
#pragma once

#include <libdevcore/Exceptions.h>
#include <retesteth/ClientConfig.h>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

using namespace dev;
namespace test
{

class Options
{
public:
	struct InvalidOption: public Exception
	{
		InvalidOption(std::string _message = std::string()): Exception(_message) {}
	};

    struct DynamicOptions
    {
        DynamicOptions() {}
        std::vector<ClientConfig> const& getClientConfigs();
        ClientConfig const& getCurrentConfig() const;
        void setCurrentConfig(ClientConfig const& _config);

    private:
        std::vector<ClientConfig> m_clientConfigs;
        unsigned m_currentConfig;
    };

    size_t threadCount = 1;	///< Execute tests on threads
	bool enableClientsOutput = false; ///< Enable stderr from clients
	bool vmtrace = false;	///< Create EVM execution tracer
	bool filltests = false; ///< Create JSON test files from execution results
	bool fillchain = false; ///< Fill tests as a blockchain tests if possible
	bool stats = false;		///< Execution time and stats for state tests
    bool poststate = false;
    std::string statsOutFile; ///< Stats output file. "out" for standard output
	bool exectimelog = false; ///< Print execution time for each test suite
	std::string rCurrentTestSuite; ///< Remember test suite before boost overwrite (for random tests)
	bool statediff = false;///< Fill full post state in General tests
	bool fulloutput = false;///< Replace large output to just it's length
	bool createRandomTest = false; ///< Generate random test
	boost::optional<uint64_t> randomTestSeed; ///< Define a seed for random test
	bool jsontrace = false; ///< Vmtrace to stdout in json format
	//eth::StandardTrace::DebugOptions jsontraceOptions; ///< output config for jsontrace
	std::string testpath;	///< Custom test folder path
    unsigned logVerbosity = 1;
	boost::optional<boost::filesystem::path> randomCodeOptionsPath; ///< Options for random code generation in fuzz tests
    std::vector<std::string> clients;                               ///< Clients to work with

    /// Test selection
	/// @{
	bool singleTest = false;
	boost::optional<std::string> singleTestFile;
	std::string singleTestName;
	std::string singleTestNet;
    int trDataIndex;	///< GeneralState data
    int trGasIndex;		///< GeneralState gas
    int trValueIndex;	///< GeneralState value
	bool all = false;	///< Running every test, including time consuming ones.
	bool nonetwork = false;///< For libp2p
	/// @}

	/// Get reference to options
	/// The first time used, options are parsed with argc, argv
	static Options const& get(int argc = 0, const char** argv = 0);
    static DynamicOptions& getDynamicOptions() { return m_dynamicOptions; }

private:
	Options(int argc = 0, const char** argv = 0);
	Options(Options const&) = delete;
    static DynamicOptions m_dynamicOptions;
};

} //namespace test
