#include "VanetRoutingProtocol.h"

/**
 * This contains the implementation of the combined routing protocol
 */

// Required header files

namespace ns3
{
  namespace btp
  {

    // Define and register the component
    NS_LOG_COMPONENT_DEFINE("VanetRoutingProtocol");
    NS_OBJECT_ENSURE_REGISTERED(VanetRoutingProtocol);

    VanetRoutingProtocol::VanetRoutingProtocol()
    {
    }

    VanetRoutingProtocol::~VanetRoutingProtocol()
    {
    }

    TypeId VanetRoutingProtocol::GetTypeId()
    {
      static TypeId tid = TypeId("ns3::btp::VanetRoutingProtocol")
                              .SetParent<Ipv4RoutingProtocol>()
                              .SetGroupName("VanetRouting")
                              .AddConstructor<VanetRoutingProtocol>();
      return tid;
    }

    //  parent class methods
    Ptr<Ipv4Route>
    VanetRoutingProtocol::RouteOutput(Ptr<Packet> p,
                                      const Ipv4Header &header,
                                      Ptr<NetDevice> oif,
                                      Socket::SocketErrno &sockerr)
    {
      Ptr<Ipv4Route> route;
      return route;
    }

    bool VanetRoutingProtocol::RouteInput(Ptr<const Packet> p,
                                          const Ipv4Header &header,
                                          Ptr<const NetDevice> idev,
                                          UnicastForwardCallback ucb,
                                          MulticastForwardCallback mcb,
                                          LocalDeliverCallback lcb,
                                          ErrorCallback ecb)
    {
      return true;
    }

    void VanetRoutingProtocol::NotifyInterfaceUp(uint32_t interface)
    {
    }
    void VanetRoutingProtocol::NotifyInterfaceDown(uint32_t interface)
    {
    }
    void VanetRoutingProtocol::NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address)
    {
    }
    void VanetRoutingProtocol::NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address)
    {
    }
    void VanetRoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4)
    {
    }
    void VanetRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
    }

    

  } // namespace btp

} // namespace ns3
