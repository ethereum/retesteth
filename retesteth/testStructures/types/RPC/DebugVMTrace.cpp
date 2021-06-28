#include "DebugVMTrace.h"
#include <EthChecks.h>
#include <TestHelper.h>
#include <dataObject/ConvertFile.h>
#include <testStructures/Common.h>

namespace test
{
namespace teststruct
{
VMLogRecord::VMLogRecord(DataObject const& _obj)
{
    try
    {
        pc = _obj.atKey("pc").asInt();
        op = _obj.atKey("op").asInt();
        gas = spVALUE(new VALUE(_obj.atKey("gas")));
        gasCost = spVALUE(new VALUE(_obj.atKey("gasCost")));
        memory = spBYTES(new BYTES(_obj.atKey("memory")));
        memSize = _obj.atKey("memSize").asInt();
        for (auto const& el : _obj.atKey("stack").getSubObjects())
            stack.push_back(el.asString());
        for (auto const& el : _obj.atKey("returnStack").getSubObjects())
            returnStack.push_back(el.asString());
        returnData = spBYTES(new BYTES(DataObject("0x")));
        depth = _obj.atKey("depth").asInt();
        refund = _obj.atKey("refund").asInt();
        opName = _obj.atKey("opName").asString();
        error = _obj.atKey("error").asString();
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("VMLogRecord parse error: ") + _ex.what());
    }
}

DebugVMTrace::DebugVMTrace(string const& _info, string const& _trNumber, FH32 const& _trHash, string const& _logs)
{
    try
    {
        m_infoString = _info;
        m_trNumber = _trNumber;
        m_trHash = spFH32(new FH32(_trHash));

        auto logs = test::explode(_logs, '\n');
        if (logs.size())
        {
            for (size_t i = 0; i < logs.size() - 1; i++)
                m_log.push_back(VMLogRecord(ConvertJsoncppStringToData(logs.at(i))));

            DataObject lastRecord(ConvertJsoncppStringToData(logs.at(logs.size() - 1)));
            m_output = lastRecord.atKey("output").asString();
            m_gasUsed = spVALUE(new VALUE(lastRecord.atKey("gasUsed")));
            m_time = lastRecord.atKey("time").asInt();
        }

        m_rawUnparsedLogs = _logs;
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("DebugVMTrace parse error: ") + _ex.what());
    }
}

void DebugVMTrace::print()
{
    ETH_LOG(m_infoString, 0);
    ETH_LOG(m_rawUnparsedLogs, 0);
}

void DebugVMTrace::printNice()
{
    ETH_LOG(m_infoString, 0);
    if (m_log.size() == 0)
        return;

    string s_comment = "";
    dev::bigint maxGas = m_log.at(0).gas->asBigInt();
    size_t k = 0;
    size_t const step = 9;
    string const stepw = "          ";
    std::cout << test::cBYellowBlack << "N" << setw(15) << "OPNAME" << setw(10) << "GASCOST" << setw(10) << "TOTALGAS"
              << setw(10) << "REMAINGAS" << setw(20) << "ERROR" << test::cDefault << std::endl;
    for (VMLogRecord const& el : m_log)
    {
        if (!s_comment.empty())
        {
            std::cout << setw(step * el.depth) << test::cYellow << s_comment << test::cDefault << std::endl;
            s_comment = string();
        }
        std::cout << setw(step * (el.depth - 1));
        std::cout << test::fto_string(k++) + "-" + test::fto_string(el.depth)
                  << setw(15) << el.opName
                  << setw(10) << el.gasCost->asDecString()
                  << setw(10) << maxGas - el.gas->asBigInt()
                  << setw(10) << el.gas->asDecString()
                  << setw(20) << el.error << std::endl;

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
    std::cout << std::endl;
}

}  // namespace teststruct
}  // namespace test
