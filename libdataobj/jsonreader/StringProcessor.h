#pragma once
#include "../DataObject.h"
#include "JsonNodeProcessor.h"

namespace dataobject::jsonreader::processors
{
class StringProcessor : public JsonNodeProcessor
{
public:
    StringProcessor() { m_res = spDataObject(new DataObject(DataType::String)); }
    NodeType type() const override { return NodeType::STRING; }
    virtual bool finalized() const override { return m_state == STATE::FINISH; }
    void processChar(char const& _ch) override;

private:
    enum class STATE
    {
        READBEGIN,
        PREFINISH,
        FINISH
    };
    STATE m_state = STATE::READBEGIN;
    JsonNodeProcessor* m_reader = nullptr;
};

}  // namespace dataobject::jsonreader::processors
