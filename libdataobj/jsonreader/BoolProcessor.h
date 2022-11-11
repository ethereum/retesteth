#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"
#include <memory>

namespace dataobject::jsonreader::processors
{
class BoolProcessor : public JsonNodeProcessor
{
public:
    BoolProcessor(const char& _ch) : m_state(STATE::BEGIN) { processChar(_ch); }
    NodeType type() const override { return NodeType::INT; }
    virtual bool finalized() const override { return m_state == STATE::FINISH; }
    void processChar(char const& _ch) override;

private:
    enum class STATE
    {
        BEGIN,
        READBEGIN,
        FINISH
    };
    std::string m_valueread;
    STATE m_state = STATE::READBEGIN;
    std::unique_ptr<JsonNodeProcessor> m_reader;
};

}  // namespace dataobject::jsonreader::processors
