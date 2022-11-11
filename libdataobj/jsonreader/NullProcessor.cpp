#include "NullProcessor.h"
#include <iostream>
using namespace std;
namespace dataobject::jsonreader::processors
{
void NullProcessor::processChar(char const& _ch)
{
    switch (m_state)
    {
    case STATE::BEGIN:
    {
        m_valueread += _ch;
        m_state = STATE::READBEGIN;
        break;
    }
    case STATE::READBEGIN:
    {
        if (_ch != 'u' && _ch != 'l')
        {
            if (m_valueread == "null")
            {
                m_res = spDataObject(new DataObject(DataType::Null));
                m_state = STATE::FINISH;
                break;
            }
            else
                throw DataObjectException(
                    string() + "JsonReader::NullProcessor::processChar: reading wrong bool! `" + m_valueread + "`");
        }
        m_valueread += _ch;
        break;
    }
    case STATE::FINISH:
    {
        throw DataObjectException("JsonReader::NullProcessor::processChar: reading char after finished parsing!");
        break;
    }
    }
}
}  // namespace dataobject::jsonreader::processors
