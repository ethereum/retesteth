#include "helpers/TestHelper.h"
#include "StateTestsHelper.h"
#include "StateTestFillerRunner.h"
#include <retesteth/helpers/TestOutputHelper.h>
#include "Options.h"
#include "session/Session.h"
#include "testSuites/Common.h"
#include "testStructures/PrepareChainParams.h"
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <testStructures/types/Ethereum/Transactions/TransactionLegacy.h>

using namespace test;
using namespace std;
using namespace test::debug;
using namespace test::session;
using namespace test::statetests;

namespace
{

string const testInclude = R"(
import pytest

from ethereum_test_tools import Account, Code, Environment
from ethereum_test_tools import StateTestFiller, TestAddress, Transaction

)";


string const expectInclude = R"(
from collections import namedtuple
from typing import Any, Mapping, Optional

import pytest

from ethereum_test_tools import Account, Code, Environment
from ethereum_test_tools import StateTestFiller, TestAddress, Transaction


ExpectSectionIndex = namedtuple("ExpectSectionIndex", ["d", "g", "v", "fork"])

class ExpectSection:
    """Manage expected post states for state tests transactions"""

    def __init__(self):
        self.sections: list[tuple[ExpectSectionIndex, dict[str, Account]]] = []

    def add_expect(self, ind: ExpectSectionIndex, expect: dict[str, Account]) -> None:
        """Adds a section with a given indexes and expect dictionary."""
        self.sections.append((ind, expect))

    def get_expect(self, tx_ind: ExpectSectionIndex) -> Optional[dict[str, Account]]:
        """Returns the element associated with the given id, if it exists."""
        for section_ind, section in self.sections:
            if (
                (tx_ind.d in section_ind.d or section_ind.d == -1)
                and (tx_ind.g in section_ind.g or section_ind.g == -1)
                and (tx_ind.v in section_ind.v or section_ind.v == -1)
            ):
                return section
        return None
)";

}

namespace test::statetests
{

std::string InsertTab(size_t _number, std::string _str, bool _exeptFirstLast = false)
{
    std::string tabs(_number * 4, ' ');

    if (!_exeptFirstLast)
        _str.insert(0, tabs);
    for (size_t i = 0; i < _str.size() - 1; ++i)
    {
        if (_str[i] == '\n' && _str[i + 1] != '\n')
        {
            if (_exeptFirstLast && _str.find('\n', i + 1) == string::npos)
                break;
            _str.insert(i + 1, tabs);
            i += _number;
        }
    }
    return _str;
}

string convertComment(StateTestInFiller const& _test, TestSuite::TestSuiteOptions& _opt)
{
    if (_test.hasInfo())
    {
        string res = "\"\"\"\n";
        res += "Converted test: " + _opt.pathToFiller.string() + "\n";
        res += "Retesteth version: " + prepareVersionString() + "\n";
        res +=  _test.Info().rawData()->atKey("comment").asString() + "\n";
        res += "\"\"\"\n" ;
        return res;
    }
    return string();
}

string defineForksParameter(StateTestInFiller const& _test)
{
    string forks;
    for (auto const& fork : _test.getAllForksFromExpectSections())
        forks += "\"" + fork.asString() + "\", ";
    forks.erase(forks.rfind(','));

    string fork = "@pytest.mark.parametrize(\"fork\", [" + forks + "])\n";
    return fork;
}

string defineTestName(StateTestInFiller const& _test)
{
    string res;
    res += "def test_" + _test.testName() + "_py(state_test: StateTestFiller):\n";
    return res;
}

string defineCommentAfterTestName(StateTestInFiller const& _test)
{
    string res = "\"\"\"\n";
    if (_test.hasInfo())
        res += _test.Info().rawData()->atKey("comment").asString() + "\n";
    else
        res += "Converted into python state test" + _test.testName() + "\n";
    res += "\"\"\"\n\n";
    return res;
}

string defineEnvironment(StateTestInFiller const& _test)
{
    auto const& env = _test.Env();
    string res = R"(
env = Environment(
    coinbase=")" + env.currentCoinbase().asString() + R"(",
    difficulty=)" + env.currentDifficulty().asString() + R"(,
    gas_limit=)" + env.currentGasLimit().asDecString() + R"(,
    number=)" + env.currentNumber().asDecString() + R"(,
    timestamp=)" + env.firstBlockTimestamp().asDecString() + R"(,
))";
    res += "\n\n";
    return res;
}

