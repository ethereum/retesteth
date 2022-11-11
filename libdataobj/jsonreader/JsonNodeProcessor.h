#pragma once
#include "../DataObject.h"

namespace dataobject::jsonreader::processors
{
enum class NodeType
{
    ARRAY,
    OBJECT,
    STRING,
    INT,
    BOOL,
    NULLNODE
};

class JsonNodeProcessor
{
public:
    virtual void processChar(char const& _ch) = 0;
    virtual NodeType type() const = 0;
    virtual bool finalized() const = 0;
    virtual ~JsonNodeProcessor(){};

public:
    spDataObject m_res;
};

}  // namespace dataobject::jsonreader::processors
