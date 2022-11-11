#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"
#include <memory>

namespace dataobject::jsonreader::processors
{
class IntegerProcessor : public JsonNodeProcessor
{
public:
    IntegerProcessor(const char& _ch, bool _minus) : m_state(&IntegerProcessor::begin), m_minus(_minus) { processChar(_ch); }
    NodeType type() const override { return NodeType::INT; }
    virtual bool finalized() const override { return m_finalized; }
    void processChar(char const& _ch) override;

private:
    void begin(const char& _ch);
    void readBegin(const char& _ch);
    void finish(const char& _ch);

private:
    std::string m_valueread;
    void (IntegerProcessor::*m_state)(char const&);
    bool m_minus = false;
    bool m_finalized = false;
    std::unique_ptr<JsonNodeProcessor> m_reader;
};

}  // namespace dataobject::jsonreader::processors
