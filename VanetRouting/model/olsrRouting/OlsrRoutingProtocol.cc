#include "OlsrRoutingProtocol.h"

// Required header files
#include "ns3/node.h"

// add node id at beginning
#define NS_LOG_APPEND_CONTEXT                                       \
  if (GetObject<Node>())                                            \
  {                                                                 \
    std::clog << "[node " << GetObject<Node>()->GetId() << ":OR] "; \
  }

namespace ns3
{
  namespace btp
  {

    // Define and register the component
    NS_LOG_COMPONENT_DEFINE("OlsrRoutingProtocol");
    NS_OBJECT_ENSURE_REGISTERED(OlsrRoutingProtocol);

    OlsrRoutingProtocol::OlsrRoutingProtocol()
    {
      // NS_LOG_INFO("OLSR routing");
    }

    OlsrRoutingProtocol::~OlsrRoutingProtocol()
    {
    }

    TypeId OlsrRoutingProtocol::GetTypeId()
    {
      static TypeId tid = TypeId("ns3::btp::OlsrRoutingProtocol")
                              .SetParent<Ipv4RoutingProtocol>()
                              .SetGroupName("VanetRouting")
                              .AddConstructor<OlsrRoutingProtocol>();

      return tid;
    }

    Ptr<Ipv4Route> OlsrRoutingProtocol::RouteOutput(Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
    {
      return Ptr<Ipv4Route>();
    }

    bool OlsrRoutingProtocol::RouteInput(Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev, UnicastForwardCallback ucb, MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
    {
      return false;
    }

    void OlsrRoutingProtocol::NotifyInterfaceUp(uint32_t interface)
    {
    }

    void OlsrRoutingProtocol::NotifyInterfaceDown(uint32_t interface)
    {
    }

    void OlsrRoutingProtocol::NotifyAddAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void OlsrRoutingProtocol::NotifyRemoveAddress(uint32_t, Ipv4InterfaceAddress address)
    {
    }

    void OlsrRoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4)
    {
    }

    void OlsrRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
    }

    void OlsrRoutingProtocol::DoInitialize()
    {
      NS_LOG_INFO("Hello from OLSR routing protocol");
    }

    void OlsrRoutingProtocol::DoDispose()
    {
    }

  } // namespace btp

} // namespace ns3
