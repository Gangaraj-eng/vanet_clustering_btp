
#include "ClusterRoutingProtocol.h"

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

#define CR_ROUTING_MAX_SEQ_NUM 65535

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("ClusterRoutingProtocol");
  NS_OBJECT_ENSURE_REGISTERED(ClusterRoutingProtocol);

  const uint16_t ClusterRoutingProtocol::CLUSTER_ROUTING_PORT_NUMBER = 600;

  ClusterRoutingProtocol::ClusterRoutingProtocol()
      : m_helloTimer(Timer::CANCEL_ON_DESTROY)
  {
    m_mainInterface = 1;
  }

  TypeId ClusterRoutingProtocol::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::ClusterRoutingProtocol")
                            .SetParent<Ipv4RoutingProtocol>()
                            .SetGroupName("ClusterRouting")
                            .AddConstructor<ClusterRoutingProtocol>()
                            .AddAttribute("HelloInterval",
                                          "Hello messages emission interval",
                                          TimeValue(Seconds(2)),
                                          MakeTimeAccessor(&ClusterRoutingProtocol::m_helloInterval),
                                          MakeTimeChecker());
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
    Ptr<Ipv4Route> rtentry;
    return rtentry;
  }

  bool ClusterRoutingProtocol::RouteInput(Ptr<const Packet> p,
                                          const Ipv4Header &header,
                                          Ptr<const NetDevice> idev,
                                          UnicastForwardCallback ucb,
                                          MulticastForwardCallback mcb,
                                          LocalDeliverCallback lcb,
                                          ErrorCallback ecb)
  {

    NS_LOG_INFO(header.GetSource() << " " << m_mainAddress << " " << p->GetSize());
    Ipv4Address dst = header.GetDestination();
    Ipv4Address origin = header.GetSource();
    if (IsMyOwnAddress(origin) == true)
    {
      return true;
    }
    NS_ASSERT(m_ipv4->GetInterfaceForDevice(idev) >= 0);
    uint32_t iif = m_ipv4->GetInterfaceForDevice(idev);
    if (m_ipv4->IsDestinationAddress(dst, iif))
    {
      if (!lcb.IsNull())
      {
        lcb(p, header, iif);
        return true;
      }
      else
      {
        return false;
      }
    }
    return false;
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
    m_helloTimer.SetFunction(&ClusterRoutingProtocol::HelloTimerExpire, this);
    m_ipv4 = ipv4;
  }
  void ClusterRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
  {
  }

  void ClusterRoutingProtocol::SendHello()
  {
    Ptr<Packet> packet = Create<Packet>();
    Ipv4Address destination = Ipv4Address("10.255.255.255");
    m_sendSocket->SendTo(packet, 0, InetSocketAddress(destination, CLUSTER_ROUTING_PORT_NUMBER));
  }

  void ClusterRoutingProtocol::RecieveMsg(Ptr<Socket> socket)
  {
    Ipv4PacketInfoTag interfaceInfo;
    Ptr<Packet> receivedPacket;
    Address sourceAddress;
    receivedPacket = socket->RecvFrom(sourceAddress);
    if (!receivedPacket->RemovePacketTag(interfaceInfo))
    {
      NS_ABORT_MSG("No incoming interface on OLSR message, aborting.");
    }
    uint32_t incomingIf = interfaceInfo.GetRecvIf();
    Ptr<Node> node = this->GetObject<Node>();
    Ptr<NetDevice> dev = node->GetDevice(incomingIf);
    // uint32_t recvInterfaceIndex = m_ipv4->GetInterfaceForDevice(dev);

    InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom(sourceAddress);
    Ipv4Address senderIfaceAddr = inetSourceAddr.GetIpv4();
    NS_LOG_INFO("Packet recieved on " << m_mainAddress << " from " << senderIfaceAddr);
  }

  void ClusterRoutingProtocol::HelloTimerExpire()
  {
    SendHello();
    m_helloTimer.Schedule(m_helloInterval);
  }

  void ClusterRoutingProtocol::DoInitialize()
  {
    // NS_LOG_INFO(m_ipv4->GetAddress(1,0));
    if (m_mainAddress == Ipv4Address())
    {
      m_mainAddress = m_ipv4->GetAddress(m_mainInterface, 0).GetLocal();
    }
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    Ptr<Node> theNode = GetObject<Node>();

    if (!m_sendSocket)
    {
      m_sendSocket = Socket::CreateSocket(theNode, tid);
      InetSocketAddress inetAddr(m_mainAddress, CLUSTER_ROUTING_PORT_NUMBER);
      m_sendSocket->SetAllowBroadcast(true);
      if (m_sendSocket->Bind(inetAddr))
      {
        NS_LOG_INFO("Failed to bind() MASP socket");
      }
      m_sendSocket->BindToNetDevice(m_ipv4->GetNetDevice(m_mainInterface));
      m_sendSocket->SetRecvPktInfo(true);
    }

    if (!m_recvSocket)
    {
      m_recvSocket = Socket::CreateSocket(theNode, tid);
      InetSocketAddress local(m_ipv4->GetAddress(1, 0).GetBroadcast(), CLUSTER_ROUTING_PORT_NUMBER);
      m_recvSocket->SetAllowBroadcast(true);
      if (m_recvSocket->Bind(local))
      {
        NS_LOG_INFO("Failed to bind() MASP Recvsocket");
      }
      m_recvSocket->SetRecvCallback(MakeCallback(&ClusterRoutingProtocol::RecieveMsg, this));
      m_recvSocket->SetRecvPktInfo(true);
      m_recvSocket->ShutdownSend();
    }

    HelloTimerExpire();
  }

  bool ClusterRoutingProtocol::IsMyOwnAddress(Ipv4Address addr)
  {
    return addr == m_ipv4->GetAddress(m_mainInterface, 0).GetLocal();
  }

  void ClusterRoutingProtocol::DoDispose()
  {
    if (m_recvSocket)
      m_recvSocket->Close();
    if (m_sendSocket)
      m_sendSocket->Close();
  }

  void ClusterRoutingProtocol::SetMainInterface(uint32_t interface)
  {
    m_mainAddress = m_ipv4->GetAddress(interface, 0).GetLocal();
  }

} // namespace ns3
