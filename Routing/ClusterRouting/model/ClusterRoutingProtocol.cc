
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
#include "ns3/UADCConstants.h"

#define CR_ROUTING_MAX_SEQ_NUM 65535
#define CR_REFRESH_INTERVAL m_helloInterval
#define CR_ROUTING_NEIGHB_HOLD_TIME Time(3 * CR_REFRESH_INTERVAL)

// A random variable delay so that brodcasting won't happen at exact same
// time which leads to packet collision
#define CR_ROUTING_MAXJITTER (m_helloInterval.GetSeconds() / 4)
#define JITTER (Seconds(m_uniformRandomVariable->GetValue(0, CR_ROUTING_MAXJITTER)))

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("ClusterRoutingProtocol");
  NS_OBJECT_ENSURE_REGISTERED(ClusterRoutingProtocol);

  ClusterRoutingProtocol::ClusterRoutingProtocol()
      : m_helloTimer(Timer::CANCEL_ON_DESTROY)
  {
    m_uniformRandomVariable = CreateObject<UniformRandomVariable>();
    m_mainInterface = 1;
  }

  ClusterRoutingProtocol::~ClusterRoutingProtocol()
  {
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
    Ipv4Address dst = header.GetDestination();
    Ipv4Address origin = header.GetSource();
    NS_LOG_LOGIC(dst << " " << origin);

    // consume self packets
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
    m_helloTimer.SetFunction(&ClusterRoutingProtocol::HelloTimerExpire, this);
    m_ipv4 = ipv4;
    m_messageSequenceNumber = CR_ROUTING_MAX_SEQ_NUM;
    m_state.SetNode(ipv4->GetObject<Node>());

    m_initialClusteringTime=Seconds(3);
    
  }

  void ClusterRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
  {
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

    if (!m_recvSocket)
    {
      m_recvSocket = Socket::CreateSocket(theNode, tid);
      m_recvSocket->SetAllowBroadcast(true);
      InetSocketAddress local(Ipv4Address::GetAny(), CLUSTER_ROUTING_PORT_NUMBER);
      m_recvSocket->SetRecvCallback(MakeCallback(&ClusterRoutingProtocol::RecieveMsg, this));
      if (m_recvSocket->Bind(local))
      {
        NS_LOG_INFO("Failed to bind() Recvsocket");
      }
      m_recvSocket->SetRecvPktInfo(true);
      m_recvSocket->ShutdownSend();
    }

    if (!m_sendSocket)
    {
      m_sendSocket = Socket::CreateSocket(theNode, tid);
      m_sendSocket->SetAllowBroadcast(true);
      m_sendSocket->SetIpTtl(1);
      m_sendSocket->BindToNetDevice(m_ipv4->GetNetDevice(m_mainInterface));
      InetSocketAddress inetAddr(m_mainAddress, CLUSTER_ROUTING_PORT_NUMBER);
      if (m_sendSocket->Bind(inetAddr))
      {
        NS_LOG_INFO("Failed to bind() socket");
      }
      m_sendSocket->SetRecvPktInfo(true);
    }

    HelloTimerExpire();
  }

  void ClusterRoutingProtocol::DoDispose()
  {
    if (m_mainAddress == "10.0.0.9")
    {
      auto x = m_state.GetNeighbors();
      NS_LOG_INFO("Neibhors of 10.0.0.1 are ");
      for (auto i : x)
      {
        NS_LOG_INFO(i.neighborAddress);
      }
    }
    if (m_recvSocket)
      m_recvSocket->Close();
    if (m_sendSocket)
      m_sendSocket->Close();
    Ipv4RoutingProtocol::DoDispose();
  }

  void ClusterRoutingProtocol::SendHello()
  {
    // NS_LOG_INFO("Sending from "<<m_mainAddress<<": "<<Simulator::Now());
    Ptr<Packet> packet = Create<Packet>();
    ClusterMessageHeader msg;
    Time now = Simulator::Now();

    msg.setMessageType(ClusterMessageType::HELLO_MESSAGE);
    msg.setVTime(CR_ROUTING_NEIGHB_HOLD_TIME);
    msg.setOriginatorAddress(m_mainAddress);
    msg.setHopCount(0);
    msg.setTimeToLive(1);
    msg.setMessageSequenceNumber(GetMessageSequenceNumber());
    msg.SetClusterId(m_state.GetClusterId());
    msg.setNodeId(m_state.GetNodeId());

    ClusterMessageHeader::Hello hello;
    hello.nodePosition=m_state.GetNodePosition();
    hello.nodeVelocity=m_state.GetNodeVelocity();
    msg.SetHello(hello);

    packet->AddHeader(msg);
    Ipv4Address destination = m_ipv4->GetAddress(m_mainInterface, 0).GetLocal().GetSubnetDirectedBroadcast(m_ipv4->GetAddress(m_mainInterface, 0).GetMask());
    // int sz=m_sendSocket->SendTo(packet, 0, InetSocketAddress(destination, CLUSTER_ROUTING_PORT_NUMBER));
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
    uint32_t recvInterfaceIndex = m_ipv4->GetInterfaceForDevice(dev);

    InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom(sourceAddress);
    Ipv4Address senderIfaceAddr = inetSourceAddr.GetIpv4();
    Ipv4Address receiverIfaceAddr = m_ipv4->GetAddress(recvInterfaceIndex, 0).GetLocal();
    NS_LOG_LOGIC(senderIfaceAddr << " " << receiverIfaceAddr);
    ClusterMessageHeader recievedHeader;
    receivedPacket->RemoveHeader(recievedHeader);
    ClusterMessageType recievedMessageType = recievedHeader.GetMessageType();
    switch (recievedMessageType)
    {
    case ClusterMessageType::HELLO_MESSAGE:
      ProcessHello(recievedHeader, receiverIfaceAddr, senderIfaceAddr);
      break;

    default:
      break;
    }
  }

  void ClusterRoutingProtocol::HelloTimerExpire()
  {
    SendHello();
    if (not m_helloTimer.IsRunning())
    {
      m_helloTimer.Schedule(m_helloInterval + JITTER);
    }
  }

  void ClusterRoutingProtocol::NeighborTimerExpire(Ipv4Address neighborIfaceAddress)
  {
    Time now = Simulator::Now();

    NeighborEntry *entry = m_state.FindNeighborEntry(neighborIfaceAddress);
    if (entry == nullptr)
    {
      return;
    }
    if (entry->time < now)
    {
      m_state.EraseNeighborEntry(neighborIfaceAddress);
    }
    else
    {
      m_events.Track(Simulator::Schedule(DELAY(entry->time), &ClusterRoutingProtocol::NeighborTimerExpire, this, neighborIfaceAddress));
    }
  }

  void ClusterRoutingProtocol::ProcessHello(ClusterMessageHeader helloMessage, Ipv4Address recievedIfaceAddress, Ipv4Address senderIfaceAddress)
  {
    NeighborEntry *n_entry = m_state.FindNeighborEntry(senderIfaceAddress);

    if (n_entry != nullptr)
    {
      return;
      // Neighbor already exists
    }
    else
    {
      n_entry = new NeighborEntry();
      n_entry->neighborAddress = senderIfaceAddress;
      n_entry->time = Simulator::Now() + helloMessage.GetVTime();
      m_state.AddNeighbor(*n_entry);
      m_events.Track(Simulator::Schedule(DELAY(n_entry->time),
                                         &ClusterRoutingProtocol::NeighborTimerExpire, this, n_entry->neighborAddress));
    }
  }

  bool ClusterRoutingProtocol::IsMyOwnAddress(Ipv4Address addr)
  {
    return addr == m_ipv4->GetAddress(m_mainInterface, 0).GetLocal();
  }

  void ClusterRoutingProtocol::SetMainInterface(uint32_t interface)
  {
    m_mainInterface = interface;
    m_mainAddress = m_ipv4->GetAddress(interface, 0).GetLocal();
  }

  uint16_t ClusterRoutingProtocol::GetMessageSequenceNumber()
  {
    m_messageSequenceNumber = (m_messageSequenceNumber + 1) % CR_ROUTING_MAX_SEQ_NUM;
    return m_messageSequenceNumber;
  }
} // namespace ns3