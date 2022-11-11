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
    JsonReader(std::string const& _stopper, bool _autosort) : m_stopper(_stopper)
    {
        m_res.getContent().setAutosort(_autosort);
        m_actualRoot = &m_res.getContent();
    }
    void processLine(std::string const& _line);
    bool finalized() const { return m_processor->finalized(); }
    spDataObject getResult() { return m_res; }


private:
    JsonReader() = delete;
    size_t m_processedLineNumber = 0;
    std::string const& m_stopper;
    spDataObject m_res = spDataObject(new DataObject(DataType::Object));
    DataObject* m_actualRoot;
    bool m_seenBegining = false;
    processors::JsonNodeProcessor* m_processor = new processors::ObjectProcessor();
};

}  // namespace dataobject::jsonreader
