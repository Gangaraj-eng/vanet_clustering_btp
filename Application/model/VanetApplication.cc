
#include "VanetApplication.h"

#include "ns3/boolean.h"
#include "ns3/enum.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/ipv4-route.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/log.h"
#include "ns3/names.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/uinteger.h"

namespace ns3
{
  namespace btp
  {

    NS_LOG_COMPONENT_DEFINE("VanetApplication");
    NS_OBJECT_ENSURE_REGISTERED(VanetApplication);
    TypeId VanetApplication::GetTypeId()
    {
      static TypeId tid = TypeId("ns3::VanetApplication")
                              .SetParent<Application>()
                              .SetGroupName("Applications")
                              .AddConstructor<VanetApplication>();
      return tid;
    }

    VanetApplication::VanetApplication()
    {
    }

    VanetApplication::~VanetApplication()
    {
    }

    // application methods
    void VanetApplication::StartApplication()
    {
    }

    void VanetApplication::StopApplication()
    {
    }

    void VanetApplication::RecieveMessage(Ptr<Socket> socket)
    {
    }

    void VanetApplication::SendPacket()
    {
    }

  } // namespace btp
} // namespace ns3
