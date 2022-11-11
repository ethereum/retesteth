#include "IntegerProcessor.h"
#include <iostream>
using namespace std;
namespace dataobject::jsonreader::processors
{
void IntegerProcessor::begin(const char& _ch)
{
    if (!m_minus)
        m_valueread += _ch;
    m_state = &IntegerProcessor::readBegin;
}
void IntegerProcessor::readBegin(const char& _ch)
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
        m_state = &IntegerProcessor::finish;
        m_finalized = true;
    }
    else
        m_valueread += _ch;
}
void IntegerProcessor::finish(const char& _ch)
{
    (void) _ch;
    throw DataObjectException("JsonReader::IntegerProcessor::processChar: reading char after finished parsing!");
}

void IntegerProcessor::processChar(char const& _ch)
{
    (this->*m_state)(_ch);
}
}  // namespace dataobject::jsonreader::processors
