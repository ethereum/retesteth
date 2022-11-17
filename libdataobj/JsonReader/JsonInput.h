#pragma once
#include <fstream>
#include <iostream>

namespace dataobject::jsonreader
{

class JsonInput
{
public:
    virtual size_t length() const = 0;
    virtual char const& operator[](size_t _i) = 0;
    virtual char const& at(size_t _i) = 0;
    virtual size_t find(char const& _ch, size_t _pos)  = 0;
    virtual std::string substr(size_t _a, size_t _b)  = 0;
    virtual std::string const& getAll() const  = 0;
    virtual ~JsonInput() {};
};

class JsonInputString : public JsonInput
{
public:
    JsonInputString(std::string const& _input) : m_input(_input){}
    size_t length() const override { return m_input.length(); }
    char const& operator[](size_t _i) override { return m_input[_i]; }
    char const& at(size_t _i) override { return m_input.at(_i); }
    size_t find(char const& _ch, size_t _pos) override {
        return m_input.find(_ch, _pos);
    }
    std::string substr(size_t _a, size_t _b) override { return m_input.substr(_a, _b); }
    std::string const& getAll() const override { return m_input; }
private:
    std::string const& m_input;
};

class JsonInputFile : public JsonInput
{
public:
    JsonInputFile(std::string const& _file);
    size_t length() const override;
    char const& operator[](size_t _i) override;
    char const& at(size_t _i) override;
    size_t find(char const& _ch, size_t _pos) override;
    std::string substr(size_t _a, size_t _b) override;
    std::string const& getAll() const override;
    ~JsonInputFile() override;

private:
    std::string const& m_filePath;
    std::ifstream m_fileStream;
    size_t m_knownMaxLength;

    void seekFile(size_t _i);
    void readLine();
    char const& _at(size_t _i);
    void debugLine();

private:
    std::string m_readLine;
    size_t m_minI = 0;
    size_t m_maxI = 0;
    char m_readChar;
};
}
