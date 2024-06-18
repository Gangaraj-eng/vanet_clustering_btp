#include "ClusterRoutingProtocol.h"

// Required header files
#include "ns3/node.h"


// add node id at beginning
#define NS_LOG_APPEND_CONTEXT                                       \
  if (GetObject<Node>())                                            \
  {                                                                 \
    std::clog << "[node " << GetObject<Node>()->GetId() << ":CR] "; \
  }

namespace ns3
{
  namespace btp
  {
    // Define and register the component 
    NS_LOG_COMPONENT_DEFINE("ClusterRoutingProtocol");
    NS_OBJECT_ENSURE_REGISTERED(ClusterRoutingProtocol);

    ClusterRoutingProtocol::ClusterRoutingProtocol()
    {
      // NS_LOG_INFO("Clustering routing");
    }

    ClusterRoutingProtocol::~ClusterRoutingProtocol()
    {
    }

    TypeId ClusterRoutingProtocol::GetTypeId()
    {
      static TypeId tid = TypeId("ns3::btp::ClusterRoutingProtocol")
                              .SetParent<Ipv4RoutingProtocol>()
                              .SetGroupName("VanetRouting")
                              .AddConstructor<ClusterRoutingProtocol>();
      return tid;
    }

    Ptr<Ipv4Route> ClusterRoutingProtocol::RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
    {
      return Ptr<Ipv4Route>();
    }

    bool ClusterRoutingProtocol::RouteInput(Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb, MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
    {
      return false;
    }

    void ClusterRoutingProtocol::NotifyInterfaceUp(uint32_t interface)
    {
    }

    void ClusterRoutingProtocol::NotifyInterfaceDown(uint32_t interface)
    {
    }

    void ClusterRoutingProtocol::NotifyAddAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void ClusterRoutingProtocol::NotifyRemoveAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void ClusterRoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4)
    {
    }

    void ClusterRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
    }

    void ClusterRoutingProtocol::DoInitialize()
    {
      NS_LOG_INFO("Hello from Clsuter routing protocol");
    }

    void ClusterRoutingProtocol::DoDispose()
    {
    }

  } // namespace btp

} // namespace ns3
