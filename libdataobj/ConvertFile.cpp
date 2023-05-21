#include "ConvertFile.h"
#include "JsonParser.h"

using namespace std;
namespace dataobject
{

// Manually construct dataobject from file string content
// Faster and less memory consuming algo, but not as perfect as full json parser
// bacuse Json::Reader::parse has a memory leak and eat too much memory on big files
// TODO still requires to load the whole file into _input, luckly we don't have too big tests

/// Convert Json object represented as string to DataObject
spDataObject ConvertJsoncppStringToData(string const& _input, CJOptions const& _opt)
{
    JsonParser parser(_input, _opt);
    parser.parse();
    return parser.root();
}

}
