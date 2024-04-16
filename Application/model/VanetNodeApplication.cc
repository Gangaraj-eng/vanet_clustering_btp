
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
  }

  VanetNodeApplication::~VanetNodeApplication()
  {
  }

  void VanetNodeApplication::sendPacket(){
       Ptr<Packet> packet=Create<Packet>(32);
            
  }

  // application methods
  void VanetNodeApplication::StartApplication()
  {
      // Ipv4Address laddr = GetNode()->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();
      // if(laddr=="10.0.0.1"){
      //   m_sendSocket=Socket::CreateSocket(GetNode(),UdpSocketFactory::GetTypeId());
      //   m_sendSocket->Bind(InetSocketAddress("10.0.0.1",12345));
      //   m_sendSocket->SetAllowBroadcast(true);
      //   Simulator::Schedule(Seconds(2),&VanetNodeApplication::sendPacket,this);
      // }
      // else{
      //   m_recSocket = Socket::CreateSocket(GetNode(),UdpSocketFactory::GetTypeId());
      //   m_recSocket->Bind(InetSocketAddress("10.0.0.2",12345));
      //   m_recSocket->SetAllowBroadcast(true);
      //   m_recSocket->SetRecvCallback(MakeCallback(&VanetNodeApplication::RecieveMessage,this));
      // }
  }

  void VanetNodeApplication::RecieveMessage(Ptr<Socket> socket)
  {
    // NS_LOG_INFO("Hello");
  }

  void VanetNodeApplication::StopApplication()
  {
  }
} // namespace ns3
