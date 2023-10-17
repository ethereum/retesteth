#pragma once
#include <atomic>
#include <string>

namespace test::main
{
// Pre Processors
const char** preprocessOptions(int& _argc, const char* _argv[]);
void initializeOptions(int _argc, const char* _argv[]);
void expandUnitTestsArg(int _argc, const char* _argv[]);
void makeSingleTestFileSuite(int _argc, const char* _argv[]);
bool checkTestSuiteIsKnown(int argc, const char* argv[], std::string sMinusTArg = std::string());
int runTheBoostTests(int _argc, const char* _argv[]);

void lookForUnregisteredTestFolders();
void cleanMemory();
void registerBuffer(char*);
void registerFakeArgs(const char**);

void travisOut(std::atomic_bool* _stopTravisOut);
void timeoutThread(std::atomic_bool* _stopTimeout);

void printTestSuiteSuggestions(std::string const& _sMinusTArg);
void disableOutput();
void enableOutput();

}  // namespace test::main
