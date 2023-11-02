#include "Options.h"
#include <retesteth/helpers/TestHelper.h>
#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

using namespace test;
using namespace std;


string Options::getGStateTransactionFilter() const
{
    string filter;
    filter += trData.index == -1 ? string() : " dInd: " + to_string(trData.index);
    filter += trData.label.empty() ? string() : " dLbl: " + trData.label;
    filter += trGasIndex == -1 ? string() : " gInd: " + to_string(trGasIndex);
    filter += trValueIndex == -1 ? string() : " vInd: " + to_string(trValueIndex);
    return filter;
}

bool Options::isLegacy()
{
    static bool isLegacy = (boost::unit_test::framework::current_test_case().full_name().find("LegacyTests") != string::npos);

    // Current test case is dynamic if we run all tests. need to see if we hit LegacyTests
    if (Options::get().rCurrentTestSuite.empty())
        isLegacy = (boost::unit_test::framework::current_test_case().full_name().find("LegacyTests") != string::npos);

    return isLegacy;
}

bool Options::Option::match(string const& _arg) const
{
    if (m_argType == ARGS::ONEMERGED)
        return m_sOptionName == _arg.substr(0, m_sOptionName.size());
    else
    {
        for (auto const& el : explode(m_sOptionName, '|'))
            if (el == _arg)
                return true;
    }
    return false;
}
bool Options::Option::isAfterSeparatorOption() const
{
    return !m_allowBeforeSeparator;
}

void Options::Option::validate() const
{
    if (initialized() && m_validatorFunc)
        m_validatorFunc();
}

void Options::Option::setDefHelp(string&& _def, std::function<void()> _help)
{
    m_sOptionName = std::move(_def);
    m_printHelpFunc = _help;
}

void Options::Option::printHelp()
{
    if (m_printHelpFunc)
        m_printHelpFunc();
}

void Options::Option::tryInit(list<const char*>& _argList)
{
    bool seenSeparator = false;
    for (list<const char*>::iterator it = _argList.begin(); it != _argList.end(); it++)
    {
        auto arg = std::string{*it};
        if (arg == "--")
            seenSeparator = true;

        if (match(arg))
        {
            if (m_optionOverrides)
            {
                m_inited = true;
                break;
            }

            if (isAfterSeparatorOption() && !seenSeparator)
                BOOST_THROW_EXCEPTION(
                    InvalidOption("Error: `" + arg + "` option appears before the separator `--`"));
            if (!isAfterSeparatorOption() && seenSeparator)
                BOOST_THROW_EXCEPTION(
                    InvalidOption("Error: `" + arg + "` option appears after the separator `--`"));

            int readArgsCount = initArgs(_argList, it);
            list<const char*>::iterator last = it;
            for (int i = 0; i < readArgsCount && last != _argList.end(); i++)
                last++;
            _argList.erase(it, last);
            break;
        }
    }
}

