#include "StringProcessor.h"
namespace dataobject::jsonreader::processors
{
char StringProcessor::beginChar = '"';

void StringProcessor::readBegin(char const& _ch)
{
    if (_ch == '"' && *m_prevChar != '\\')
        m_state = &StringProcessor::preFinish;
    else
    {
        m_prevChar = &_ch;
        m_res.getContent().asStringUnsafe() += _ch;
    }

}
void StringProcessor::preFinish(char const&)
{
    m_finalized = true;
    m_state = &StringProcessor::finish;
}
void StringProcessor::finish(char const&)
{
    throw DataObjectException("JsonReader::KeyProcessor::processChar: reading char after finished parsing!");
}

void StringProcessor::processChar(char const& _ch)
{
    (this->*m_state)(_ch);
}
}  // namespace dataobject::jsonreader::processors
