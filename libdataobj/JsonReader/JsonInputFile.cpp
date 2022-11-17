#include "JsonInput.h"
#include "../DataObject.h"

using namespace std;
using namespace dataobject::jsonreader;
string const msg = "JsonInputFile ";
int const cMaxChunkLength = 1000;
int const cMaxChunkLengthDelete = 2000;

void JsonInputFile::debugLine()
{
    //std::cerr << "read line `" << m_readLine << "`" << std::endl;
    //std::cerr << "min " << to_string(m_minI) << ", max " << to_string(m_maxI) << std::endl;
}

JsonInputFile::JsonInputFile(std::string const& _file) : m_filePath(_file)
{
    m_fileStream = std::ifstream(_file);
    if (!m_fileStream.is_open())
        throw DataObjectException() << msg << "can't open file: `" + _file;
    m_knownMaxLength = SIZE_MAX;

    m_fileStream.seekg(0, m_fileStream.end);
    m_knownMaxLength = m_fileStream.tellg();
    m_fileStream.seekg(0, m_fileStream.beg);
}

void JsonInputFile::readLine()
{
    size_t readSize = 0;
    size_t initialBufferSize = m_readLine.length();
    while (readSize < cMaxChunkLength)
    {
        string line;
        if (std::getline(m_fileStream, line))
        {
            m_maxI += line.length();
            readSize += line.length();
            m_readLine.append(std::move(line));
        }
        else
        {
            m_knownMaxLength = m_maxI;
            return;
        }
    }

    if (initialBufferSize > cMaxChunkLengthDelete)
    {
        size_t removeLength = cMaxChunkLength;
        m_readLine.erase(0, removeLength);
        m_minI += removeLength;
    }

    debugLine();
}

size_t JsonInputFile::length() const
{
    return m_knownMaxLength;
}

void JsonInputFile::seekFile(size_t _i)
{
    if (_i >= m_maxI)
    {
        readLine();
        if (_i > m_maxI)
        {
            std::cerr << "request " << _i << " maxI " << to_string(m_maxI) << std::endl;
            throw DataObjectException() << msg << "seekFile beyound UP cache `" + std::to_string(_i);
        }
    }
    else if (_i < m_minI)
        throw DataObjectException() << msg << "seekFile beyound cache `" + std::to_string(_i);
}

char const& JsonInputFile::_at(size_t _i)
{
    seekFile(_i);
    return m_readLine[_i - m_minI];
}

char const& JsonInputFile::operator[](size_t _i)
{
    return _at(_i);
}

char const& JsonInputFile::at(size_t _i)
{
    return _at(_i);
}

size_t JsonInputFile::find(char const& _ch, size_t _pos)
{
    seekFile(_pos);


        //for (size_t i = m_minI + _pos; i < m_readLine.length(); i++)
        //{
        //    if (m_readLine.at(i) == _ch)
        //        return i - m_minI;
        //}

    size_t seekI = m_minI;
    size_t posFound = _pos;
    for (auto const& ch : m_readLine)
    {
        if (seekI++ < _pos)
            continue;
        if (ch == _ch)
            return posFound;
        posFound++;
    }


    return string::npos;
}

std::string JsonInputFile::substr(size_t _a, size_t _b)
{
    //seekFile(_a);
    if (_a < m_minI || _a >= m_maxI)
        throw DataObjectException() << msg << "substr _a(" + to_string(_a) + ") beyound cache: `"
            + to_string(m_minI) + " ... " + to_string(m_maxI);
    if (_b != string::npos)
    {
        if (_b + m_minI >= m_maxI)
            throw DataObjectException() << msg << "substr _b beyound cache `";

        return m_readLine.substr(_a - m_minI, _b);
    }

    return m_readLine.substr(_a - m_minI);
}
std::string const& JsonInputFile::getAll() const
{
    return m_readLine;
}
JsonInputFile::~JsonInputFile()
{
    m_fileStream.close();
}
