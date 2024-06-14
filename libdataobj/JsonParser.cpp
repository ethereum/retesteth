#include "JsonParser.h"
#include "Exception.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;
using namespace dataobject;

JsonParser::JsonParser(std::string const& _input, CJOptions const& _opt)
  : m_input(_input), m_opt(_opt)
{
    if (_input.size() < 2 || _input.find("{") == string::npos || _input.rfind("}") == string::npos)
        throw DataObjectException() << "ConvertJsoncppStringToData can't read json structure in file: `" + _input.substr(0, 50);

    m_applyDepth.clear();
    m_root.getContent().setAutosort(_opt.autosort);
    m_actualRoot = &m_root.getContent();
}

string JsonParser::printDebug(size_t const& _i) const
{
    static const short c_debugSize = 120;
    string debug;
    if (_i > c_debugSize)
        debug = m_input.substr(_i - c_debugSize, c_debugSize);
    else
        debug = m_input.substr(0, c_debugSize);
    return "\n\"------\n" + debug + "\n\"------";
}

void JsonParser::parse()
{
    for (size_t i = 0; i < m_input.length(); i++)
    {
        bool isSeenCommaBefore = checkExcessiveComaBefore(i);
        i = skipSpaces(i);
        if (i == m_input.length())
            throw DataObjectException() << errorPrefix + "unexpected end of json! around: " + printDebug(i);

        if (tryParseKeyValue(i) == RET::CONTINUE)
            continue;

        m_keyEncountered = false;

        if (tryParseArrayBegin(i) == RET::CONTINUE)
            continue;

        auto const arrayEndResult = tryParseArrayEnd(i, isSeenCommaBefore);
        if (arrayEndResult == RET::CONTINUE)
            continue;
        if (arrayEndResult == RET::RETURN)
            return;

        tryParseDigitBoolNull(i);
    }
}

void JsonParser::checkJsonCommaEnding(size_t& _i) const
{
    if (m_input.at(_i) == '}' || m_input.at(_i) == ']')
        _i--;  // because cycle iteration we need to process ending clouse
    else if (m_input.at(_i) != ',')
        throw DataObjectException() << errorPrefix
            + "Dataobject array/object expected ',' when listing elements, but got `" + m_input.at(_i) + "`"
            + "around: " + printDebug(_i);
}

bool JsonParser::isEscapeChar(size_t _i) const
{
    if (_i < 1)
        return m_input.at(_i) == '\\';
    if (m_input.at(_i) == '\\' && m_input.at(_i - 1) != '\\')
        return true;
    return false;
}

JsonParser::RET JsonParser::tryParseKeyValue(size_t& _i)
{
    if (m_input.at(_i) == '"' && _i > 0 && !isEscapeChar(_i - 1))
    {
        spDataObject obj;
        string key = parseKeyValue(_i);

        _i = skipSpaces(_i);
        if (m_input.at(_i) == ':')
        {
            if (m_keyEncountered)
                throw DataObjectException() << errorPrefix + "attempt to set key multiple times! "
                                  "(like \"key\" : \"key\" : \"value\") around: " + printDebug(_i);

            m_keyEncountered = true;
            if (m_actualRoot->type() == DataType::Array)
                throw DataObjectException()
                    << errorPrefix + "array could not have elements with keys! around: " + printDebug(_i);
            (*obj).setKey(key);

            m_applyDepth.push_back(m_actualRoot);
            if (m_actualRoot->count(key))
            {
                bool allowedComment = false;
                if (m_opt.jsonParse == CJOptions::JsonParse::ALLOW_COMMENTS
                    && key.size() > 2 && key.at(0) == '/' && key.at(1) == '/')
                    allowedComment = true;

                if (m_opt.jsonParse == CJOptions::JsonParse::STRICT_JSON || !allowedComment)
                {
                    throw DataObjectException() << errorPrefix
                       + "ConvertJsoncppStringToData::Error: Reading json with dublicate fields: `"
                       + key + "` around: " + printDebug(_i) + "\n";
                }
                m_actualRoot = &m_actualRoot->atKeyPointerUnsafe(key).getContent();
                m_actualRoot->clearSubobjects();
                return RET::CONTINUE;
            }

            m_actualRoot = &m_actualRoot->addSubObject(obj);
            m_actualRoot->setAutosort(m_opt.autosort);
            return RET::CONTINUE;
        }
        else
        {
            m_keyEncountered = false;
            if (m_actualRoot->type() == DataType::Array)
            {
                m_actualRoot->addArrayObject(sDataObject(std::move(key)));
                checkJsonCommaEnding(_i);
                return RET::CONTINUE;
            }
            else
                m_actualRoot->setString(std::move(key));
            checkJsonCommaEnding(_i);
            m_actualRoot = m_applyDepth.at(m_applyDepth.size() - 1);
            m_applyDepth.pop_back();
            return RET::CONTINUE;
        }
    }
    return RET::GOON;
}

