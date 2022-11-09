#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"

namespace dataobject::jsonreader::processors
{
class KeyProcessor : public JsonNodeProcessor
{
public:
    KeyProcessor(bool _begin) : m_state(STATE::READKEY) { (void)_begin; }
    NodeType type() const override { return NodeType::KEY; }
    virtual bool finalized() const override { return m_state == STATE::FINISH; }
    void processChar(char const& _ch) override;
    std::string& key() { return m_key; }

private:
    enum class STATE
    {
        BEGIN,
        READKEY,
        FINISH
    };
    STATE m_state = STATE::BEGIN;
    std::string m_key;
    JsonNodeProcessor* m_reader = nullptr;
};

}  // namespace dataobject::jsonreader::processors
