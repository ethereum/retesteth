#include "ObjectProcessor.h"
#include "JsonReader.h"
#include "KeyProcessor.h"
#include <iostream>
using namespace std;

namespace dataobject::jsonreader::processors
{
void ObjectProcessor::processChar(char const& _ch)
{
    string key;
    auto seekforcontinue = [this](const char& _ch) {
        std::cerr << "scont ch `" << _ch << "` " << int(_ch) << std::endl;
        if (_ch == ',')
        {
            m_state = STATE::READBEGIN;
            return;
        }
        if (_ch == '}')
        {
            m_state = STATE::FINISH;
            return;
        }
        if (_ch == ' ')
            return;
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
            throw DataObjectException(string() + "JsonReader::ObjectProcessor::processChar: expected '{' in declaration of the object! Got: `" + _ch);
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
                m_state = STATE::SEEKFORVALUE;
            }
            else
            {
                m_res.getContent().addSubObject(m_reader->m_res);
                delete m_reader;
                m_reader = nullptr;
                m_state = STATE::SEEKFORCONTINUE;
            }
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
        seekforcontinue(_ch);
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
        m_reader->processChar(_ch);
        if (m_reader->finalized())
        {
            if (!key.empty())
                m_reader->m_res.getContent().setKey(key);
            if (m_reader->type() == NodeType::KEY)
            {
                spDataObject value(new DataObject(std::move(((KeyProcessor*)m_reader)->key())));
                m_res.getContent().addSubObject(value);
            }
            else
                m_res.getContent().addSubObject(m_reader->m_res);
            delete m_reader;
            m_reader = nullptr;

            m_state = STATE::SEEKFORCONTINUE;
            if (_ch == '"')
                break;
            seekforcontinue(_ch);
            break;
        }
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
            throw DataObjectException(string() + "JsonReader::ObjectProcessor::processChar: Unexpected End Of Object node, expected '}', but got: `" + _ch + "`");
        break;
    }
    case STATE::FINISH:
    {
        throw DataObjectException(string() + "JsonReader::ObjectProcessor::processChar: reading char after finished parsing! `" + _ch + "`");
        break;
    }
    }
}
}  // namespace dataobject::jsonreader::processors
