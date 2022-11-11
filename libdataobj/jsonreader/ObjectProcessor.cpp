#include "ObjectProcessor.h"
#include "JsonReader.h"
#include "StringProcessor.h"
#include <iostream>
using namespace std;

namespace dataobject::jsonreader::processors
{

void ObjectProcessor::readBegin(char const& _ch)
{
    if (_ch == ' ')
        return;
    if (m_reader == nullptr)
        m_reader = JsonReader::detectJsonNode(_ch);
    m_state = &ObjectProcessor::readNode;
}

void ObjectProcessor::readNode(char const& _ch)
{
    if (m_reader->finalized())
    {
        if (m_reader->type() == NodeType::STRING)
        {
            m_key = std::move(m_reader->m_res.getContent().asStringUnsafe());
            m_state = &ObjectProcessor::seekValueOrContinue;
        }
        else
        {
            m_res.getContent().addSubObject(m_reader->m_res);
            m_state = &ObjectProcessor::seekContinue;
        }
        delete m_reader;
        m_reader = nullptr;
        (this->*m_state)(_ch);
    }
    else
        m_reader->processChar(_ch);
}

void ObjectProcessor::seekValueOrContinue(char const& _ch)
{
    if (_ch == ' ')
        return;
    if (_ch == ':')
    {
        m_state = &ObjectProcessor::readValueBegin;
        return;
    }
    m_state = &ObjectProcessor::seekContinue;
    (this->*m_state)(_ch);
}

void ObjectProcessor::seekContinue(char const& _ch)
{
    std::cerr << "scont ch `" << _ch << "` " << int(_ch) << std::endl;
    if (_ch == ',')
    {
        m_state = &ObjectProcessor::readBegin;
        return;
    }
    if (_ch == '}')
    {
        m_state = &ObjectProcessor::prefinish;
        return;
    }
    if (_ch == ' ')
        return;
}

void ObjectProcessor::readValueBegin(char const& _ch)
{
    if (_ch == ' ')
        return;
    if (m_reader == nullptr)
        m_reader = JsonReader::detectJsonNode(_ch);
    m_state = &ObjectProcessor::readValue;
}

void ObjectProcessor::readValue(char const& _ch)
{
    m_reader->processChar(_ch);
    if (m_reader->finalized())
    {
        if (!m_key.empty())
            m_reader->m_res.getContent().setKey(std::move(m_key));
        m_res.getContent().addSubObject(m_reader->m_res);
        delete m_reader;
        m_reader = nullptr;

        m_state = &ObjectProcessor::seekContinue;
        (this->*m_state)(_ch);
    }
}

void ObjectProcessor::prefinish(char const& _ch)
{
    (void)_ch;
    m_finalized = true;
    m_state = &ObjectProcessor::finish;
}

void ObjectProcessor::finish(char const& _ch)
{
    throw DataObjectException(string() + "JsonReader::ObjectProcessor::processChar: reading char after finished parsing! `" + _ch + "`");
}

void ObjectProcessor::processChar(char const& _ch)
{
    (this->*m_state)(_ch);
}
}  // namespace dataobject::jsonreader::processors