JsonParser::RET JsonParser::tryParseArrayBegin(size_t const& _i)
{
    if (m_input.at(_i) == '{')
    {
        if (m_actualRoot->type() == DataType::Array || m_actualRoot->type() == DataType::Object)
        {
            spDataObject newObj = sDataObject(DataType::Object);
            m_applyDepth.push_back(m_actualRoot);
            m_actualRoot = &m_actualRoot->addSubObject(newObj);
            return RET::CONTINUE;
        }

        m_actualRoot->addSubObject(spDataObject());
        m_actualRoot->getSubObjectsUnsafe().pop_back();
        return RET::CONTINUE;
    }

    if (m_input.at(_i) == '[')
    {
        if (m_actualRoot->type() == DataType::Array || m_actualRoot->type() == DataType::Object)
        {
            spDataObject newObj = sDataObject(DataType::Array);
            (*newObj).setAutosort(m_opt.autosort);
            m_applyDepth.push_back(m_actualRoot);
            m_actualRoot = &m_actualRoot->addSubObject(newObj);
            return RET::CONTINUE;
        }

        m_actualRoot->addArrayObject(spDataObject());
        m_actualRoot->getSubObjectsUnsafe().pop_back();
        return RET::CONTINUE;
    }
    return RET::GOON;
}

JsonParser::RET JsonParser::tryParseArrayEnd(size_t& _i, bool _seenCommaBefore)
{
    if (m_input.at(_i) == ']' || m_input.at(_i) == '}')
    {
        // if (actualRoot->type() == DataType::Null)
        //    throw DataObjectException()
        //        << "lost actual root pointer around: " + printDebug(debug);
        if (_seenCommaBefore)
            throw DataObjectException() << "unexpected ',' before end of the array/object! around: " + printDebug(_i);
        if (m_actualRoot->type() == DataType::Array && m_input.at(_i) != ']')
            throw DataObjectException() << "expected ']' closing the array! around: " + printDebug(_i);
        if (m_actualRoot->type() == DataType::Object && m_input.at(_i) != '}')
            throw DataObjectException()
                << "expected '}' closing the object! around: " + printDebug(_i) + ", got: `" + m_input.at(_i) + "'";

        if (!m_opt.stopper.empty() && m_actualRoot->getKey() == m_opt.stopper)
            return RET::RETURN;

        if (m_applyDepth.size() == 0)
        {
            _i++;
            _i = skipSpaces(_i);
            if (_i != m_input.length())
                throw DataObjectException() << errorPrefix + "expected end of json! " + m_input;
            return RET::RETURN;
        }
        else
        {
            m_actualRoot = m_applyDepth.at(m_applyDepth.size() - 1);
            m_applyDepth.pop_back();

            if (_i + 1 < m_input.length())
            {
                if (m_input.at(_i + 1) == ',')
                {
                    _i++;
                    return RET::CONTINUE;
                }
                if (m_input.at(_i + 1) == ':')
                    throw DataObjectException()
                        << errorPrefix + "unexpected ':' after closing an object/array! around: " + printDebug(_i);
            }
            return RET::CONTINUE;
        }
    }

    if (m_input.at(_i) == ',')
        throw DataObjectException() << errorPrefix + "unhendled ',' when parsing json around: " + printDebug(_i);
    if (m_input.at(_i) == ':')
        throw DataObjectException() << errorPrefix + "unhendled ':' when parsing json around: " + printDebug(_i);

    return RET::GOON;
}

