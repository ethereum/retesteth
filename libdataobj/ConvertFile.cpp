#include "ConvertFile.h"
#include "DataObject.h"
#include "Exception.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;
using namespace dataobject::jsonreader;
// Manually construct dataobject from file string content
// bacuse Json::Reader::parse has a memory leak

namespace dataobject
{
std::string const errorPrefix = "Error parsing json: ";
bool isEmptyChar(char const& _char)
{
    if (_char == ' ' || _char == '\n' || _char == '\r' || _char == '\t')
        return true;
    return false;
}

size_t stripSpaces(std::string const& _input, size_t _i)
{
    size_t i = _i;
    for (; i < _input.length(); i++)
    {
        if (isEmptyChar(_input[i]))
            continue;
        else
            return i;
    }
    return i;
}

string parseKeyValue(string const& _input, size_t& _i)
{
    if (_i + 1 > _input.size())
        throw DataObjectException() << errorPrefix + "reached EOF before reading char: `\"`";

    bool escapeChar = true;
    size_t endPos = _input.find('"', _i + 1);
    while (escapeChar)
    {
        escapeChar = (_input[endPos - 1] == '\\');
        if (escapeChar)
            endPos = _input.find('"', endPos + 1);
    }

    if (endPos != string::npos)
    {
        const string key = _input.substr(_i + 1, endPos - _i - 1);
        _i = endPos + 1;
        return key;
    }
    else
        throw DataObjectException() << errorPrefix + "not found key ending char: `\"`";
    return string();
}

bool readBoolOrNull(string const& _input, size_t& _i, bool& _result, bool& _readNull)
{
    if (_i + 4 >= _input.size())
        return false;

    // true false
    const string text = _input.substr(_i, 4);
    if (text == "null")
    {
        _i += 4;
        _readNull = true;
        return false;
    }
    if (text == "true")
    {
        _result = true;
        _i += 4;
        return true;
    }
    else if (text == "fals")
    {
        if (_input.substr(_i, 5) == "false")
        {
            _i += 5;
            _result = false;
            return true;
        }
    }
    return false;
}

bool readDigit(string const& _input, size_t& _i, int& _result)
{
    bool readMinus = false;
    auto const& e = _input[_i];
    if (e == '-')
    {
        readMinus = true;
        _i++;
    }

    bool digit = true;
    string readNumber;
    while (digit)
    {
        auto const& e = _input[_i];
        if (e == '0' || e == '1' || e == '2' || e == '3' || e == '4' || e == '5' || e == '6' ||
            e == '7' || e == '8' || e == '9')
        {
            readNumber += e;
            _i++;
        }
        else
        {
            digit = false;
            _i = stripSpaces(_input, _i);
        }
    }
    if (readNumber.size())
    {
        _result = std::atoi(readNumber.c_str());
        if (readMinus)
            _result *= -1;
        return true;
    }
    return false;
}

bool checkExcessiveComa(string const& _input, size_t _i)
{
    if (_i < 1)
        return false;
    size_t reader = _i - 1;
    while (isEmptyChar(_input[reader]) && reader != 0)
        reader--;
    if (_input[reader] == ',')
        return true;
    return false;
}

void JsonReader::processLine(string const& _line)
{
    if (_line.empty())
        return;
    if (!m_seenBegining)
    {
        // if (_line.find("{") == string::npos)
        //    throw DataObjectException() << "ConvertJsoncppStringToData can't read json structure in file: `" +
        //    _input.substr(0, 50);
    }
    std::cerr << _line << std::endl;
    (void)m_stopper;
    // for (size_t i = 0; i < _line.length(); i++)
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
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            reader.processLine(line);
        }
        file.close();
        return reader.getResult();
    }
    else
        throw DataObjectException() << "ConvertJsoncppFileToData can't open file: `" + _file;
}

