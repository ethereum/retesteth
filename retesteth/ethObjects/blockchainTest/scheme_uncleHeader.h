#pragma once
#include "../object.h"
#include "../stateTest/scheme_state.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;

namespace test
{
class scheme_uncleHeader : public object
{
public:
    scheme_uncleHeader(DataObject const& _test);
    size_t getPopulateFrom() const;
    string const& getOverwrite() const;
    string const& getRelTimestampFromPopulateBlock() const;

private:
};
}  // namespace test
