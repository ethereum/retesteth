#pragma once
#include "VMLogRecord.h"
#include <libdataobj/DataObject.h>
#include <boost/filesystem/path.hpp>

namespace test::teststruct
{

class DebugVMTrace : public GCP_SPointerBase
{
private:
    class DebugVMTraceImplInterface
    {
    public:
        virtual void print() const = 0;
        std::vector<VMLogRecord> const& getLog() const { return m_log; };
        virtual ~DebugVMTraceImplInterface(){}
    protected:
        std::string m_infoString;
        std::vector<VMLogRecord> m_log;
    };
    class DebugVMTraceRaw : public DebugVMTraceImplInterface
    {
    public:
        DebugVMTraceRaw(std::string const& _info, boost::filesystem::path const&);
        void print() const override;
    private:
        std::string m_rawUnparsedLogs;
    };
    class DebugVMTraceNice : public DebugVMTraceImplInterface
    {
    public:
        DebugVMTraceNice(std::string const& _info, boost::filesystem::path const&);
        void print() const override;
    private:
        void readLog(std::string const&);
        bool m_limitReached = false;
    };


public:
    DebugVMTrace() {}  // for tuples
    DebugVMTrace(std::string const& _info, boost::filesystem::path const& _logs);
    void print() const;
    void exportLogs(boost::filesystem::path const& _folder) const;
    std::vector<VMLogRecord> const& getLog();
    ~DebugVMTrace();

private:
    std::shared_ptr<DebugVMTraceImplInterface> m_impl;
    boost::filesystem::path m_rawVmTraceFile;
};

typedef GCP_SPointer<DebugVMTrace> spDebugVMTrace;

}  // namespace teststruct
