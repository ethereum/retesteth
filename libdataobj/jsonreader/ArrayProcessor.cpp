#include "ArrayProcessor.h"
#include "JsonReader.h"
#include "JsonReaderHelper.h"
using namespace std;
using namespace dataobject::jsonreader;
namespace dataobject::jsonreader::processors
{

void ArrayProcessor::readBegin(char const& _ch)
{
    if (isEmptyChar(_ch))
        return;
    if (m_reader.get() == nullptr)
    {
        m_reader = JsonReader::detectJsonNode(_ch);
        if (m_reader.get() == nullptr)
        {
            m_res = spDataObject(new DataObject(DataType::Array));
            m_state = &ArrayProcessor::preFinish;
            return;
        }
        else
        {
            m_reader->m_res.getContent().setAutosort(m_res->isAutosort());
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
    if (isEmptyChar(_ch))
        return;
    if (_ch == ',')
    {
        m_res.getContent().addArrayObject(m_reader->m_res);
        m_reader.reset();
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
    if (isEmptyChar(_ch))
        return;
    if (_ch == ']')
    {
        m_res.getContent().addArrayObject(m_reader->m_res);
        m_reader.reset();
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
    if (isEmptyChar(_ch))
        return;
    throw DataObjectException(string() + "JsonReader::ArrayProcessor::processChar: reading char after finished parsing! `" + _ch + "`");
}


void ArrayProcessor::processChar(char const& _ch)
{
    (this->*m_state)(_ch);
}

}  // namespace dataobject::jsonreader::processors
