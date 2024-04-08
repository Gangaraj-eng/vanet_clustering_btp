
#include "ClusterRoutingProtocol.h"

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("ClusterRoutingProtocol");
  NS_OBJECT_ENSURE_REGISTERED(ClusterRoutingProtocol);
  ClusterRoutingProtocol::ClusterRoutingProtocol()
  {
    NS_LOG_INFO("Created Cluster Routing Protocol Object");
  }

  TypeId ClusterRoutingProtocol::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::ClusterRoutingProtocol")
                            .SetParent<Ipv4RoutingProtocol>()
                            .SetGroupName("ClusterRouting")
                            .AddConstructor<ClusterRoutingProtocol>(); // object will be creatd only if constructor is added
    return tid;
  }

  ClusterRoutingProtocol::~ClusterRoutingProtocol()
  {
  }

  Ptr<Ipv4Route>
  ClusterRoutingProtocol::RouteOutput(Ptr<Packet> p,
                                      const Ipv4Header &header,
                                      Ptr<NetDevice> oif,
                                      Socket::SocketErrno &sockerr)
  {
    Ptr<Ipv4Route> route;
    return route;
  }

  bool ClusterRoutingProtocol::RouteInput(Ptr<const Packet> p,
                                          const Ipv4Header &header,
                                          Ptr<const NetDevice> idev,
                                          UnicastForwardCallback ucb,
                                          MulticastForwardCallback mcb,
                                          LocalDeliverCallback lcb,
                                          ErrorCallback ecb)
  {
    return true;
  }

  void ClusterRoutingProtocol::NotifyInterfaceUp(uint32_t interface)
  {
  }
  void ClusterRoutingProtocol::NotifyInterfaceDown(uint32_t interface)
  {
  }
  void ClusterRoutingProtocol::NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address)
  {
  }
  void ClusterRoutingProtocol::NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address)
  {
  }
  void ClusterRoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4)
  {
  }
  void ClusterRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
  {
  }
} // namespace ns3
