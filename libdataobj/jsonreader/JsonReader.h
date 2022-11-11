#pragma once
#include "../DataObject.h"
#include "ObjectProcessor.h"
#include "ArrayProcessor.h"
namespace dataobject::jsonreader
{
class JsonReader
{
protected:
    static processors::JsonNodeProcessor* detectJsonNode(const char& _ch);
    friend class processors::ObjectProcessor;
    friend class processors::ArrayProcessor;

public:
    JsonReader(std::string const& _stopper, bool _autosort)
    {
        m_processor->m_res.getContent().setAutosort(_autosort);
        ((processors::ObjectProcessor*)m_processor)->setStopper(_stopper);
    }
    void processLine(std::string const& _line);
    bool finalized() const { return m_processor->finalized(); }
    bool aborted() const { return m_processor->aborted(); }
    spDataObject getResult() { return m_processor->m_res; }

private:
    JsonReader() = delete;
    size_t m_processedLineNumber = 0;
    bool m_seenBegining = false;
    processors::JsonNodeProcessor* m_processor = new processors::ObjectProcessor(true);
};

}  // namespace dataobject::jsonreader
