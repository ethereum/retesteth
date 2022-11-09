#include "ObjectProcessor.h"
#include "JsonReader.h"
#include "KeyProcessor.h"
using namespace std;

namespace dataobject::jsonreader::processors
{
void ObjectProcessor::processChar(char const& _ch)
{
    string key;
    auto seekforcontinue = [this, &key](const char& _ch) {
        if (_ch == ',')
        {
            m_reader->m_res.getContent().setKey(key);
            m_res.getContent().addSubObject(m_reader->m_res);
            delete m_reader;
            m_reader = nullptr;
            m_state = STATE::READBEGIN;
        }
        else
            m_state = STATE::SEEKFOREND;
    };
    switch (m_state)
    {
    case STATE::BEGIN:
    {
        if (_ch == ' ')
            break;
        if (_ch != '{')
            throw DataObjectException("JsonReader::ObjectProcessor::processChar: expected '{' in declaration of the object! ");
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
        {
            if (m_reader->type() == NodeType::KEY)
            {
                key = std::move(((KeyProcessor*)m_reader)->key());
                delete m_reader;
                m_reader = nullptr;
            }
            m_state = STATE::SEEKFORVALUE;
        }
        else
            m_reader->processChar(_ch);
        break;
    }
    case STATE::SEEKFORVALUE:
    {
        if (_ch == ' ')
            break;
        if (_ch == ':')
        {
            m_state = STATE::READVALUEBEGIN;
            break;
        }
        break;
    }
    case STATE::READVALUEBEGIN:
    {
        if (_ch == ' ')
            break;
        if (m_reader == nullptr)
            m_reader = JsonReader::detectJsonNode(_ch);
        m_state = STATE::READVALUE;
        break;
    }
    case STATE::READVALUE:
    {
        if (m_reader->finalized())
        {
            m_state = STATE::SEEKFORCONTINUE;
            seekforcontinue(_ch);
        }
        else
            m_reader->processChar(_ch);
        break;
    }
    case STATE::SEEKFORCONTINUE:
    {
        if (_ch == ' ')
            break;
        seekforcontinue(_ch);
        break;
    }
    case STATE::SEEKFOREND:
    {
        if (_ch == ' ')
            break;
        if (_ch == '}')
            m_state = STATE::FINISH;
        else
            throw DataObjectException("JsonReader::ObjectProcessor::processChar: Unexpected End Of Object node, expected '}'!");
        break;
    }
    case STATE::FINISH:
    {
        throw DataObjectException("JsonReader::ObjectProcessor::processChar: reading char after finished parsing!");
        break;
    }
    }
}
}  // namespace dataobject::jsonreader::processors
