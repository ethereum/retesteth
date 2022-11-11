#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"
#include <memory>

namespace dataobject::jsonreader::processors
{
class NullProcessor : public JsonNodeProcessor
{
public:
    NullProcessor(const char& _ch) : m_state(&NullProcessor::begin) { processChar(_ch); }
    NodeType type() const override { return NodeType::NULLNODE; }
    virtual bool finalized() const override { return m_finalized; }
    void processChar(char const& _ch) override;

private:
    void begin(const char& _ch);
    void readbegin(const char& _ch);
    void finish(const char& _ch);

private:
    std::string m_valueread;
    void (NullProcessor::*m_state)(char const&);
    bool m_finalized = false;
    std::unique_ptr<JsonNodeProcessor> m_reader;
};

}  // namespace dataobject::jsonreader::processors
