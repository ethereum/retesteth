#include "SPointer.h"
#include <string>
#include <mutex>
namespace dataobject
{

bool G_IS_THREADSAFE = true;
std::mutex GCP_SPointerBase::g_spRefAccessMutex;

void disableThreadsafe()
{
    G_IS_THREADSAFE = false;
}

// To debug exceptions as breakpoints does not work from header
void throwException(std::string const& _ex)
{
    throw SPointerException(_ex);
}

void GCP_SPointerBase::AddRef()
{
    // very heavy. use thread unsafe pointer preferably
    if (G_IS_THREADSAFE)
    {
        std::lock_guard<std::mutex> lock(g_spRefAccessMutex);
        _nRef++;
    }
    else
        _nRef++;
}

int GCP_SPointerBase::DelRef()
{
    if (G_IS_THREADSAFE)
    {
        std::lock_guard<std::mutex> lock(g_spRefAccessMutex);
        _nRef--;
        return _nRef;
    }
    else
        _nRef--;
    return _nRef;
}

int GCP_SPointerBase::GetRef()
{
    if (G_IS_THREADSAFE)
    {
        std::lock_guard<std::mutex> lock(g_spRefAccessMutex);
        return _nRef;
    }
    return _nRef;
}

}  // namespace dataobject
