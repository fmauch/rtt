#include <rtt/Service.hpp>
#include <rtt/plugin/ServicePlugin.hpp>

using namespace RTT;

class ServicePluginTest : public Service
{
public:
    ServicePluginTest(TaskContext* tc) : Service("the_service", tc) {}
};

ORO_SERVICE_NAMED_PLUGIN( ServicePluginTest, "serviceplugin")