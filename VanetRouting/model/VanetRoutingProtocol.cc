#include "VanetRoutingProtocol.h"

/**
 * This contains the implementation of the combined routing protocol
 */

// Required header files
#include "ns3/CustomNode.h"
#include "VanetRoutingRepository.h"

// add node id at beginning
#define NS_LOG_APPEND_CONTEXT                                       \
  if (GetObject<Node>())                                            \
  {                                                                 \
    std::clog << "[node " << GetObject<Node>()->GetId() << ":VR] "; \
  }

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
      m_ipv4 = ipv4;

      m_OlsrRoutingProtocol.SetIpv4(ipv4);
      m_QueryRoutingProtocol.SetIpv4(ipv4);
      m_ClusterRoutingProtocol.SetIpv4(ipv4);
    }
    void VanetRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
    {
    }

    void VanetRoutingProtocol::DoInitialize()
    {
      Ptr<Node> node = GetObject<Node>();
      NodeType currentNodeType = DynamicCast<CustomNode>(node)->mnodeType;
     

      if (currentNodeType == NodeType::CLUSTER_HEAD)
      {
        m_ClusterRoutingProtocol.SetMainInterface(1);
        m_ClusterRoutingProtocol.Initialize();

        m_QueryRoutingProtocol.SetMainInterface(2);
        m_QueryRoutingProtocol.Initialize();

        m_OlsrRoutingProtocol.SetMainInterface(3);
        m_OlsrRoutingProtocol.Initialize();
      }
      else if (currentNodeType == NodeType::UAV)
      {
       
        m_OlsrRoutingProtocol.SetMainInterface(1);
        m_OlsrRoutingProtocol.Initialize();
      }
      else if (currentNodeType == NodeType::RSU)
      {
        m_QueryRoutingProtocol.SetMainInterface(1);
        m_QueryRoutingProtocol.Initialize();
      }
      // NS_LOG_INFO("Started Vanet Routing Protocol");
    }

    void VanetRoutingProtocol::DoDispose()
    {
    }

  } // namespace btp

} // namespace ns3
