#pragma once
#include "../object.h"
#include "../stateTest/scheme_env.h"
#include "../stateTest/scheme_state.h"
#include "../stateTest/scheme_transaction.h"

#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;
namespace testprivate
{
class scheme_vmTestBase : public object
{
public:
    scheme_vmTestBase(DataObject const& _test);
    scheme_env const& getEnv() const { return m_env; }
    scheme_state const& getPre() const { return m_pre; }

private:
    class fieldChecker
    {
    public:
        fieldChecker(DataObject const& _test);
    };
    fieldChecker m_checker;
    scheme_env m_env;
    scheme_state m_pre;
};
}  // namespace testprivate
