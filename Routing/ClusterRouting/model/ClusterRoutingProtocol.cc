
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
    m_helloTimer.SetFunction(&ClusterRoutingProtocol::HelloTimerExpire, this);
    m_ipv4 = ipv4;
  }
  void ClusterRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
  {
  }

    void ClusterRoutingProtocol::SendHello()
    {
      Ptr<Packet> pkt = Create<Packet>(32);
       int i=m_sendSocket->SendTo(pkt,0,InetSocketAddress("10.0.0.2",12345));
          if(i!=-1){
        NS_LOG_INFO("Sent some packets successfully");
       }
    }

    void ClusterRoutingProtocol::RecieveMsg(Ptr<Socket> socket)
    {
      NS_LOG_INFO("Recieved some packet");
    }

    void ClusterRoutingProtocol::HelloTimerExpire()
    {
      SendHello();
      // m_helloTimer.Schedule(m_helloInterval);
    }

    void ClusterRoutingProtocol::DoInitialize()
    {
      Ipv4Address laddr = m_ipv4->GetAddress(1,0).GetLocal();
      if(laddr=="10.0.0.1"){
        m_sendSocket=Socket::CreateSocket(GetObject<Node>(),UdpSocketFactory::GetTypeId());
        m_sendSocket->Bind(InetSocketAddress("10.0.0.1",12345));
        m_sendSocket->SetAllowBroadcast(true);
        Simulator::Schedule(Seconds(2),&ClusterRoutingProtocol::SendHello,this);
      }
      else{
        m_recvSocket = Socket::CreateSocket(GetObject<Node>(),UdpSocketFactory::GetTypeId());
        m_recvSocket->Bind(InetSocketAddress("10.0.0.2",12345));
        m_recvSocket->SetAllowBroadcast(true);
        m_recvSocket->SetRecvCallback(MakeCallback(&ClusterRoutingProtocol::RecieveMsg,this));
      }
    }

  void ClusterRoutingProtocol::DoDispose()
  {
  }

} // namespace ns3
