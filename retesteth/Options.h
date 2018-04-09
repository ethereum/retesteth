
#pragma once

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

//#include <test/tools/libtestutils/Common.h>
//#include <test/tools/libtesteth/JsonSpiritHeaders.h>
#include <libdevcore/Exceptions.h>
//#include <libethereum/Executive.h>*/

using namespace dev;
namespace test
{

enum class Verbosity
{
	None,
	NiceReport,
	Full
};

class Options
{
public:
	struct InvalidOption: public Exception
	{
		InvalidOption(std::string _message = std::string()): Exception(_message) {}
	};

	bool vmtrace = false;	///< Create EVM execution tracer
	bool filltests = false; ///< Create JSON test files from execution results
	bool fillchain = false; ///< Fill tests as a blockchain tests if possible
	bool stats = false;		///< Execution time and stats for state tests
	std::string statsOutFile; ///< Stats output file. "out" for standard output
	bool exectimelog = false; ///< Print execution time for each test suite
	std::string rCurrentTestSuite; ///< Remember test suite before boost overwrite (for random tests)
	bool statediff = false;///< Fill full post state in General tests
	bool fulloutput = false;///< Replace large output to just it's length
	bool createRandomTest = false; ///< Generate random test
	boost::optional<uint64_t> randomTestSeed; ///< Define a seed for random test
	bool jsontrace = false; ///< Vmtrace to stdout in json format
	//eth::StandardTrace::DebugOptions jsontraceOptions; ///< output config for jsontrace
	std::string testpath;	///< Custom test folder path
	Verbosity logVerbosity = Verbosity::NiceReport;
	boost::optional<boost::filesystem::path> randomCodeOptionsPath; ///< Options for random code generation in fuzz tests

	/// Test selection
	/// @{
	bool singleTest = false;
	boost::optional<std::string> singleTestFile;
	std::string singleTestName;
	std::string singleTestNet;
    int trDataIndex;	///< GeneralState data
    int trGasIndex;		///< GeneralState gas
    int trValueIndex;	///< GeneralState value
	bool all = false;	///< Running every test, including time consuming ones.
	bool nonetwork = false;///< For libp2p
	/// @}

	/// Get reference to options
	/// The first time used, options are parsed with argc, argv
	static Options const& get(int argc = 0, const char** argv = 0);

private:
	Options(int argc = 0, const char** argv = 0);
	Options(Options const&) = delete;
};

} //namespace test
