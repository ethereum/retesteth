#include "Common.h"
#include "EthChecks.h"
#include "TestHelper.h"
#include "TestOutputHelper.h"
#include <retesteth/ethObjects/object.h>

using namespace test;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
void checkOnlyOneTest(DataObject const& _input)
{
    ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().size() == 1,
        " A test file must contain only one test: " + TestOutputHelper::get().testFile().string());

    ETH_ERROR_REQUIRE_MESSAGE(_input.getSubObjects().at(0).type() == DataType::Object,
        TestOutputHelper::get().testFile().string() +
            " should contain an object under a test name.");
}

}  // namespace teststruct
}  // namespace test
