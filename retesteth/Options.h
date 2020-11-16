
#pragma once

#include <libdevcore/Exceptions.h>
#include <retesteth/configs/ClientConfig.h>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

using namespace dev;
namespace test
{
class TestOptions;
class Options
{
public:
    struct InvalidOption : public Exception
    {
        InvalidOption(std::string _message = std::string()) : Exception(_message) {}
    };

    struct DynamicOptions
    {
        DynamicOptions() {}
        std::vector<ClientConfig> const& getClientConfigs();
        ClientConfig const& getCurrentConfig() const;
        void setCurrentConfig(ClientConfig const& _config);
        size_t activeConfigs() const;
        bool currentConfigIsSet() const;

    private:
        std::vector<ClientConfig> m_clientConfigs;
        test::ClientConfigID m_currentConfigID = test::ClientConfigID::null();
    };

    size_t threadCount = 1;	///< Execute tests on threads
    bool enableClientsOutput = false;  ///< Enable stderr from clients
    bool vmtrace = false;              ///< Create EVM execution tracer
    bool vmtraceraw = false;           ///< Create EVM execution tracer. output raw info
    bool filltests = false;            ///< Create JSON test files from execution results
    bool showhash = false;  ///< Show filler hash for debug information
    bool checkhash = false; ///< Check that tests are updated from fillers
    size_t blockLimit = 0;  ///< Perform blockchain blocks till this limit
    size_t rpcLimit = 0;    ///< Perform rpcRequests till this limit
    bool fillchain = false; ///< Fill tests as a blockchain tests if possible
    bool stats = false;     ///< Execution time and stats for state tests
    bool poststate = false;
    std::string statsOutFile; ///< Stats output file. "out" for standard output
    fs::path datadir;         ///< Path to datadir (~/.retesteth)
    std::vector<IPADDRESS> nodesoverride;  ///< ["IP:port", ""IP:port""] array
    bool exectimelog = false; ///< Print execution time for each test suite
	std::string rCurrentTestSuite; ///< Remember test suite before boost overwrite (for random tests)
    bool statediff = false;        ///< Fill full post state in General tests
    bool fullstate = false;        ///< Replace large state output to it's hash
    bool createRandomTest = false; ///< Generate random test
    bool travisOutThread = false;  ///< Output `.` to std:out when running tests
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
    boost::optional<std::string> singleTestFile;    // --testfile run a single file
    boost::optional<std::string> singleTestOutFile; // --testfile run a single file filler output
    std::string singleTestName;     // A test name (usually a file.json test)
    std::string singleSubTestName;  // A test name inside a file.json (for blockchain tests)
    std::string singleTestNet;
    std::string trDataLabel;  ///< GeneralState data
    int trDataIndex;    ///< GeneralState data
    int trGasIndex;     ///< GeneralState gas
    int trValueIndex;   ///< GeneralState value
    bool all = false;	///< Running every test, including time consuming ones.
    bool lowcpu = false; ///< Disable cpu-intense tests
	bool nonetwork = false;///< For libp2p
	/// @}

	/// Get reference to options
	/// The first time used, options are parsed with argc, argv
	static Options const& get(int argc = 0, const char** argv = 0);
    static DynamicOptions& getDynamicOptions() { return m_dynamicOptions; }
    string getGStateTransactionFilter() const;

private:
	Options(int argc = 0, const char** argv = 0);
	Options(Options const&) = delete;
    static DynamicOptions m_dynamicOptions;
    friend class TestOptions;
};

} //namespace test
