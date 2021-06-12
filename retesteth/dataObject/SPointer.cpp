#include "SPointer.h"
#include <string>

namespace dataobject
{
// To debug exceptions as breakpoints does not work from header
void throwException(std::string const& _ex)
{
    throw SPointerException(_ex);
}

}  // namespace dataobject
