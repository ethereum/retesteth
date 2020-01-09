#include <dataObject/ConvertFile.h>
#include <dataObject/Exception.h>
// Manually construct dataobject from file string content
// bacuse Json::Reader::parse has a memory leak

namespace dataobject
{
string const errorPrefix = "Error parsing json: ";

size_t stripSpaces(string const& _input, size_t _i)
{
    size_t i = _i;
    for (; i < _input.length(); i++)
    {
        if (_input[i] == ' ' || _input[i] == '\n' || _input[i] == '\r' || _input[i] == '\t')
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

    size_t endPos = _input.find('"', _i + 1);
    if (endPos != string::npos)
    {
        string key = _input.substr(_i + 1, endPos - _i - 1);
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
    string text = _input.substr(_i, 4);
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
        if (_i + 5 < _input.size() && (_input[_i + 5] == 'e'))
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
    char e = _input[_i];
    if (e == '-')
    {
        readMinus = true;
        _i++;
    }

    bool digit = true;
    string readNumber;
    while (digit)
    {
        e = _input[_i];
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

/// Convert Json object represented as string to DataObject
DataObject ConvertJsoncppStringToData(
    std::string const& _input, string const& _stopper, bool _autosort)
{
    std::vector<DataObject*> applyDepth;  // indexes at root array of objects that we are reading
                                          // into
    DataObject root;
    root.setAutosort(_autosort);
    DataObject* actualRoot = &root;
    bool keyEncountered = false;
    const short c_debugSize = 120;
    string debug = _input.substr(0, c_debugSize);
    auto printDebug = [](string const& _debug) { return "\n\"------\n" + _debug + "\n\"------"; };
    for (size_t i = 0; i < _input.length(); i++)
    {
        // std::cerr << root.asJson() << std::endl;
        i = stripSpaces(_input, i);
        if (i == _input.length())
            throw DataObjectException()
                << errorPrefix + "unexpected end of json! around: " + printDebug(debug);

        if (i > c_debugSize)
            debug = _input.substr(i - c_debugSize, c_debugSize / 2);

        if (_input[i] == '"')
        {
            DataObject obj;
            string key = parseKeyValue(_input, i);
            i = stripSpaces(_input, i);
            if (_input.at(i) == ':')
            {
                if (keyEncountered)
                    throw DataObjectException() << errorPrefix +
                                                       "attempt to set key multiple times! (like "
                                                       "\"key\" : \"key\" : \"value\") around: " +
                                                       printDebug(debug);

                keyEncountered = true;
                if (actualRoot->type() == DataType::Array)
                    throw DataObjectException()
                        << errorPrefix + "array could not have elements with keys! around: " +
                               printDebug(debug);
                obj.setKey(key);
                bool replaceKey = false;
                size_t keyPosExpected = _autosort ?
                        max(0, (int)findOrderedKeyPosition(key, actualRoot->getSubObjects()) - 1) : 0;
                for (size_t objI = keyPosExpected; objI < actualRoot->getSubObjects().size(); objI++)
                {
                    if (actualRoot->getSubObjects().at(objI).getKey() == key)
                    {
                        replaceKey = true;
                        applyDepth.push_back(actualRoot);
                        actualRoot = &actualRoot->getSubObjectsUnsafe().at(objI);
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
                    actualRoot->addArrayObject(DataObject(key));
                    if (_input.at(i) != ',')
                        i--;  // because cycle iteration we need to process ending clouse
                    continue;
                }
                else
                    actualRoot->setString(key);
                if (_input.at(i) != ',')
                    i--;  // because cycle iteration we need to process ending clouse
                actualRoot = applyDepth.at(applyDepth.size() - 1);
                applyDepth.pop_back();
                continue;
            }
        }

        keyEncountered = false;
        if (_input[i] == '{')
        {
            if (actualRoot->type() == DataType::Array || actualRoot->type() == DataType::Object)
            {
                DataObject newObj(DataType::Object);
                applyDepth.push_back(actualRoot);
                actualRoot = &actualRoot->addSubObject(newObj);
                actualRoot->setAutosort(_autosort);
                continue;
            }

            actualRoot->addSubObject(DataObject());
            actualRoot->getSubObjectsUnsafe().pop_back();
            continue;
        }
        if (_input[i] == '[')
        {
            if (actualRoot->type() == DataType::Array || actualRoot->type() == DataType::Object)
            {
                DataObject* newObj = &actualRoot->addSubObject(DataObject(DataType::Array));
                newObj->setAutosort(_autosort);
                continue;
            }

            actualRoot->addArrayObject(DataObject());
            actualRoot->getSubObjectsUnsafe().pop_back();
            continue;
        }

        if (_input[i] == ']' || _input[i] == '}')
        {
            if (actualRoot->type() == DataType::Array && _input.at(i) != ']')
                throw DataObjectException()
                    << "expected ']' closing the array! around: " + printDebug(debug);
            if (actualRoot->type() == DataType::Object && _input.at(i) != '}')
                throw DataObjectException()
                    << "expected '}' closing the object! around: " + printDebug(debug);

            if (!_stopper.empty() && actualRoot->getKey() == _stopper)
                return root;

            if (applyDepth.size() == 0)
            {
                i++;
                i = stripSpaces(_input, i);
                if (i != _input.length())
                    throw DataObjectException() << errorPrefix + "expected end of json!";
                return root;
            }
            else
            {
                actualRoot = applyDepth.at(applyDepth.size() - 1);
                applyDepth.pop_back();
                if (i + 1 < _input.length())
                {
                    if (_input[i + 1] == ',')
                        i++;
                    if (_input[i + 1] == ':')
                        throw DataObjectException()
                            << errorPrefix +
                                   "unexpected ':' after closing an object/array! around: " +
                                   printDebug(debug);
                }
                continue;
            }
        }

        if (_input.at(i) == ',')
            throw DataObjectException()
                << errorPrefix + "unhendled ',' when parsing json around: " + printDebug(debug);
        if (_input.at(i) == ':')
            throw DataObjectException()
                << errorPrefix + "unhendled ':' when parsing json around: " + printDebug(debug);

        int resInt = 0;
        bool resBool = false;
        bool isReadBool = false;
        bool isReadNull = false;
        bool isReadDigit = readDigit(_input, i, resInt);
        if (!isReadDigit)
            isReadBool = readBoolOrNull(_input, i, resBool, isReadNull);

        if (isReadDigit || isReadBool || isReadNull)
        {
            if (actualRoot->type() == DataType::Array)
            {
                if (isReadDigit)
                    actualRoot->addArrayObject(DataObject(resInt));
                else if (isReadBool)
                    actualRoot->addArrayObject(DataObject(DataType::Bool, resBool));
                else
                    actualRoot->addArrayObject(DataObject());
            }
            else
            {
                if (isReadDigit)
                    actualRoot->setInt(resInt);
                else if (isReadBool)
                    actualRoot->setBool(resBool);
                else
                    actualRoot->clearSubobjects();
                actualRoot = applyDepth.at(applyDepth.size() - 1);
                applyDepth.pop_back();
            }
            if (_input[i] != ',')
                i--;
            continue;
        }
    }
    return root;
}
}
