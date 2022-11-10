#include "KeyProcessor.h"
namespace dataobject::jsonreader::processors
{
void KeyProcessor::processChar(char const& _ch)
{
    switch (m_state)
    {
    case STATE::BEGIN:
    {
        if (_ch == ' ')
            break;
        if (_ch != '"')
            throw DataObjectException("JsonReader::KeyProcessor::processChar: expected '\"' in declaration of the key! ");
        m_state = STATE::READKEY;
        break;
    }
    case STATE::READKEY:
    {
        if (_ch == '"')
            m_state = STATE::FINISH;
        else
            m_key += _ch;
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
