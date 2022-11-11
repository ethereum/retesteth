#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"

namespace dataobject::jsonreader::processors
{
class IntegerProcessor : public JsonNodeProcessor
{
public:
    IntegerProcessor(const char& _ch, bool _minus) : m_state(STATE::BEGIN), m_minus(_minus) { processChar(_ch); }
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
    bool m_minus = false;
    JsonNodeProcessor* m_reader = nullptr;
};

}  // namespace dataobject::jsonreader::processors