void defineAccount(string& _res, spAccountBase _acc)
{
    auto const& addr = _acc->address();
    if (_acc->shouldNotExist())
    {
        _res += "    \"" + addr.asString() + "\": Account.NONEXISTENT,\n";
        return;
    }

    _res += "    \"" + addr.asString() + "\": Account(\n";
    if (_acc->hasBalance())
        _res += "        balance=" + _acc->balance().asDecString() + ",\n";
    if (_acc->hasNonce())
        _res += "        nonce=" + _acc->nonce().asDecString() + ",\n";
    if (_acc->hasCode())
        _res += "        code=\"" + _acc->code().asString() + "\",\n";
    if (_acc->hasStorage())
    {
        _res += "        storage="
                + InsertTab(2, _acc->storage().asDataObject()->asJsonNoFirstKey(), true)
                + ",\n";
        _res.insert(_res.end() - 3, 2, '\t');
    }
    _res += "    ),\n";
}

string definePre(StateTestInFiller const& _test)
{
    string res = "pre = {\n";
    for (auto const& [addr, acc] : _test.Pre().accounts())
        defineAccount(res, acc);
    res += "}\n\n";
    return res;
}

string defineTransactionStructure(spTransaction _tr)
{
    string res;
    if (_tr->type() == TransactionType::LEGACY)
    {
        auto const& legacyTr = static_cast<TransactionLegacy const&>(_tr.getCContent());
        res = R"(tx = Transaction(
        ty=0x0,
        chain_id=0x0,
        nonce=)" + legacyTr.nonce().asDecString() + R"(,
        to=")" + legacyTr.to().asString() + R"(",
        gas_price=)" + legacyTr.gasPrice().asDecString() + R"(,
        protected=False,
))";
        res += "\n\n";
    }
    return res;
}

string defineOnePostTransaction(StateTestInFiller const& _test)
{
    string res;
    auto txs = _test.GeneralTr().buildTransactions();
    res += defineTransactionStructure(txs.at(0).transaction());
    for (auto const& tx : txs)
    {
        res += "tx.data=\"" + tx.transaction()->data().asString() + "\"\n";
        res += "tx.gas_limit=" + tx.transaction()->gasLimit().asDecString() + "\n";
        res += "tx.value=" + tx.transaction()->value().asDecString() + "\n";
        res += "state_test(env=env, pre=pre, post=post, txs=[tx])\n\n";
    }
    return res;
}

string definePostAndTransactions(StateTestInFiller const& _test)
{
    string res;
    bool onlyOne = (_test.Expects().size() == 1);
    if (onlyOne)
    {
        res = "post = {\n";
        for (auto const& [addr, acc] : _test.Expects().at(0).result().accounts())
            defineAccount(res, acc);
        res += "}\n\n";
        res += defineOnePostTransaction(_test);
    }
    else
    {
        res = "expect_section = ExpectSection()\n";

        for (auto const& expect : _test.Expects())
        {
            res += "expect_section.add_expect(ExpectSectionIndex(d=[0], g=[0], v=[0, 1], fork=Shanghai), post_1)";
        }
    }
    return res;
}

spDataObject ConvertpyTest(StateTestInFiller const& _test, TestSuite::TestSuiteOptions& _opt)
{
    spDataObject res = sDataObject(DataType::String);
    auto& s = (*res).asStringUnsafe();
    s += convertComment(_test, _opt);

    if (_test.Expects().size() == 1)
        s += testInclude + "\n";
    else
        s += expectInclude + "\n";

    //s += defineForksParameter(_test);
    s += defineTestName(_test);
    s += InsertTab(1, defineCommentAfterTestName(_test));
    s += InsertTab(1, defineEnvironment(_test));
    s += InsertTab(1, definePre(_test));
    s += InsertTab(1, definePostAndTransactions(_test));

    (void)_test;
    return res;
}
}
