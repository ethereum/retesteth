#include <mutex>
#include <retesteth/ClientConfig.h>
std::mutex g_staticDeclaration_clientConfigID;
namespace test
{

ClientConfigID::ClientConfigID()
{
        std::lock_guard<std::mutex> lock(g_staticDeclaration_clientConfigID);
        static unsigned uniqueID = 0;
        uniqueID++;
        m_id = uniqueID;
}

}
