#include "StringProcessor.h"
namespace dataobject::jsonreader::processors
{
void StringProcessor::processChar(char const& _ch)
{
    switch (m_state)
    {
    case STATE::READBEGIN:
    {
        if (_ch == '"')
            m_state = STATE::PREFINISH;
        else
            m_res.getContent().asStringUnsafe() += _ch;
        break;
    }
    case STATE::PREFINISH:
    {
        m_state = STATE::FINISH;
        break;
    }
    case STATE::FINISH:
    {
        throw DataObjectException("JsonReader::KeyProcessor::processChar: reading char after finished parsing!");
        break;
    }
    }
}
}  // namespace dataobject::jsonreader::processors