int Options::Option::initArgs(list<const char*> const& _argList, list<const char*>::const_iterator _arg)
{
    m_inited = m_inited || false;
    auto throwIfNoArgumentFollows = [&_argList, this](list<const char*>::const_iterator _arg) {
        auto throwException = [this](){
            BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` option is missing an argument."));
        };
        if (++_arg == _argList.end())
            throwException();
        string nextArg{*_arg};
        if (nextArg.substr(0, 1) == "-")
            throwException();
    };

    switch(m_argType)
    {
    case ARGS::NONE:
    {
        m_inited = true;
        return 1;
    }
    case ARGS::NONE_OPTIONAL:
    {
        m_inited = true;
        if (++_arg != _argList.end())
        {
            auto const arg = string{(*_arg)};
            if (arg.substr(0, 1) != "-")
            {
                initArg(arg);
                return 2;
            }
        }
        return 1;
        break;
    }
    case ARGS::NONE_OPTIONAL2:
    {
        m_inited = true;
        if (++_arg != _argList.end())
        {
            auto const arg = string{(*_arg)};
            if (arg.substr(0, 1) != "-")
            {
                initArg(arg);
                if (++_arg != _argList.end())
                {
                    auto const arg2 = string{(*_arg)};
                    if (arg2.substr(0, 1) != "-")
                    {
                        initArg2(arg2);
                        return 3;
                    }
                }
                return 2;
            }
        }
        return 1;
        break;
    }
    case ARGS::ONEMERGED:
    {
        size_t const optNameLength = m_sOptionName.length();
        string const inputOption = string(*_arg);
        if (inputOption.length() != optNameLength)
        {
            string mergedArg = inputOption.substr(optNameLength, inputOption.length());
            initArg(mergedArg);
            m_inited = true;
            return 1;
        }
        else
        {
            throwIfNoArgumentFollows(_arg);
            initArg(string{*(++_arg)});
            m_inited = true;
            return 2;
        }
        return 1;
    }
    case ARGS::ONE:
    {
        throwIfNoArgumentFollows(_arg);
        initArg(string{*(++_arg)});
        m_inited = true;
        return 2;
    }
    default:
        m_inited = false;
    }
    return 0;
}

void Options::stringosizet_opt::initArg(std::string const& _arg)
{
    DigitsType type = test::stringIntegerType(_arg);
    if (type == DigitsType::String)
        str = _arg;
    else if (type == DigitsType::Decimal)
        val = std::max(0, atoi(_arg.c_str()));
    else
        BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` wrong option argument format: " + _arg));
}

void Options::vecstr_opt::initArg(std::string const& _arg)
{
    std::vector<std::string> elements;
    boost::split(elements, _arg, boost::is_any_of(", "));
    for (auto& it : elements)
    {
        boost::algorithm::trim(it);
        if (!it.empty())
            m_vector.push_back(it);
    }
}

void Options::vecaddr_opt::initArg(std::string const& _arg)
{
    for (auto const& el : explode(_arg, ','))
        m_vector.push_back(IPADDRESS(DataObject(el)));
}

void Options::singletest_opt::initArg(std::string const& _arg)
{
    name = _arg;

    size_t pos = name.find("Filler");
    if (pos != std::string::npos)
    {
        name = name.substr(0, pos);
        std::cout << "WARNING: Correcting filter to: `" + name + "`" << std::endl;
    }
    pos = name.find_last_of('/');
    if (pos != std::string::npos)
    {
        subname = name.substr(pos + 1);
        name = name.substr(0, pos);
    }
}

void Options::dataind_opt::initArg(std::string const& _arg)
{
    DigitsType type = test::stringIntegerType(_arg);
    switch (type)
    {
    case DigitsType::Decimal:
        index = atoi(_arg.c_str());
        break;
    case DigitsType::String:
        label = _arg;
        if (_arg.find(":label") == std::string::npos)
            label = ":label " + label;
        break;
    default:
        BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` option has wrong argument format: " + _arg));
    }
}

void Options::fspath_opt::initArg(std::string const& _arg)
{
    string_opt::initArg(_arg);
    if (!boost::filesystem::exists(_arg))
        BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` could not locate file or path: " + _arg));
}

void Options::statediff_opt::initArg(std::string const& _arg)
{
    string const forkDel = "--";
    size_t const pos0 = _arg.find(forkDel);
    if (pos0 != string::npos)
    {
        // Parse --statediff Fork--Fork
        isForkSelected = true;
        firstFork = _arg.substr(0, pos0);
        seconFork = _arg.substr(pos0 + forkDel.size());
        return;
    }

    // Parse --statediff xtoy or --statediff x:ytox2:y2 (Block:Tx selection)
    string const del = "to";
    size_t const pos1 = _arg.find(":");
    if (pos1 != string::npos)
    {
        size_t const pos2 = _arg.find(del);
        size_t const pos3 = _arg.find(":", pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos)
        {
            isBlockSelected = true;
            isTransSelected = true;
            string const block1 = _arg.substr(0, pos1);
            string const trans1 = _arg.substr(pos1 + 1, pos2 - pos1 - 1);
            string const block2 = _arg.substr(pos2 + del.size(), pos3 - pos2 - del.size());
            string const trans2 = _arg.substr(pos3 + 1);
            firstBlock = atoi(block1.c_str());
            firstTrnsx = atoi(trans1.c_str());
            seconBlock = atoi(block2.c_str());
            seconTrnsx = atoi(trans2.c_str());
        }
        else
            BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` option arg format is x:ytox2:y2"));
    }
    else
    {
        size_t const pos2 = _arg.find(del);
        if (pos2 != string::npos)
        {
            isBlockSelected = true;
            firstTrnsx = 0;
            seconTrnsx = 0;
            isTransSelected = false;
            string const block1 = _arg.substr(0, pos2);
            string const block2 = _arg.substr(pos2 + del.size(), _arg.size() - pos2 - del.size());
            firstBlock = atoi(block1.c_str());
            seconBlock = atoi(block2.c_str());
        }
        else
            BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` option arg format is xtoy"));
    }

    if (firstBlock > seconBlock)
        BOOST_THROW_EXCEPTION(InvalidOption("Error: `" + m_sOptionName + "` option arg format is `xtoy` or `x:ytox2:y2`, where `y >= x` or `x2 >= x`"));
}

void Options::booloutpathselector_opt::parse2OptionalArgs(std::string const& _arg)
{
    // Can take 0 args, act as bool
    // 1 arg = either path or block selector
    // 2 arg = path and block selector in any order
    size_t pos = _arg.find(":");
    if (pos != string::npos && !isBlockSelected)
    {
        blockNumber = atoi(_arg.substr(0, pos).c_str());
        transactionNumber = atoi(_arg.substr(pos + 1).c_str());
        isBlockSelected = true;
    }
    else
        outpath = _arg;
}
