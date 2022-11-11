#include "ObjectProcessor.h"
#include "JsonReader.h"
#include "JsonReaderHelper.h"
#include "StringProcessor.h"
#include <iostream>
using namespace std;
using namespace dataobject::jsonreader;

namespace dataobject::jsonreader::processors
{
string ObjectProcessor::emptyString;
string const errPrefix = "JsonReader::ObjectProcessor::processChar:";

void ObjectProcessor::begin(char const& _ch)
{
    if (isEmptyChar(_ch))
        return;
    if (_ch != '{')
        throw DataObjectException(errPrefix + " expected `{` at the begining of json! got(`" + _ch + "`)");
    m_state = &ObjectProcessor::readBegin;
}

void ObjectProcessor::readBegin(char const& _ch)
{
    if (isEmptyChar(_ch))
        return;

    m_reader = JsonReader::detectJsonNode(_ch);
    if (m_reader.get() == nullptr)
    {
        if (_ch != '}')
            throw DataObjectException(errPrefix + " expected `}` at the end of object! got(`" + _ch + "`)");
        m_res = spDataObject(new DataObject(DataType::Object));
        m_state = &ObjectProcessor::prefinish;
        return;
    }
    else
    {
        m_reader->m_res.getContent().setAutosort(m_res->isAutosort());
        if (!m_stopper->empty() && m_reader->type() == NodeType::OBJECT)
            ((ObjectProcessor*)m_reader.get())->setStopper(*m_stopper);
    }

    m_state = &ObjectProcessor::readNode;
}

void ObjectProcessor::readNode(char const& _ch)
{
    m_reader->processChar(_ch);
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
        m_reader.reset();
        (this->*m_state)(_ch);
    }
}

void ObjectProcessor::seekValueOrContinue(char const& _ch)
{
    if (isEmptyChar(_ch))
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
    if (isEmptyChar(_ch))
        return;
    if (_ch == ':')
        throw DataObjectException(errPrefix + " trying to read `:` instead of `,`");
}

void ObjectProcessor::readValueBegin(char const& _ch)
{
    if (isEmptyChar(_ch))
        return;
    if (m_reader.get() == nullptr)
    {
        bool autosort = m_res.getContent().isAutosort();
        m_reader = JsonReader::detectJsonNode(_ch);
        m_reader->m_res.getContent().setAutosort(autosort);
        if (!m_stopper->empty() && m_reader->type() == NodeType::OBJECT)
            ((ObjectProcessor*)m_reader.get())->setStopper(*m_stopper);
    }
    m_state = &ObjectProcessor::readValue;
}

void ObjectProcessor::readValue(char const& _ch)
{
    m_reader->processChar(_ch);
    if (m_reader->finalized() || m_reader->aborted())
    {
        if (!m_key.empty())
        {
            if (m_res->count(m_key))
            {
                // make a unit test about it
                // remove or overwrite?
                // m_res.getContent().removeKey(m_key);
                m_reader->m_res.getContent().setKey(std::move(m_key));
                m_res.getContent().replaceObjectAtKey(m_key, m_reader->m_res);
            }
            else
            {
                m_reader->m_res.getContent().setKey(std::move(m_key));
                m_res.getContent().addSubObject(m_reader->m_res);
            }
        }
        else
        {
            m_res.getContent().addSubObject(m_reader->m_res);
        }


        if (m_reader->aborted())
        {
            m_state = &ObjectProcessor::aborted;
            (this->*m_state)(_ch);
        }

        m_reader.reset();

        if (!(*m_stopper).empty() && m_key == *m_stopper)
        {
            m_state = &ObjectProcessor::aborted;
            (this->*m_state)(_ch);
        }

        m_state = &ObjectProcessor::seekContinue;
        (this->*m_state)(_ch);
    }
    else if (m_reader->aborted())
    {
        m_state = &ObjectProcessor::aborted;
        (this->*m_state)(_ch);
    }
}

void ObjectProcessor::prefinish(char const& _ch)
{
    (void)_ch;
    m_finalized = true;
    m_state = &ObjectProcessor::finish;
}

void ObjectProcessor::aborted(char const& _ch)
{
    (void)_ch;
    m_aborted = true;
}

void ObjectProcessor::finish(char const& _ch)
{
    if (isEmptyChar(_ch))
        return;
    throw DataObjectException(errPrefix + " reading char after finished parsing! `" + _ch + "`");
}

void ObjectProcessor::processChar(char const& _ch)
{
    (this->*m_state)(_ch);
}

}  // namespace dataobject::jsonreader::processors
