#include "NullProcessor.h"
#include <iostream>
using namespace std;
namespace dataobject::jsonreader::processors
{
void NullProcessor::begin(const char& _ch)
{
    m_valueread += _ch;
    m_state = &NullProcessor::readbegin;
}
void NullProcessor::readbegin(const char& _ch)
{
    if (_ch != 'u' && _ch != 'l')
    {
        if (m_valueread == "null")
        {
            m_res = spDataObject(new DataObject(DataType::Null));
            m_state = &NullProcessor::finish;
            m_finalized = true;
            return;
        }
        else
            throw DataObjectException(
                string() + "JsonReader::NullProcessor::processChar: reading wrong bool! `" + m_valueread + "`");
    }
    m_valueread += _ch;
}
void NullProcessor::finish(const char& _ch)
{
    (void)_ch;
    throw DataObjectException("JsonReader::NullProcessor::processChar: reading char after finished parsing!");
}

void NullProcessor::processChar(char const& _ch)
{
    (this->*m_state)(_ch);
}
}  // namespace dataobject::jsonreader::processors
