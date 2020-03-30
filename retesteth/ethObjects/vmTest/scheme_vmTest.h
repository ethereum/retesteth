#pragma once
#include "../expectSection/scheme_expectSection.h"
#include "../object.h"
#include "scheme_vmTestBase.h"

#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;
using namespace testprivate;

namespace test
{
/*
    class scheme_vmTest : public scheme_vmTestBase
    {
        public:
        scheme_vmTest(DataObject const& _test);

        private:
        class fieldChecker
        {
            public:
            fieldChecker(DataObject const& _test);
        };
        fieldChecker m_checker;
    };
*/

class scheme_vmTestFiller : public scheme_vmTestBase
{
public:
    scheme_vmTestFiller(DataObject const& _test);
    scheme_generalTransaction const& getTransaction() { return m_gtransaction; }

private:
    class fieldChecker
    {
    public:
        fieldChecker(DataObject const& _test);
    };
    fieldChecker m_checker;
    scheme_generalTransaction m_gtransaction;
};
}  // namespace test
