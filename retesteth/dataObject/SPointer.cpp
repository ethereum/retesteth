#include "SPointer.h"
#include <string>
#include <mutex>


namespace dataobject
{

// To debug exceptions as breakpoints does not work from header
void throwException(std::string const& _ex)
{
    throw SPointerException(_ex);
}

std::mutex g_spMutexAdd;
std::mutex g_spMutexDel;


void GCP_SPointerBase::AddRef()
{
    std::lock_guard<std::mutex> lock(g_spMutexAdd);
    _nRef++;
}
int GCP_SPointerBase::DelRef()
{
    std::lock_guard<std::mutex> lock(g_spMutexDel);
    _nRef--;
    return _nRef;
}

}  // namespace dataobject
