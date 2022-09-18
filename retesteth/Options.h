
#pragma once

#include <libdevcore/Exceptions.h>
#include <retesteth/configs/ClientConfig.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

using namespace dev;
namespace test
{
class TestOptions;
class Options
{
private:
    enum class ARGS
    {
        NONE,
        ONE,
        ONEMERGED,
        TWO
    };

    class Option
    {
    public:
        bool initialized() const { return m_inited; }
        void setDefHelp(string&& _def, std::function<void()> _help);
        void setBeforeSeparator() { m_allowBeforeSeparator = true; }
        void setOverrideOption() { m_optionOverrides = true; }
        void tryInit(const char** _argv, size_t _argc);
        void printHelp();
    private:
        void initArgs(const char** _argv, size_t _argc, string const& _arg, size_t& _i);
        bool isAfterSeparatorOption() const;
        bool match(string const& _arg) const;
    protected:
        virtual void initArg(string const& _arg) = 0;
        Option(){};
        string m_sOptionHelp;
        string m_sOptionName;
        ARGS m_argType;
        bool m_allowBeforeSeparator = false;
        bool m_optionOverrides = false;
        bool m_inited = false;
        std::function<void()> m_printHelpFunc;
    };

    struct void_opt : public Option
    {
        void_opt() { m_argType = ARGS::NONE; }
    protected:
        void initArg(string const& _arg) override { (void) _arg; }
    };

    struct sizet_opt : public Option
    {
        sizet_opt(int _arg) { m_argType = ARGS::ONEMERGED; m_arg = (size_t)_arg; }
        operator size_t() const { return m_arg; }
    protected:
        size_t m_arg;
        void initArg(string const& _arg) override {
            m_arg = max(1, atoi(_arg.c_str()));
        }
    };

    struct string_opt : public Option, string
    {
        string_opt() { m_argType = ARGS::ONE;}
    protected:
        void initArg(string const& _arg) override {
            assign(_arg);
        }
    };

    struct fspath_opt : public string_opt
    {
        operator bool() const { return !empty(); }
    protected:
        void initArg(string const& _arg) override {
            string_opt::initArg(_arg);
            if (!boost::filesystem::exists(_arg))
            {
                std::cerr << "Could not locate file or path: '" + _arg + "'" << std::endl;
                exit(1);
            }
        }
    };

    struct vecstr_opt : public Option
    {
        vecstr_opt() { m_argType = ARGS::ONE;}
        operator std::vector<string>() const { return m_vector; }
    protected:
        std::vector<string> m_vector;
        void initArg(string const& _arg) override {
            std::vector<string> elements;
            boost::split(elements, _arg, boost::is_any_of(", "));
            for (auto& it : elements)
            {
                boost::algorithm::trim(it);
                if (!it.empty())
                    m_vector.push_back(it);
            }
        }
    };

    struct vecaddr_opt : public Option
    {
        vecaddr_opt() { m_argType = ARGS::ONE;}
        operator std::vector<IPADDRESS>() const { return m_vector; }
        size_t size() const { return m_vector.size(); }
    protected:
        std::vector<IPADDRESS> m_vector;
        void initArg(string const& _arg) override {
            for (auto const& el : explode(_arg, ','))
                m_vector.push_back(IPADDRESS(el));
        }
    };


public:
    // General Options
    void_opt help;
    void_opt version;
    void_opt listsuites;

    // Setting test suite
    string_opt rCurrentTestSuite;

    // Retesteth options
    sizet_opt threadCount = 1;
    vecstr_opt clients;
    fspath_opt datadir;
    vecaddr_opt nodesoverride;

    // Setting test suite and test
    fspath_opt testpath;
    fspath_opt singleTestFile;
    fspath_opt customTestFolder;


    //--------------------------------------------------------------
    bool enableClientsOutput = false;  ///< Enable stderr from clients

    bool vmtrace = false;              ///< Create EVM execution tracer
    bool vmtraceraw = false;           ///< Create EVM execution tracer. output raw info
    fs::path vmtracerawfolder;

    bool vmtrace_nomemory = false;
    bool vmtrace_nostack = false;
    bool vmtrace_noreturndata = false;

    bool filltests = false;            ///< Create JSON test files from execution results
    bool showhash = false;  ///< Show filler hash for debug information
    bool checkhash = false; ///< Check that tests are updated from fillers
    bool forceupdate = false; ///< Force tests update ragardless of new changes
    size_t blockLimit = 0;  ///< Perform blockchain blocks till this limit
    size_t rpcLimit = 0;    ///< Perform rpcRequests till this limit
    bool fillchain = false; ///< Fill tests as a blockchain tests if possible
    bool stats = false;     ///< Execution time and stats for state tests
    bool poststate = false;
    fs::path poststatefolder;
    bool nologcolor = false;
    std::string statsOutFile; ///< Stats output file. "out" for standard output


    bool exectimelog = false; ///< Print execution time for each test suite

    bool statediff = false;        ///< Fill full post state in General tests
    bool fullstate = false;        ///< Replace large state output to it's hash
    bool createRandomTest = false; ///< Generate random test
    bool travisOutThread = false;  ///< Output `.` to std:out when running tests
    boost::optional<uint64_t> randomTestSeed; ///< Define a seed for random test
	bool jsontrace = false; ///< Vmtrace to stdout in json format
	//eth::StandardTrace::DebugOptions jsontraceOptions; ///< output config for jsontrace

    unsigned logVerbosity = 1;
	boost::optional<boost::filesystem::path> randomCodeOptionsPath; ///< Options for random code generation in fuzz tests


    /// Test selection
	/// @{
	bool singleTest = false;


    boost::optional<std::string> singleTestOutFile; // --testfile run a single file filler output
    std::string singleTestName;     // A test name (usually a file.json test)
    std::string singleSubTestName;  // A test name inside a file.json (for blockchain tests)
    std::string singleTestNet;
    std::string trDataLabel;  ///< GeneralState data
    int trDataIndex = -1;     ///< GeneralState data
    int trGasIndex= -1;       ///< GeneralState gas
    int trValueIndex = -1;    ///< GeneralState value
    bool all = false;	///< Running every test, including time consuming ones.
    bool lowcpu = false; ///< Disable cpu-intense tests
	bool nonetwork = false;///< For libp2p
	/// @}
    static bool isLegacy();

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

public:
    /// Get reference to options
    /// The first time used, options are parsed with argc, argv
    static Options const& get(int argc = 0, const char** argv = 0);
    static DynamicOptions& getDynamicOptions() { return m_dynamicOptions; }
    static ClientConfig const& getCurrentConfig() { return m_dynamicOptions.getCurrentConfig(); }
    string getGStateTransactionFilter() const;

private:
    Options(int argc = 0, const char** argv = 0);
    Options(Options const&) = delete;

    static DynamicOptions m_dynamicOptions;
    friend class TestOptions;
    std::vector<Option*> m_options;
};

} //namespace test
