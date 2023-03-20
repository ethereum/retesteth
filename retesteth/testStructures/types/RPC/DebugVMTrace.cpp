#include "DebugVMTrace.h"
#include <EthChecks.h>
#include <TestHelper.h>
#include <libdataobj/ConvertFile.h>
#include <testStructures/Common.h>
#include <boost/filesystem/fstream.hpp>
#include <Options.h>

using namespace std;
using namespace test::debug;
namespace fs = boost::filesystem;

const size_t c_maxRowsToPrint = 300;
const string c_tooManyRawsMessage = "==TOO MANY LOG ROWS TO PRINT (Use --vmtraceraw <folder>)==";

namespace test::teststruct
{

DebugVMTrace::DebugVMTraceRaw::DebugVMTraceRaw(string const& _info, fs::path const& _logs)
{
    m_infoString = _info;
    string line;
    size_t k = 0;
    if (!fs::exists(_logs))
        return;

    try
    {
        fs::ifstream fileHandler(_logs);
        while (getline(fileHandler, line))
        {
            if (++k < c_maxRowsToPrint)
                m_rawUnparsedLogs += line + "\n";
            else if (k == c_maxRowsToPrint)
            {
                m_rawUnparsedLogs += c_tooManyRawsMessage;
                break;
            }
        }
        if (m_rawUnparsedLogs.empty())
            ETH_WARNING("Reading empty vmtrace logs: " + _logs.string());
        fileHandler.close();
    }
    catch (const ifstream::failure& e) {
        throw UpwardsException("Error reading trace file: " + _logs.string());
    }
}

void DebugVMTrace::DebugVMTraceRaw::print()
{
    ETH_DC_MESSAGE(DC::DEFAULT, m_infoString);
    ETH_DC_MESSAGE(DC::DEFAULT, m_rawUnparsedLogs);
}

DebugVMTrace::DebugVMTraceNice::DebugVMTraceNice(string const& _info, fs::path const& _logs)
{
    m_infoString = _info;
    string line;
    size_t k = 0;
    if (!fs::exists(_logs))
        return;

    try
    {
        fs::ifstream fileHandler(_logs);
        if (Options::get().fillvmtrace)
        {
            while (getline(fileHandler, line))
                readLog(line);
        }
        else
        {
            while (getline(fileHandler, line))
            {
                if (++k < c_maxRowsToPrint)
                    readLog(line);
                else if (k == c_maxRowsToPrint)
                {
                    m_limitReached = true;
                    break;
                }
            }
        }
        if (m_log.size() == 0)
            ETH_WARNING("Reading empty vmtrace logs: " + _logs.string());
        fileHandler.close();
    }
    catch (const ifstream::failure& e) {
        throw UpwardsException("Error reading trace file: " + _logs.string());
    }
}

void DebugVMTrace::DebugVMTraceNice::readLog(string const& _line)
{
    auto const data = ConvertJsoncppStringToData(_line);
    m_log.emplace_back(VMLogRecord(data));
}

DebugVMTrace::DebugVMTrace(string const& _info, fs::path const& _logs)
{
    try
    {
        if (!fs::exists(_logs))
            throw EthError("Log file not found: `" + _logs.string());

        if (Options::get().vmtraceraw)
            m_impl.reset(new DebugVMTraceRaw(_info, _logs));
        else
            m_impl.reset(new DebugVMTraceNice(_info, _logs));

        // Take a handle of t8ntool file in our own tmp path
        auto const uniqueFolder = fs::unique_path();
        fs::create_directory(_logs.parent_path().parent_path() / uniqueFolder);
        m_rawVmTraceFile = _logs.parent_path().parent_path() / uniqueFolder / _logs.stem();
        fs::rename(_logs, m_rawVmTraceFile);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("DebugVMTrace parse error: ") + _ex.what());
    }
}


