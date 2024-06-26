#include "OLSRRoutingProtocol.h"

// Required header files
#include "ns3/node.h"

namespace ns3
{
  namespace btp
  {
    // Define and register the component
    NS_LOG_COMPONENT_DEFINE("OLSRRoutingProtocol");
    NS_OBJECT_ENSURE_REGISTERED(OLSRRoutingProtocol);

    OLSRRoutingProtocol::OLSRRoutingProtocol()
    {
      // NS_LOG_INFO("OLSR routing");
    }

    OLSRRoutingProtocol::~OLSRRoutingProtocol()
    {
    }

    TypeId OLSRRoutingProtocol::GetTypeId()
    {
      static TypeId tid = TypeId("ns3::btp::OLSRRoutingProtocol")
                              .SetParent<Ipv4RoutingProtocol>()
                              .SetGroupName("VanetRouting")
                              .AddConstructor<OLSRRoutingProtocol>();

      return tid;
    }

    Ptr<Ipv4Route> OLSRRoutingProtocol::RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
    {
      return Ptr<Ipv4Route>();
    }

    bool OLSRRoutingProtocol::RouteInput(Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb, MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
    {
      return false;
    }

    void OLSRRoutingProtocol::NotifyInterfaceUp(uint32_t interface)
    {
    }

    void OLSRRoutingProtocol::NotifyInterfaceDown(uint32_t interface)
    {
    }

    void OLSRRoutingProtocol::NotifyAddAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void OLSRRoutingProtocol::NotifyRemoveAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void OLSRRoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4)
    {
      m_ipv4 = ipv4;
    }

    void OLSRRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
    }

    void OLSRRoutingProtocol::DoInitialize()
    {
      m_mainAddress = m_ipv4->GetAddress(m_mainInterfaceIndex, 0).GetLocal();
      Ptr<Node> node = m_ipv4->GetObject<Node>();
      // NS_LOG_INFO("Hello from OLSR routing protocol "<<m_mainAddress);
      
    }

    void OLSRRoutingProtocol::DoDispose()
    {
    }

    void OLSRRoutingProtocol::SetMainInterface(uint32_t interfaceIndex)
    {
      m_mainInterfaceIndex = interfaceIndex;
    }

  } // namespace btp

} // namespace ns3