JsonParser::RET JsonParser::tryParseDigitBoolNull(size_t& _i)
{
    double resDouble = 0.0;
    int resInt = 0;
    bool resBool = false;
    bool isReadBool = false;
    bool isReadNull = false;
    const JsonParser::ReadDigitType isReadDigit = readDigit(_i, resInt, resDouble);
    if (isReadDigit == JsonParser::ReadDigitType::NONE)
        isReadBool = readBoolOrNull(_i, resBool, isReadNull);

    if (isReadDigit != JsonParser::ReadDigitType::NONE || isReadBool || isReadNull)
    {
        if (m_actualRoot->type() == DataType::Array)
        {
            if (isReadDigit == JsonParser::ReadDigitType::INT)
                m_actualRoot->addArrayObject(sDataObject(DataType::Integer, (int)resInt));
            else if (isReadDigit == JsonParser::ReadDigitType::DOUBLE)
                m_actualRoot->addArrayObject(sDataObject(DataType::Double, (double)resDouble));
            else if (isReadBool)
                m_actualRoot->addArrayObject(sDataObject(DataType::Bool, resBool));
            else
                m_actualRoot->addArrayObject(sDataObject(DataType::Null));
        }
        else
        {
            if (isReadDigit == JsonParser::ReadDigitType::INT)
                m_actualRoot->setInt(resInt);
            else if (isReadDigit == JsonParser::ReadDigitType::DOUBLE)
                m_actualRoot->setDouble(resDouble);
            else if (isReadBool)
                m_actualRoot->setBool(resBool);
            else
                m_actualRoot->clearSubobjects(DataType::Null);
            m_actualRoot = m_applyDepth.at(m_applyDepth.size() - 1);
            m_applyDepth.pop_back();
        }
        if (m_input.at(_i) != ',')
            _i--;
        return RET::CONTINUE;
    }
    return RET::GOON;
}

bool JsonParser::isEmptyChar(char const& _char) const
{
    static constexpr char emptyChars[] = {' ', '\n', '\r', '\t'};
    return std::any_of(std::begin(emptyChars), std::end(emptyChars), [_char](char c) { return c == _char; });
}

size_t JsonParser::skipSpaces(size_t const& _i) const
{
    auto const it = std::find_if_not(m_input.begin() + _i, m_input.end(), [this](char c) { return isEmptyChar(c); });
    return std::distance(m_input.begin(), it);
}

string JsonParser::parseKeyValue(size_t& _i) const
{
    if (_i + 1 > m_input.size())
        throw DataObjectException() << errorPrefix + "reached EOF before reading char: `\"` around: " + printDebug(_i);

    bool escapeChar = true;
    size_t endPos = m_input.find('"', _i + 1);
    if (endPos != string::npos)
    {
        while (escapeChar)
        {
            escapeChar = isEscapeChar(endPos - 1);
            if (escapeChar)
                endPos = m_input.find('"', endPos + 1);
        }

        const string key = m_input.substr(_i + 1, endPos - _i - 1);
        _i = endPos + 1;
        return key;
    }
    else
        throw DataObjectException() << errorPrefix + "not found key ending char: `\"` around: " + printDebug(_i);
    return string();
}

bool JsonParser::readBoolOrNull(size_t& _i, bool& _result, bool& _readNull) const
{
    if (_i + 4 >= m_input.size())
        return false;

    // true false
    const string text = m_input.substr(_i, 4);
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
        if (m_input.substr(_i, 5) == "false")
        {
            _i += 5;
            _result = false;
            return true;
        }
    }
    return false;
}

JsonParser::ReadDigitType JsonParser::readDigit(size_t& _i, int& _result, double& _resultDouble) const
{
    bool readDouble = false;
    bool readMinus = false;
    auto const& e = m_input[_i];
    if (e == '-')
    {
        readMinus = true;
        _i++;
    }

    bool digit = true;
    string readNumber;
    while (digit)
    {
        auto const& e = m_input[_i];
        if (e == '0' || e == '1' || e == '2' || e == '3' || e == '4' || e == '5' || e == '6' ||
            e == '7' || e == '8' || e == '9' || e == '.')
        {
            if (e == '.' && readDouble)
            {
                _i++;
                digit = false;
                _i = skipSpaces(_i);
                continue;
            }

            readNumber += e;
            if (e == '.')
                readDouble = true;
            _i++;
        }
        else
        {
            digit = false;
            _i = skipSpaces(_i);
        }
    }
    if (readNumber.size())
    {
        if (readDouble)
        {
            std::locale::global(std::locale("C"));
            _resultDouble = std::stod(readNumber);
            if (readMinus)
                _resultDouble *= -1;
            return JsonParser::ReadDigitType::DOUBLE;
        }
        else
        {
            _result = std::atoi(readNumber.c_str());
            if (readMinus)
                _result *= -1;
            return JsonParser::ReadDigitType::INT;
        }
    }
    return JsonParser::ReadDigitType::NONE;
}

bool JsonParser::checkExcessiveComaBefore(size_t const& _i) const
{
    if (_i < 1)
        return false;
    size_t reader = _i - 1;
    while (isEmptyChar(m_input[reader]) && reader != 0)
        reader--;
    if (m_input[reader] == ',')
        return true;
    return false;
}
