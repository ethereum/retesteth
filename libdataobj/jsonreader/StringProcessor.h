#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"
#include <memory>

namespace dataobject::jsonreader::processors
{
class StringProcessor : public JsonNodeProcessor
{
public:
    StringProcessor() : m_state(&StringProcessor::readBegin)
    {
        m_res = spDataObject(new DataObject(DataType::String));
        m_prevChar = &beginChar;
    }
    NodeType type() const override { return NodeType::STRING; }
    virtual bool finalized() const override { return m_finalized; }
    void processChar(char const& _ch) override;

private:
    void readBegin(char const&);
    void preFinish(char const&);
    void finish(char const&);

private:
    void (StringProcessor::*m_state)(char const&);
    std::unique_ptr<JsonNodeProcessor> m_reader;
    static char beginChar;
    bool m_finalized = false;
    char const* m_prevChar;
};

}  // namespace dataobject::jsonreader::processors
