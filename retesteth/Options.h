#pragma once

#include <libdevcore/Exceptions.h>
#include <retesteth/configs/ClientConfig.h>
#include <list>

namespace test
{
class TestOptions;
class Options
{
private:
    enum class ARGS
    {
        NONE,
        NONE_OPTIONAL,
        NONE_OPTIONAL2,
        ONE,
        ONEMERGED
    };

    class Option
    {
    public:
        bool initialized() const { return m_inited; }
        void setValidator(std::function<void()> _func) { m_validatorFunc = _func; }
        void setDefHelp(std::string&& _def, std::function<void()> _help);
        void setBeforeSeparator() { m_allowBeforeSeparator = true; }
        void setOverrideOption() { m_optionOverrides = true; }
        void tryInit(std::list<const char*>& _argList);
        void overrideInitArg(std::string const& _arg) { m_inited = true; initArg(_arg); }
        void deInitialize() { m_inited = false; }
        void printHelp();
        void validate() const;
    private:
        int initArgs(std::list<const char*> const& _argList, std::list<const char*>::const_iterator _arg);
        bool isAfterSeparatorOption() const;
        bool match(std::string const& _arg) const;

    protected:
        virtual void initArg(std::string const& _arg) = 0;
        virtual void initArg2(std::string const&){};
        Option(){};
        std::string m_sOptionHelp;
        std::string m_sOptionName;

        ARGS m_argType;
        bool m_allowBeforeSeparator = false;
        bool m_optionOverrides = false;
        bool m_inited = false;
        std::function<void()> m_printHelpFunc;
        std::function<void()> m_validatorFunc;
    };

    struct void_opt : public Option
    {
        void_opt() { m_argType = ARGS::NONE; }
    protected:
        void initArg(std::string const& _arg) override { (void)_arg; }
    };

    struct sizet_opt : public Option
    {
        sizet_opt(int _arg) { m_argType = ARGS::ONEMERGED; m_arg = (size_t)_arg; }
        operator size_t() const { return m_arg; }
        sizet_opt& operator=(size_t _var) { m_arg = _var; return *this;}
    protected:
        size_t m_arg;
        void initArg(std::string const& _arg) override { m_arg = std::max(0, atoi(_arg.c_str())); }
    };

    struct int_opt : public Option
    {
        int_opt(int _arg) : m_arg(_arg) { m_argType = ARGS::ONE; }
        operator int() const { return m_arg; }
    protected:
        int m_arg;
        void initArg(std::string const& _arg) override { m_arg = atoi(_arg.c_str()); }
    };

    struct bool_opt : public Option
    {
        bool_opt(bool _arg) { m_argType = ARGS::NONE; m_inited = _arg; }
        operator bool() const { return m_inited; }
        bool_opt& operator=(bool _arg) { m_inited = _arg; return *this; }
    protected:
        void initArg(std::string const& _arg) override { (void)_arg; }
    };

    struct booloutpath_opt : public bool_opt
    {
        booloutpath_opt(bool _arg) : bool_opt(_arg) { m_argType = ARGS::NONE_OPTIONAL; }
        operator bool() const { return m_inited; }
        std::string outpath;

    protected:
        void initArg(std::string const& _arg) override { outpath = _arg; }
    };

    struct statediff_opt : public bool_opt
    {
        statediff_opt(bool _arg) : bool_opt(_arg) { m_argType = ARGS::NONE_OPTIONAL; }
        operator bool() const { return m_inited; }
        bool isForkSelected = false;
        std::string firstFork;
        std::string seconFork;

        bool isBlockSelected = false;
        bool isTransSelected = false;
        size_t firstBlock;
        size_t firstTrnsx;
        size_t seconBlock;
        size_t seconTrnsx;

    protected:
        void initArg(std::string const& _arg) override;
    };

    struct booloutpathselector_opt : public booloutpath_opt
    {
        booloutpathselector_opt(bool _arg) : booloutpath_opt(_arg) { m_argType = ARGS::NONE_OPTIONAL2; }
        operator bool() const { return m_inited; }
        size_t blockNumber;
        size_t transactionNumber;
        bool isBlockSelected = false;

    protected:
        void initArg(std::string const& _arg) override { parse2OptionalArgs(_arg); }
        void initArg2(std::string const& _arg) override { parse2OptionalArgs(_arg); }
        void parse2OptionalArgs(std::string const& _arg);
    };

    struct string_opt : public Option, std::string
    {
        string_opt() { m_argType = ARGS::ONE;}
    protected:
        void initArg(std::string const& _arg) override { assign(_arg); }
    };

    struct stringosizet_opt : public Option
    {
        stringosizet_opt() { m_argType = ARGS::ONE;}
        stringosizet_opt(size_t _i) : val(_i) { m_argType = ARGS::ONE;}
        bool operator == (size_t _i) const { return _i == val; }
        std::string str;
        size_t val;
    protected:
        void initArg(std::string const& _arg) override;
    };

