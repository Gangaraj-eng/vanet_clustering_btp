
#include "VanetNodeApplication.h"

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
#include "ns3/uinteger.h"
#include "ns3/UADCConstants.h"

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("VanetNodeApplication");
  NS_OBJECT_ENSURE_REGISTERED(VanetNodeApplication);
  TypeId VanetNodeApplication::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::VanetNodeApplication")
                            .SetParent<Application>()
                            .SetGroupName("Applications")
                            .AddConstructor<VanetNodeApplication>();
    return tid;
  }

  VanetNodeApplication::VanetNodeApplication()
  {
    this->m_node = DynamicCast<VanetNode>(GetNode());
  }

  VanetNodeApplication::~VanetNodeApplication()
  {
  }

  // application methods
  void VanetNodeApplication::StartApplication()
  {
    TypeId tid = UdpSocketFactory::GetTypeId();
    m_recvSocket = Socket::CreateSocket(GetNode(), tid);

    // setup recieve socket
    InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), 400);
    if (m_recvSocket->Bind(local) == -1)
    {
      NS_FATAL_ERROR("Failed to bind recieve socket");
    }
    m_recvSocket->SetRecvCallback(MakeCallback(&VanetNodeApplication::RecieveMessage, this));

    m_sendSocket = Socket::CreateSocket(GetNode(), tid);

    Simulator::Schedule(Seconds(8), &VanetNodeApplication::SendPacket, this);
  }

  void VanetNodeApplication::StopApplication()
  {
  }

  void VanetNodeApplication::RecieveMessage(Ptr<Socket> socket)
  {
    Ptr<Ipv4> ipv4 = GetNode()->GetObject<Ipv4>();
    Ipv4Address address = ipv4->GetAddress(1, 0).GetLocal();
    NS_LOG_INFO("Recive message of application layer called " << address);
    // NS_LOG_FUNCTION(this);
    // Ptr<Packet> packet;
    // Address sourceAddr;

    // // In application layer, all the headers are stripped off
    // // only have the payload
    // // use packet tags to attach some useful information
    // while ((packet = socket->RecvFrom(sourceAddr)))
    // {
    //   NS_LOG_INFO(packet->ToString());
    // }
  }

  void VanetNodeApplication::SendPacket()
  {
    NS_LOG_FUNCTION(this);

    // connect send socket to destination
    // if (IsNodeClusterHead())
    // {
    // }
    // else
    // {

    Ipv4Address destination = "10.0.0.1";
    int port = 400;
    Ptr<Packet> packet = Create<Packet>(20);
    Ptr<Ipv4> ipv4 = GetNode()->GetObject<Ipv4>();
    Ipv4Address address = ipv4->GetAddress(1, 0).GetLocal();
    // NS_LOG_INFO(address << " from Application layer");
    if (address == "10.0.0.9")
    {
      m_sendSocket->Connect(InetSocketAddress(destination, port));
      m_sendSocket->Send(packet);
    }
    // }
  }

  bool VanetNodeApplication::IsNodeClusterHead()
  {
    return m_node->GetNodeId() == m_node->GetClusterId();
  }
} // namespace ns3