/// Convert Json object represented as string to DataObject
spDataObject ConvertJsoncppStringToData(string const& _input, string const& _stopper, bool _autosort)
{
    if (_input.size() < 2 || _input.find("{") == string::npos || _input.find("}") == string::npos)
        throw DataObjectException() << "ConvertJsoncppStringToData can't read json structure in file: `" + _input.substr(0, 50);

    std::vector<DataObject*> applyDepth;  // indexes at root array of objects that we are reading into
    spDataObject root;
    root.getContent().setAutosort(_autosort);
    DataObject* actualRoot = &root.getContent();
    bool keyEncountered = false;

    auto printDebug = [&_input](int _i) {
        static const short c_debugSize = 120;
        string debug;
        if (_i > c_debugSize)
            debug = _input.substr(_i - c_debugSize, c_debugSize);
        else
            debug = _input.substr(0, c_debugSize);
        return "\n\"------\n" + debug + "\n\"------";
    };

    for (size_t i = 0; i < _input.length(); i++)
    {
        bool isSeenCommaBefore = checkExcessiveComa(_input, i);
        i = stripSpaces(_input, i);
        if (i == _input.length())
            throw DataObjectException() << errorPrefix + "unexpected end of json! around: " + printDebug(i);

        const bool escapeChar = (i > 0 && _input.at(i - 1) == '\\');
        if (_input.at(i) == '"' && !escapeChar)
        {
            spDataObject obj;
            string key = parseKeyValue(_input, i);
            i = stripSpaces(_input, i);
            if (_input.at(i) == ':')
            {
                if (keyEncountered)
                    throw DataObjectException() << errorPrefix + "attempt to set key multiple times! "
                       "(like \"key\" : \"key\" : \"value\") around: " + printDebug(i);

                keyEncountered = true;
                if (actualRoot->type() == DataType::Array)
                    throw DataObjectException()
                        << errorPrefix + "array could not have elements with keys! around: " + printDebug(i);
                (*obj).setKey(key);
                bool replaceKey = false;
                const size_t keyPosExpected = _autosort ?
                                            max(0, (int)findOrderedKeyPosition(key, actualRoot->getSubObjects()) - 1) : 0;
                for (size_t objI = keyPosExpected; objI < actualRoot->getSubObjects().size(); objI++)
                {
                    if (actualRoot->getSubObjects().at(objI)->getKey() == key)
                    {
                        replaceKey = true;
                        applyDepth.push_back(actualRoot);
                        actualRoot = &actualRoot->getSubObjectsUnsafe().at(objI).getContent();
                        actualRoot->clearSubobjects();
                        break;
                    }
                }

                if (replaceKey)
                    continue;
                applyDepth.push_back(actualRoot);  // remember the header
                actualRoot = &actualRoot->addSubObject(obj);
                actualRoot->setAutosort(_autosort);
                continue;
            }
            else
            {
                keyEncountered = false;
                if (actualRoot->type() == DataType::Array)
                {
                    actualRoot->addArrayObject(spDataObject(new DataObject(std::move(key))));
                    if (_input.at(i) != ',')
                        i--;  // because cycle iteration we need to process ending clouse
                    continue;
                }
                else
                    actualRoot->setString(std::move(key));
                if (_input.at(i) != ',')
                    i--;  // because cycle iteration we need to process ending clouse
                actualRoot = applyDepth.at(applyDepth.size() - 1);
                applyDepth.pop_back();
                continue;
            }
        }

        keyEncountered = false;
        if (_input.at(i) == '{')
        {
            if (actualRoot->type() == DataType::Array || actualRoot->type() == DataType::Object)
            {
                spDataObject newObj(new DataObject(DataType::Object));
                applyDepth.push_back(actualRoot);
                actualRoot = &actualRoot->addSubObject(newObj);
                // actualRoot->setAutosort(_autosort);
                continue;
            }

            actualRoot->addSubObject(spDataObject());
            actualRoot->getSubObjectsUnsafe().pop_back();
            continue;
        }
        if (_input.at(i) == '[')
        {
            if (actualRoot->type() == DataType::Array || actualRoot->type() == DataType::Object)
            {
                spDataObject newObj(new DataObject(DataType::Array));
                (*newObj).setAutosort(_autosort);
                applyDepth.push_back(actualRoot);
                actualRoot = &actualRoot->addSubObject(newObj);

                // DataObject* newObj = &actualRoot->addSubObject(DataObject(DataType::Array));
                // newObj->setAutosort(_autosort);
                continue;
            }

            actualRoot->addArrayObject(spDataObject());
            actualRoot->getSubObjectsUnsafe().pop_back();
            continue;
        }

        if (_input.at(i) == ']' || _input.at(i) == '}')
        {
            // if (actualRoot->type() == DataType::Null)
            //    throw DataObjectException()
            //        << "lost actual root pointer around: " + printDebug(debug);
            if (isSeenCommaBefore)
                throw DataObjectException() << "unexpected ',' before end of the array/object! around: " + printDebug(i);
            if (actualRoot->type() == DataType::Array && _input.at(i) != ']')
                throw DataObjectException() << "expected ']' closing the array! around: " + printDebug(i);
            if (actualRoot->type() == DataType::Object && _input.at(i) != '}')
                throw DataObjectException()
                    << "expected '}' closing the object! around: " + printDebug(i) + ", got: `" + _input.at(i) + "'";

            if (!_stopper.empty() && actualRoot->getKey() == _stopper)
                return root;

            if (applyDepth.size() == 0)
            {
                i++;
                i = stripSpaces(_input, i);
                if (i != _input.length())
                    throw DataObjectException() << errorPrefix + "expected end of json! " + _input;
                return root;
            }
            else
            {
                actualRoot = applyDepth.at(applyDepth.size() - 1);
                applyDepth.pop_back();

                if (i + 1 < _input.length())
                {
                    if (_input.at(i + 1) == ',')
                    {
                        i++;
                        continue;
                    }
                    if (_input.at(i + 1) == ':')
                        throw DataObjectException()
                            << errorPrefix + "unexpected ':' after closing an object/array! around: " + printDebug(i);
                }
                continue;
            }
        }

        if (_input.at(i) == ',')
            throw DataObjectException() << errorPrefix + "unhendled ',' when parsing json around: " + printDebug(i);
        if (_input.at(i) == ':')
            throw DataObjectException() << errorPrefix + "unhendled ':' when parsing json around: " + printDebug(i);

        int resInt = 0;
        bool resBool = false;
        bool isReadBool = false;
        bool isReadNull = false;
        const bool isReadDigit = readDigit(_input, i, resInt);
        if (!isReadDigit)
            isReadBool = readBoolOrNull(_input, i, resBool, isReadNull);

        if (isReadDigit || isReadBool || isReadNull)
        {
            if (actualRoot->type() == DataType::Array)
            {
                if (isReadDigit)
                    actualRoot->addArrayObject(spDataObject(new DataObject(resInt)));
                else if (isReadBool)
                    actualRoot->addArrayObject(spDataObject(new DataObject(DataType::Bool, resBool)));
                else
                    actualRoot->addArrayObject(spDataObject(new DataObject(DataType::Null)));
            }
            else
            {
                if (isReadDigit)
                    actualRoot->setInt(resInt);
                else if (isReadBool)
                    actualRoot->setBool(resBool);
                else
                    actualRoot->clearSubobjects(DataType::Null);
                actualRoot = applyDepth.at(applyDepth.size() - 1);
                applyDepth.pop_back();
            }
            if (_input.at(i) != ',')
                i--;
            continue;
        }
    }
    return root;
}
}
