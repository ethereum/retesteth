#include "JsonReader.h"
#include "../DataObject.h"
#include "../Exception.h"
#include "BoolProcessor.h"
#include "IntegerProcessor.h"
#include "JsonNodeProcessor.h"
#include "NullProcessor.h"
#include "StringProcessor.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>

using namespace std;
using namespace dataobject::jsonreader;
using namespace dataobject::jsonreader::processors;

namespace dataobject::jsonreader
{

std::unique_ptr<JsonNodeProcessor> JsonReader::detectJsonNode(const char& _ch)
{
    if (_ch == '"')
        return std::unique_ptr<JsonNodeProcessor>(new StringProcessor());
    if (_ch == '{')
        return std::unique_ptr<JsonNodeProcessor>(new ObjectProcessor());
    if (_ch == '[')
        return std::unique_ptr<JsonNodeProcessor>(new ArrayProcessor());
    const bool minus = _ch == '-';
    if (std::isdigit(_ch) || minus)
        return std::unique_ptr<JsonNodeProcessor>(new IntegerProcessor(_ch, minus));
    if (_ch == '}' || _ch == ']')
        return std::unique_ptr<JsonNodeProcessor>(nullptr);
    if (_ch == 't' || _ch == 'f')
        return  std::unique_ptr<JsonNodeProcessor>(new BoolProcessor(_ch));
    if (_ch == 'n')
        return std::unique_ptr<JsonNodeProcessor>(new NullProcessor(_ch));
    throw DataObjectException(string("Undetermend processor `") + _ch + "`");
    return nullptr;
}


void JsonReader::processLine(string const& _line)
{
    m_processedLineNumber++;
    // Reading root json object from file. The root json must include one object.
    if (_line.empty())
        return;
    try
    {
        for (auto const& ch : _line)
        {
            m_processor->processChar(ch);
            if (m_processor->aborted())
                break;
        }
    }
    catch (DataObjectException const& _ex)
    {
        throw DataObjectException(string(_ex.what()) + "\n Line " + std::to_string(m_processedLineNumber) + ": " + _line);
    }
}
}  // namespace dataobject::jsonreader

namespace dataobject
{
spDataObject ConvertJsoncppStringToData2(string const& _input, string const& _stopper, bool _autosort)
{
    JsonReader reader(_stopper, _autosort);
    reader.processLine(_input);
    reader.processLine(" ");
    if (!reader.finalized() && !reader.aborted())
        throw DataObjectException("ConvertJsoncppFileToData: Json structure incomplete in string!");
    return reader.getResult();
}

spDataObject ConvertJsoncppFileToData(string const& _file, string const& _stopper, bool _autosort)
{
    std::ifstream file(_file);
    if (file.is_open())
    {
        string line;
        JsonReader reader(_stopper, _autosort);
        while (std::getline(file, line))
        {
            reader.processLine(line);
            if (reader.aborted())
                break;
        }
        reader.processLine(" ");
        file.close();

        if (!reader.finalized())
        {
            //std::cerr << reader.getResult()->asJson() << std::endl;
            throw DataObjectException("ConvertJsoncppFileToData: Json structure incomplete in file: " + _file);
        }
        //else
        //    std::cerr << reader.getResult()->asJson() << std::endl;


        return reader.getResult();
    }
    else
        throw DataObjectException() << "ConvertJsoncppFileToData: can't open file: `" + _file;
}

}  // namespace dataobject
