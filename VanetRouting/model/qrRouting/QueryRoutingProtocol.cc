#include "QueryRoutingProtocol.h"

/**
 * This contains the implementation of query routing protocol
 */

// Required header files
#include "ns3/node.h"


// add node id at beginning
#define NS_LOG_APPEND_CONTEXT                                       \
  if (GetObject<Node>())                                            \
  {                                                                 \
    std::clog << "[node " << GetObject<Node>()->GetId() << ":QR] "; \
  }

namespace ns3
{
  namespace btp
  {

    // Define and register the component
    NS_LOG_COMPONENT_DEFINE("QueryRoutingProtocol");
    NS_OBJECT_ENSURE_REGISTERED(QueryRoutingProtocol);

    QueryRoutingProtocol::QueryRoutingProtocol()
    {
    }

    QueryRoutingProtocol::~QueryRoutingProtocol()
    {
    }

    TypeId QueryRoutingProtocol::GetTypeId()
    {
      static TypeId tid = TypeId("ns3::btp::QueryRoutingProtocol")
                              .SetParent<Ipv4RoutingProtocol>()
                              .SetGroupName("VanetRouting")
                              .AddConstructor<QueryRoutingProtocol>();
      return tid;
    }

    Ptr<Ipv4Route> QueryRoutingProtocol::RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
    {
      return Ptr<Ipv4Route>();
    }

    bool QueryRoutingProtocol::RouteInput(Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb, MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
    {
      return false;
    }

    void QueryRoutingProtocol::NotifyInterfaceUp(uint32_t interface)
    {
    }

    void QueryRoutingProtocol::NotifyInterfaceDown(uint32_t interface)
    {
    }

    void QueryRoutingProtocol::NotifyAddAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void QueryRoutingProtocol::NotifyRemoveAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void QueryRoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4)
    {
    }

    void QueryRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
    }

    void QueryRoutingProtocol::DoInitialize()
    {
      NS_LOG_INFO("Hello from Query routing Protocol");
    }

    void QueryRoutingProtocol::DoDispose()
    {
    }

    // Define and register the component
  } // namespace btp

} // namespace ns3
