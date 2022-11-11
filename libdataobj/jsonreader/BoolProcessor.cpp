#include "BoolProcessor.h"
#include <iostream>
using namespace std;
namespace dataobject::jsonreader::processors
{
void BoolProcessor::processChar(char const& _ch)
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
        if (_ch != 'r' && _ch != 'u' && _ch != 'e'
            && _ch != 'a' && _ch != 'l' && _ch != 's')
        {
            if (m_valueread == "true" || m_valueread == "false")
            {
                m_res.getContent().setBool(m_valueread == "true");
                m_state = STATE::FINISH;
                break;
            }
            else
                throw DataObjectException(string() + "JsonReader::BoolProcessor::processChar: reading wrong bool! `" + m_valueread + "`");
        }
        m_valueread += _ch;
        break;
    }
    case STATE::FINISH:
    {
        throw DataObjectException("JsonReader::BoolProcessor::processChar: reading char after finished parsing!");
        break;
    }
    }
}
}  // namespace dataobject::jsonreader::processors
