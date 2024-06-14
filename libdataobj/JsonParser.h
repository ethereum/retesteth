#pragma once
#include "DataObject.h"
#include "ConvertFile.h"

namespace dataobject
{

class JsonParser
{
public:
    JsonParser(std::string const& _input, CJOptions const& _opt = CJOptions());
    void parse();
    spDataObject root() { return  m_root; }
private:
    enum class RET
    {
        CONTINUE,
        GOON,
        RETURN
    };
private:
    void keyEncountered() { m_keyEncountered = true; }
    std::string printDebug(size_t const& _i) const;
    bool isEmptyChar(char const& _char) const;
    RET tryParseKeyValue(size_t& _i);
    RET tryParseArrayBegin(size_t const& _i);
    RET tryParseArrayEnd(size_t& _i, bool);
    RET tryParseDigitBoolNull(size_t& _i);

private:
    // Work with iterator i
    bool checkExcessiveComaBefore(size_t const& _i) const;
    size_t skipSpaces(size_t const& _i) const;
    std::string parseKeyValue(size_t& _i) const;
    bool readBoolOrNull(size_t& _i, bool& _result, bool& _readNull) const;
    bool isEscapeChar(size_t _i) const;

    enum class ReadDigitType
    {
        INT,
        DOUBLE,
        NONE
    };
    ReadDigitType readDigit(size_t& _i, int& _result, double& _doubleResult) const;
    void checkJsonCommaEnding(size_t& _i) const;
private:
    std::string const& m_input;
    CJOptions const m_opt;

    std::vector<DataObject*> m_applyDepth;  // indexes at root array of objects that we are reading into
    spDataObject m_root;
    DataObject* m_actualRoot = nullptr;
    bool m_keyEncountered = false;
    std::string const errorPrefix = "Error parsing json: ";
};

}
