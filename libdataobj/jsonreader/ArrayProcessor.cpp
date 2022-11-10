#include "JsonReader.h"
#include "ArrayProcessor.h"
namespace dataobject::jsonreader::processors
{
void ArrayProcessor::processChar(char const& _ch)
{
    switch (m_state)
    {
        case STATE::BEGIN:
        {
            if (_ch == ' ')
                break;
            if (_ch != '[')
                throw DataObjectException("JsonReader::ArrayProcessor::processChar: expected '[' in declaration of the array! ");
            m_state = STATE::READBEGIN;
            break;
        }
        case STATE::READBEGIN:
        {
            if (_ch == ' ')
                break;
            if (m_reader == nullptr)
                m_reader = JsonReader::detectJsonNode(_ch);
            m_state = STATE::READ;
            break;
        }
        case STATE::READ:
        {
            if (m_reader->finalized())
                m_state = STATE::SEEKFORCONTINUE;
            else
                m_reader->processChar(_ch);
            break;
        }
        case STATE::SEEKFORCONTINUE:
        {
            if (_ch == ' ')
                break;
            if (_ch == ',')
            {
                m_res.getContent().addArrayObject(m_reader->m_res);
                delete m_reader;
                m_reader = nullptr;
                m_state = STATE::READBEGIN;
            }
            else
                m_state = STATE::SEEKFOREND;
            break;
        }
        case STATE::SEEKFOREND:
        {
            if (_ch == ' ')
                break;
            if (_ch == ']')
                m_state = STATE::FINISH;
            else
                throw DataObjectException("JsonReader::ArrayProcessor::processChar: Unexpected End Of Array node, expected ']'!");
            break;
        }
        case STATE::FINISH:
        {
            throw DataObjectException("JsonReader::ArrayProcessor::processChar: reading char after finished parsing!");
            break;
        }

    }
}
}  // namespace dataobject::jsonreader::processors