    struct fspath_opt : public string_opt
    {
    protected:
        void initArg(std::string const& _arg) override;
    };

    struct vecstr_opt : public Option
    {
        vecstr_opt() { m_argType = ARGS::ONE;}
        operator std::vector<std::string>() const { return m_vector; }

    protected:
        std::vector<std::string> m_vector;
        void initArg(std::string const& _arg) override;
    };

    struct vecaddr_opt : public Option
    {
        vecaddr_opt() { m_argType = ARGS::ONE;}
        operator std::vector<IPADDRESS>() const { return m_vector; }
        size_t size() const { return m_vector.size(); }
    protected:
        std::vector<IPADDRESS> m_vector;
        void initArg(std::string const& _arg) override;
    };

    struct singletest_opt : public Option
    {
        singletest_opt() { m_argType = ARGS::ONE;}
        std::string name;
        std::string subname;

    protected:
        void initArg(std::string const& _arg) override;
    };

    struct dataind_opt : public Option
    {
        dataind_opt() { m_argType = ARGS::ONE;}
        int index = -1;
        std::string label;

    protected:
        void initArg(std::string const& _arg) override;
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
    string_opt datadir;
    vecaddr_opt nodesoverride;

    // Setting test suite and test
    fspath_opt testpath;
    fspath_opt singleTestFile;
    string_opt singleTestOutFile;
    singletest_opt singletest;
    string_opt singleTestNet;
    string_opt runOnlyNets;

    // Debugging
    dataind_opt trData;
    int_opt trGasIndex= -1;
    int_opt trValueIndex = -1;
    bool_opt getvectors = false;
    booloutpathselector_opt vmtrace = false;
    statediff_opt statediff = false;
    booloutpathselector_opt vmtraceraw = false;
    bool_opt vmtrace_nomemory = false;
    bool_opt vmtrace_nostack = false;
    bool_opt vmtrace_noreturndata = false;
    sizet_opt blockLimit = 0;
    sizet_opt rpcLimit = 0;
    stringosizet_opt logVerbosity = 1;
    bool_opt nologcolor = false;
    bool_opt exectimelog = false;
    bool_opt enableClientsOutput = false;
    bool_opt travisOutThread = false;
    string_opt t8ntoolcall;

    // Additional Tests
    bool_opt all = false;
    bool_opt lowcpu = false;

    // Test Generation
    bool_opt filltests = false;
    bool_opt filloutdated = false;
    bool_opt fillvmtrace = false;
    bool_opt fillchain = false;
    sizet_opt chainid = 1;
    bool_opt showhash = false;
    bool_opt checkhash = false;
    booloutpathselector_opt poststate = false;
    bool_opt fullstate = false;
    bool_opt forceupdate = false;
    static bool isLegacy();

public:
    struct InvalidOption : public dev::Exception
    {
        InvalidOption(std::string&& _message = std::string()) : dev::Exception(std::move(_message)) {}
    };

    struct DynamicOptions
    {
        DynamicOptions() {}
        std::vector<ClientConfig> const& getClientConfigs() const;
        ClientConfig const& getCurrentConfig() const;
        void setCurrentConfig(ClientConfig const& _config);
        void setTestsuiteRunning(bool _arg);
        bool testSuiteRunning() const;
        size_t activeConfigs() const;
        bool currentConfigIsSet() const;
        std::set<FORK> const& runOnlyNetworks() const { return m_runOnlyNetworks; }

    private:
        bool m_testSuiteRunning = false;
        std::set<FORK> m_runOnlyNetworks;
        mutable std::vector<ClientConfig> m_clientConfigs;
        test::ClientConfigID m_currentConfigID = test::ClientConfigID::null();
    };

public:
    /// Get reference to options
    /// The first time used, options are parsed with argc, argv
    static Options const& get(int argc = 0, const char** argv = 0);
    static DynamicOptions& getDynamicOptions() { return m_dynamicOptions; }
    static ClientConfig const& getCurrentConfig() { return m_dynamicOptions.getCurrentConfig(); }
    std::string getGStateTransactionFilter() const;

private:
    Options(int argc = 0, const char** argv = 0);
    Options(Options const&) = delete;

    static DynamicOptions m_dynamicOptions;
    friend class TestOptions;
    std::vector<Option*> m_options;
};

class TestOptions
{
public:
    TestOptions(int argc = 0, const char** argv = 0) : m_opt(argc, argv) {}
    Options const& get() { return m_opt; }
    void overrideMainOptions() const;
    ~TestOptions();

    static bool isOverride();
    static Options const& getOverride();
private:
    Options m_opt;
    static bool m_isOverride;
    static Options const* m_global_test_opt;
};

} //namespace test
