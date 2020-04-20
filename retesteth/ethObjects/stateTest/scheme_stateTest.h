#pragma once
#include "../expectSection/scheme_expectSection.h"
#include "../object.h"
#include "scheme_account.h"
#include "scheme_env.h"
#include "scheme_postState.h"
#include "scheme_state.h"
#include "scheme_stateTestBase.h"
#include "scheme_transaction.h"

#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace  test;
using namespace  testprivate;

namespace test {

    class scheme_stateTest: public scheme_stateTestBase
    {
        public:
        scheme_stateTest(DataObject const& _test);
        scheme_postState const& getPost() const { return m_post; }

        private:
        class fieldChecker
        {
            public:
            fieldChecker(DataObject const& _test);
        };
        fieldChecker m_checker;
        scheme_postState m_post;
    };

    class scheme_stateTestFiller: public scheme_stateTestBase
    {
        public:
        scheme_stateTestFiller(DataObject const& _test);
        scheme_expectSection const& getExpectSection() const { return m_expectSection; }
        scheme_expectSection& getExpectSectionUnsafe() { return m_expectSection; }

    private:
        class fieldChecker
        {
            public:
            fieldChecker(DataObject const& _test);
        };
        fieldChecker m_checker;
        scheme_expectSection m_expectSection;
    };
}
