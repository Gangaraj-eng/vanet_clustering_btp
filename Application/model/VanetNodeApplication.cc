
#include "VanetNodeApplication.h"

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("VanetNodeApplication");
  NS_OBJECT_ENSURE_REGISTERED(VanetNodeApplication);
  TypeId VanetNodeApplication::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::VanetNodeApplication")
                            .SetParent<Application>()
                            .SetGroupName("Applications")
                            .AddConstructor<VanetNodeApplication>();
    return tid;
  }

  VanetNodeApplication::VanetNodeApplication()
  {
  }

  VanetNodeApplication::~VanetNodeApplication()
  {
  }

  // application methods
  void VanetNodeApplication::StartApplication()
  {
    NS_LOG_INFO("Hey, Application Started!!\n");
  }

  void VanetNodeApplication::StopApplication()
  {
    NS_LOG_INFO("Stopping Application\\\n");
  }
} // namespace ns3
