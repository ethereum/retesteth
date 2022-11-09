#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"

namespace dataobject::jsonreader::processors
{
class ArrayProcessor : public JsonNodeProcessor
{
public:
    ArrayProcessor(bool _begin) : m_state(STATE::READBEGIN)
    {
        if (!_begin)
            m_state = STATE::BEGIN;
    }
    NodeType type() const override { return NodeType::OBJECT; }
    virtual bool finalized() const override { return m_state == STATE::FINISH; }
    void processChar(char const& _ch) override { (void)_ch; };

private:
    enum class STATE
    {
        BEGIN,
        READBEGIN,
        FINISH
    };
    STATE m_state = STATE::BEGIN;
    JsonNodeProcessor* m_reader = nullptr;
};

}  // namespace dataobject::jsonreader::processors
