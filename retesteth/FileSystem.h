#pragma once
#include <boost/filesystem.hpp>

namespace test {
namespace fsmacro {

// https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros
// Make a FOREACH macro
#define FE_0(WHAT)
#define FE_1(WHAT, X) WHAT(X)
#define FE_2(WHAT, X, ...) WHAT(X)FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X)FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X)FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X)FE_4(WHAT, __VA_ARGS__)
//... repeat as needed

#define GET_MACRO(_0,_1,_2,_3,_4,_5,NAME,...) NAME
#define FOR_EACH(action,...) \
  GET_MACRO(_0,__VA_ARGS__,FE_5,FE_4,FE_3,FE_2,FE_1,FE_0)(action,__VA_ARGS__)

#define OPENFILE(_path) boost::filesystem::ofstream file(_path);
#define PUTTOFILE(_arg) file << _arg;
#define CLOSEFILE() \
    file.close();   \
    }
}

#define WRITEFILE(_path, ...)            \
    {                                    \
        OPENFILE(_path)                  \
        FOR_EACH(PUTTOFILE, __VA_ARGS__) \
        CLOSEFILE()

#define WRITEFILEN(_path, ...) \
    {                          \
        OPENFILE(_path)        \
        FOR_EACH(PUTTOFILE, __VA_ARGS__)

#define ADDTOFILE(_arg) \
    PUTTOFILE(_arg)

}
