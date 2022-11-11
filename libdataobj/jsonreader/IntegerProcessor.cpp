#include "IntegerProcessor.h"
#include <iostream>
using namespace std;
namespace dataobject::jsonreader::processors
{
void IntegerProcessor::processChar(char const& _ch)
{
    switch (m_state)
    {
    case STATE::BEGIN:
    {
        if (!m_minus)
            m_valueread += _ch;
        m_state = STATE::READBEGIN;
        break;
    }
    case STATE::READBEGIN:
    {
        if (!std::isdigit(_ch))
        {
            if (m_valueread.size() == 0)
                throw DataObjectException(string() + "JsonReader::IntegerProcessor::processChar: reading empty int!");

            const int v = std::atoi(m_valueread.c_str());
            if (m_minus)
                m_res.getContent().setInt(v * -1);
            else
                m_res.getContent().setInt(v);
            m_state = STATE::FINISH;
        }
        m_valueread += _ch;
        break;
    }
    case STATE::FINISH:
    {
        throw DataObjectException("JsonReader::IntegerProcessor::processChar: reading char after finished parsing!");
        break;
    }
    }
}
}  // namespace dataobject::jsonreader::processors
