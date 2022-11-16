#include "JsonReader.h"
using namespace std;
using namespace dataobject::jsonreader;

JsonInputFile::JsonInputFile(std::string const& _file) : m_filePath(_file)
{
    m_fileStream = std::ifstream(_file);
    if (!m_fileStream.is_open())
        throw DataObjectException() << "ConvertJsoncppFileToData can't open file: `" + _file;
}
size_t JsonInputFile::length() const
{

}
char const& JsonInputFile::operator[](size_t _i) const
{

}
char const& JsonInputFile::at(size_t _i) const
{

}
size_t JsonInputFile::find(char const& _ch, size_t _pos) const
{

}
std::string JsonInputFile::substr(size_t _a, size_t _b) const
{

}
std::string const& JsonInputFile::getAll() const
{

}
JsonInputFile::~JsonInputFile()
{
    m_fileStream.close();
}
