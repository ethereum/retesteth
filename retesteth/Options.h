#pragma once

#include <libdevcore/Exceptions.h>
#include <retesteth/TestHelper.h>
#include <retesteth/configs/ClientConfig.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <list>

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
        NONE_OPTIONAL,
        ONE,
        ONEMERGED
    };

    class Option
    {
    public:
        bool initialized() const { return m_inited; }
        void setValidator(std::function<void()> _func) { m_validatorFunc = _func; }
        void setDefHelp(string&& _def, std::function<void()> _help);
        void setBeforeSeparator() { m_allowBeforeSeparator = true; }
        void setOverrideOption() { m_optionOverrides = true; }
        void tryInit(list<const char*>& _argList);
        void printHelp();
        void validate() const;
    private:
        int initArgs(list<const char*> const& _argList, list<const char*>::const_iterator _arg);
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
        std::function<void()> m_validatorFunc;
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
        sizet_opt& operator=(size_t _var) { m_arg = _var; return *this;}
    protected:
        size_t m_arg;
        void initArg(string const& _arg) override {
            m_arg = max(0, atoi(_arg.c_str()));
        }
    };

    struct int_opt : public Option
    {
        int_opt(int _arg) : m_arg(_arg) { m_argType = ARGS::ONE; }
        operator int() const { return m_arg; }
    protected:
        int m_arg;
        void initArg(string const& _arg) override {
            m_arg = atoi(_arg.c_str());
        }
    };

    struct bool_opt : public Option
    {
        bool_opt(bool _arg) { m_argType = ARGS::NONE; m_inited = _arg; }
        operator bool() const { return m_inited; }
        bool_opt& operator=(bool _arg) { m_inited = _arg; return *this; }
    protected:
        void initArg(string const& _arg) override { (void) _arg; }
    };

    struct booloutpath_opt : public bool_opt
    {
        booloutpath_opt(bool _arg) : bool_opt(_arg) { m_argType = ARGS::NONE_OPTIONAL; }
        operator bool() const { return m_inited; }
        string outpath;
    protected:
        void initArg(string const& _arg) override {
                outpath = _arg;
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

    struct stringosizet_opt : public Option
    {
        stringosizet_opt() { m_argType = ARGS::ONE;}
        stringosizet_opt(size_t _i) : val(_i) { m_argType = ARGS::ONE;}
        bool operator == (size_t _i) const { return _i == val; }
        string str;
        size_t val;
    protected:
        void initArg(string const& _arg) override {
            DigitsType type = test::stringIntegerType(_arg);
            if (type == DigitsType::String)
                str = _arg;
            else if (type == DigitsType::Decimal)
                val = max(0, atoi(_arg.c_str()));
            else
                BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` wrong option argument format: " + _arg));
        }
    };

    struct fspath_opt : public string_opt
    {
    protected:
        void initArg(string const& _arg) override {
            string_opt::initArg(_arg);
            if (!boost::filesystem::exists(_arg))
                BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` could not locate file or path: " + _arg));
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

    struct singletest_opt : public Option
    {
        singletest_opt() { m_argType = ARGS::ONE;}
        string name;
        string subname;
    protected:
        void initArg(string const& _arg) override {
            name = _arg;

            size_t pos = name.find("Filler");
            if (pos != string::npos)
            {
                name = name.substr(0, pos);
                std::cout << "WARNING: Correcting filter to: `" + name + "`" << std::endl;
            }
            pos = name.find_last_of('/');
            if (pos != string::npos)
            {
                subname = name.substr(pos + 1);
                name = name.substr(0, pos);
            }
        }
    };

    struct dataind_opt : public Option
    {
        dataind_opt() { m_argType = ARGS::ONE;}
        int index = -1;
        string label;
    protected:
        void initArg(string const& _arg) override {
            DigitsType type = stringIntegerType(_arg);
            switch (type)
            {
            case DigitsType::Decimal:
                index = atoi(_arg.c_str());
                break;
            case DigitsType::String:
                label = _arg;
                if (_arg.find(":label") == string::npos)
                    label = ":label " + label;
                break;
            default:
                BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` option has wrong argument format: " + _arg));
            }
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
    string_opt datadir;
    vecaddr_opt nodesoverride;

    // Setting test suite and test
    fspath_opt testpath;
    fspath_opt singleTestFile;
    fspath_opt customTestFolder;
    string_opt singleTestOutFile;
    singletest_opt singletest;
    string_opt singleTestNet;

    // Debugging
    dataind_opt trData;
    int_opt trGasIndex= -1;
    int_opt trValueIndex = -1;
    bool_opt vmtrace = false;
    booloutpath_opt vmtraceraw = false;
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

    // Additional Tests
    bool_opt all = false;
    bool_opt lowcpu = false;

    // Test Generation
    bool_opt filltests = false;
    bool_opt fillchain = false;
    bool_opt showhash = false;
    bool_opt checkhash = false;
    booloutpath_opt poststate = false;
    bool_opt fullstate = false;
    bool_opt forceupdate = false;
    static bool isLegacy();

public:
    struct InvalidOption : public Exception
    {
        InvalidOption(std::string&& _message = std::string()) : Exception(std::move(_message)) {}
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
