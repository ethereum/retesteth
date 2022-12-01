#pragma once
#include "DataObject.h"

namespace dataobject
{
namespace jsonreader
{
class JsonReader
{
public:
    JsonReader(std::string const& _stopper, bool _autosort) : m_stopper(_stopper)
    {
        m_res.getContent().setAutosort(_autosort);
        m_actualRoot = &m_res.getContent();
    }
    void processLine(std::string const& _line);
    spDataObject getResult() { return m_res; }

private:
    JsonReader() = delete;
    std::string const& m_stopper;
    spDataObject m_res;
    DataObject* m_actualRoot;
    bool m_seenBegining = false;
};
}  // namespace jsonreader

/// Convert Json object represented as string to DataObject
spDataObject ConvertJsoncppStringToData(
    std::string const& _input, std::string const& _stopper = std::string(), bool _setAutosort = false);

spDataObject ConvertJsoncppFileToData(
    std::string const& _file, std::string const& _stopper = std::string(), bool _setAutosort = false);
}
