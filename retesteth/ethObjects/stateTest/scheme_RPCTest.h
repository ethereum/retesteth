#pragma once
#include "../object.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;
namespace testprivate
{
class scheme_RPCTestBase : public object
{
public:
    scheme_RPCTestBase(DataObject const& _test) : object(_test) {}
    std::string get_method() { return m_data.at("request_method").asString(); }
    std::vector<std::string> get_params()
    {
        // no static here because of the threads
        std::vector<std::string> params;
        for (auto const& d : m_data.at("request_params").getSubObjects())
            params.push_back("\"" + d.asString() + "\"");
        return params;
    }
};
}

using namespace testprivate;
namespace test
{
class scheme_RPCTest : public scheme_RPCTestBase
{
public:
    scheme_RPCTest(DataObject const& _test) : scheme_RPCTestBase(_test)
    {
        requireJsonFields(_test, "rpcTest " + _test.getKey(),
            {{"request_method", {DataType::String}}, {"request_params", {DataType::Array}}});
    }
};

class scheme_RPCTestFiller : public scheme_RPCTestBase
{
public:
    scheme_RPCTestFiller(DataObject const& _test) : scheme_RPCTestBase(_test)
    {
        requireJsonFields(_test, "rpcTest " + _test.getKey(),
            {{"request_method", {DataType::String}}, {"request_params", {DataType::Array}},
                {"expect_return", {DataType::Object}}});
    }
    DataObject get_return() { return m_data.at("expect_return"); }
};
}
