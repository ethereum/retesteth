#include "JsonReader.h"
#include "ArrayProcessor.h"
using namespace std;
namespace dataobject::jsonreader::processors
{

void ArrayProcessor::readBegin(char const& _ch)
{
    if (_ch == ' ')
        return;
    if (m_reader == nullptr)
    {
        m_reader = JsonReader::detectJsonNode(_ch);
        if (m_reader == nullptr)
        {
            m_res = spDataObject(new DataObject(DataType::Array));
            m_state = &ArrayProcessor::preFinish;
            return;
        }
    }
    m_state = &ArrayProcessor::read;
}

void ArrayProcessor::read(char const& _ch)
{
    m_reader->processChar(_ch);
    if (m_reader->finalized())
    {
        m_state = &ArrayProcessor::seekContinue;
        (this->*m_state)(_ch);
    }
}

void ArrayProcessor::seekContinue(char const& _ch)
{
    if (_ch == ' ')
        return;
    if (_ch == ',')
    {
        m_res.getContent().addArrayObject(m_reader->m_res);
        delete m_reader;
        m_reader = nullptr;
        m_state = &ArrayProcessor::readBegin;
    }
    else
    {
        m_state = &ArrayProcessor::seekEnd;
        (this->*m_state)(_ch);
    }
}

void ArrayProcessor::seekEnd(char const& _ch)
{
    if (_ch == ' ')
        return;
    if (_ch == ']')
    {
        m_res.getContent().addArrayObject(m_reader->m_res);
        delete m_reader;
        m_reader = nullptr;
        m_state = &ArrayProcessor::preFinish;
    }
    else
        throw DataObjectException("JsonReader::ArrayProcessor::processChar: Unexpected End Of Array node, expected ']'!");
}

void ArrayProcessor::preFinish(char const& _ch)
{
    (void)_ch;
    m_finalized = true;
    m_state = &ArrayProcessor::finish;
}

void ArrayProcessor::finish(char const& _ch)
{
    throw DataObjectException(string() + "JsonReader::ArrayProcessor::processChar: reading char after finished parsing! `" + _ch + "`");
}


void ArrayProcessor::processChar(char const& _ch)
{
    (this->*m_state)(_ch);
}

}  // namespace dataobject::jsonreader::processors
