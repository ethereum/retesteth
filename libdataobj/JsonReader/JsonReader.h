#pragma once
#include "../DataObject.h"
#include <fstream>
#include <iostream>

namespace dataobject::jsonreader
{

class JsonInput
{
    public:
        virtual size_t length() const = 0;
        virtual char const& operator[](size_t _i) const = 0;
        virtual char const& at(size_t _i) const = 0;
        virtual size_t find(char const& _ch, size_t _pos) const  = 0;
        virtual std::string substr(size_t _a, size_t _b) const  = 0;
        virtual std::string const& getAll() const  = 0;
        virtual ~JsonInput() {};
};

class JsonInputString : public JsonInput
{
public:
    JsonInputString(std::string const& _input) : m_input(_input){}
    size_t length() const override { return m_input.length(); }
    char const& operator[](size_t _i) const override { return m_input[_i]; }
    char const& at(size_t _i) const override { return m_input.at(_i); }
    size_t find(char const& _ch, size_t _pos) const override {
        return m_input.find(_ch, _pos);
    }
    std::string substr(size_t _a, size_t _b) const override { return m_input.substr(_a, _b); }
    std::string const& getAll() const override { return m_input; }
private:
    std::string const& m_input;
};

class JsonInputFile : public JsonInput
{
public:
    JsonInputFile(std::string const& _file);
    size_t length() const override;
    char const& operator[](size_t _i) const override;
    char const& at(size_t _i) const override;
    size_t find(char const& _ch, size_t _pos) const override;
    std::string substr(size_t _a, size_t _b) const override;
    std::string const& getAll() const override;
    ~JsonInputFile() override;
private:
    std::string const& m_filePath;
    std::ifstream m_fileStream;
};

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