void DebugVMTrace::DebugVMTraceNice::print()
{
    ETH_DC_MESSAGE(DC::DEFAULT, m_infoString);
    if (m_log.size() == 0)
        return;

    string s_comment = "";
    dev::bigint maxGas = m_log.at(0).isShort ? 500000 : m_log.at(0).gas->asBigInt();
    size_t k = 0;
    size_t const step = 9;
    string const stepw = "          ";

    std::cout << cBYellowBlack << "N" << setw(15) << "OPNAME" << setw(10) << "GASCOST" << setw(10) << "TOTALGAS" << setw(10)
              << "REMAINGAS" << setw(10) << "STACK" << cDefault << std::endl;
    for (VMLogRecord const& el : m_log)
    {
        // Last record with error info
        if (el.isShort)
            continue;

        if (!s_comment.empty())
        {
            std::cout << setw(step * el.depth) << cYellow << s_comment << cDefault << std::endl;
            s_comment = string();
        }
        std::cout << setw(step * (el.depth - 1));
        std::cout << test::fto_string(k++) + "-" + test::fto_string(el.depth)
                  << setw(15) << el.opName
                  << setw(10) << el.gasCost->asDecString()
                  << setw(10) << maxGas - el.gas->asBigInt()
                  << setw(10) << el.gas->asDecString();

        const size_t c_stackPreviewSize = 10;
        if (el.stack.size())
            std::cout << setw(5) <<  el.stack.size() << ":[";
        for (auto const& stackEl : el.stack)
        {
            if (stackEl.size() > c_stackPreviewSize)
                std::cout << stackEl.substr(2, c_stackPreviewSize) << "...,";
            else
                std::cout << stackEl.substr(2) << ",";
        }

        if (el.stack.size())
            std::cout << "]";
        std::cout << std::endl;
        if (el.error.size())
            std::cout << "Detected error: " << el.error << std::endl;

        // Opcode highlight
        static vector<string> callopcodes = { "CALLCODE", "CALL", "DELEGATECALL" };
        if ( inArray(callopcodes, el.opName) && el.stack.size() > 1)
            s_comment = "SUBCALL: " + el.stack.at(el.stack.size() - 2);
        if (el.opName == "SSTORE" && el.stack.size() > 1)
            s_comment = stepw + "SSTORE [" + el.stack.at(el.stack.size() - 1) + "] = " + el.stack.at(el.stack.size() - 2);
        if (el.opName == "MSTORE" && el.stack.size() > 1)
            s_comment = stepw + "MSTORE [" + el.stack.at(el.stack.size() - 1) + "] = " + el.stack.at(el.stack.size() - 2);
        if (el.opName == "RETURN")
            s_comment = stepw + "RETURN " + el.memory->asString();
    }
    if (m_limitReached)
        std::cout << c_tooManyRawsMessage << std::endl;
    std::cout << std::endl;
}

DebugVMTrace::~DebugVMTrace()
{
    if (!m_rawVmTraceFile.empty())
    {
        boost::filesystem::remove(m_rawVmTraceFile);
        boost::filesystem::remove(m_rawVmTraceFile.parent_path());
    }
}

void DebugVMTrace::exportLogs(fs::path const& _folder)
{
    try
    {
        if (!fs::exists(_folder.parent_path()))
            fs::create_directories(_folder.parent_path());
        fs::rename(m_rawVmTraceFile, _folder);
    }
    catch (std::exception const& _ex)
    {
        try
        {
            fs::copy(m_rawVmTraceFile, _folder);
            fs::remove(m_rawVmTraceFile);
            fs::remove(m_rawVmTraceFile.parent_path());
        }
        catch (std::exception const& _ex)
        {
            throw UpwardsException(string("DebugVMTrace::exportLogs error: ") + _ex.what());
        }
    }
}

std::vector<VMLogRecord> const& DebugVMTrace::getLog()
{
    return m_impl->getLog();
}

void DebugVMTrace::print()
{
    if (m_impl)
        m_impl->print();
}

}  // namespace teststruct
