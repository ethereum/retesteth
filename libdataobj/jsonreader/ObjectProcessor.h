#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"

namespace dataobject::jsonreader::processors
{
class ObjectProcessor : public JsonNodeProcessor
{
public:
    ObjectProcessor(bool) : m_state(&ObjectProcessor::begin) {}
    ObjectProcessor() : m_state(&ObjectProcessor::readBegin) {}
    NodeType type() const override { return NodeType::OBJECT; }
    virtual bool finalized() const override { return m_finalized; }
    void processChar(char const& _ch) override;

private:
    void begin(char const&);
    void readBegin(char const&);
    void readNode(char const&);
    void seekValueOrContinue(char const&);
    void seekContinue(char const&);
    void readValueBegin(char const&);
    void readValue(char const&);
    void prefinish(char const&);
    void finish(char const&);

private:
    void (ObjectProcessor::*m_state)(char const&);
    JsonNodeProcessor* m_reader = nullptr;
    bool m_finalized = false;
    std::string m_key;
};

}  // namespace dataobject::jsonreader::processors
