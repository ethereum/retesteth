#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"

namespace dataobject::jsonreader::processors
{
class ArrayProcessor : public JsonNodeProcessor
{
public:
    ArrayProcessor() : m_state(&ArrayProcessor::readBegin) {}
    NodeType type() const override { return NodeType::ARRAY; }
    virtual bool finalized() const override { return m_finalized; }
    void processChar(char const& _ch) override;
private:
    void readBegin(char const&);
    void read(char const&);
    void seekContinue(char const&);
    void seekEnd(char const&);
    void preFinish(char const&);
    void finish(char const&);

private:
    void (ArrayProcessor::*m_state)(char const&);
    JsonNodeProcessor* m_reader = nullptr;
    bool m_finalized = false;
};

}  // namespace dataobject::jsonreader::processors
