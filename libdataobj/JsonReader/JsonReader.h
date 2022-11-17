#pragma once
#include "../DataObject.h"
#include "JsonInput.h"

namespace dataobject::jsonreader
{

class JsonReader
{
public:
    JsonReader(JsonInput* _jinput, std::string const& _stopper, bool _autosort)
      :  m_input(_jinput), m_stopper(_stopper), m_autosort(_autosort)
    {
        root.getContent().setAutosort(_autosort);
        actualRoot = &root.getContent();
    }

    void parse();
    spDataObject getResult() { return root; }
    ~JsonReader() { delete m_input; }

private:
    JsonReader() = delete;

private:
    JsonInput* m_input;

    // indexes at root array of objects that we are reading into
    std::vector<DataObject*> applyDepth;
    spDataObject root;
    DataObject* actualRoot;
    bool keyEncountered = false;
    std::string const& m_stopper;
    bool m_autosort = false;
};

}  // namespace jsonreader
