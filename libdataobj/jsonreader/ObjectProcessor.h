#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"

namespace dataobject::jsonreader::processors
{
class ObjectProcessor : public JsonNodeProcessor
{
public:
    ObjectProcessor(bool) : m_state(&ObjectProcessor::begin), m_stopper(&emptyString) {}
    ObjectProcessor() : m_state(&ObjectProcessor::readBegin), m_stopper(&emptyString) {}
    NodeType type() const override { return NodeType::OBJECT; }
    virtual bool finalized() const override { return m_finalized; }
    virtual bool aborted() const override { return m_aborted; }
    void processChar(char const& _ch) override;
    void setStopper(std::string const& _s) { m_stopper = &_s; }

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
    void aborted(char const&);

private:
    void (ObjectProcessor::*m_state)(char const&);
    JsonNodeProcessor* m_reader = nullptr;
    bool m_aborted = false;
    bool m_finalized = false;
    std::string m_key;
    static std::string emptyString;
    std::string const* m_stopper;
};

}  // namespace dataobject::jsonreader::processors
