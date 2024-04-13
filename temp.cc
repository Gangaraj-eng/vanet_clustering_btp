/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "masp-routing-protocol.h"
#include "ns3/log.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-route.h"
#include "ns3/string.h"
#include "ns3/pointer.h"



#define MASP_PORT_NUMBER 698


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MaspRoutingProtocol");

namespace masp {


NS_OBJECT_ENSURE_REGISTERED (MaspRoutingProtocol);

TypeId
MaspRoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::masp::MaspRoutingProtocol")
    .SetParent<Ipv4RoutingProtocol> ()
    .AddConstructor<MaspRoutingProtocol> ()
	.AddAttribute ("Bcst1Interval", "Broadcast1 messages emission interval.",
	                   TimeValue (Seconds (2)),
	                   MakeTimeAccessor (&MaspRoutingProtocol::m_bcst1Interval),
	                   MakeTimeChecker ());
  return tid;
}



MaspRoutingProtocol::MaspRoutingProtocol ():
  m_sinkFlag(false),
  m_bcst1RateLimitTimer (Timer::CANCEL_ON_DESTROY),
  m_bcst1Count(0),
  bcst1RateLimit(10)

{

}

MaspRoutingProtocol::~MaspRoutingProtocol ()
{
  NS_LOG_FUNCTION_NOARGS ();
}



Ptr<Ipv4Route>
MaspRoutingProtocol::RouteOutput (Ptr<Packet> p, const Ipv4Header &header,
                              Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{


	  NS_LOG_DEBUG ("Entering RouteOutput");

	 Ptr<Ipv4Route> route = Create<Ipv4Route> ();
	 //Not Implemented yet
	return route;
}

bool
MaspRoutingProtocol::RouteInput (Ptr<const Packet> p, const Ipv4Header &header,
                             Ptr<const NetDevice> idev, UnicastForwardCallback ucb,
                             MulticastForwardCallback mcb, LocalDeliverCallback lcb, ErrorCallback ecb)
{
	//Not Implemented yet

	  NS_LOG_DEBUG ("Entering RouteInput");

	return false;
}

void
MaspRoutingProtocol::NotifyInterfaceUp (uint32_t i)
{
	//Not Implemented yet
}

void
MaspRoutingProtocol::NotifyInterfaceDown (uint32_t i)
{
	//Not Implemented yet
}


void
MaspRoutingProtocol::NotifyAddAddress (uint32_t i, Ipv4InterfaceAddress address)
{

}

void
MaspRoutingProtocol::NotifyRemoveAddress (uint32_t i, Ipv4InterfaceAddress address)
{
	//Not Implemented yet
}


void
MaspRoutingProtocol::SetIpv4 (Ptr<Ipv4> ipv4)
{

	m_ipv4 = ipv4;

	Simulator::ScheduleNow (&MaspRoutingProtocol::StartTimers, this);

}

void
MaspRoutingProtocol::PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const
{
	//Not Implemented yet
}

void
MaspRoutingProtocol::DoInitialize ()
{

  if (m_mainAddress == Ipv4Address ())
    {
	      Ipv4Address loopback ("127.0.0.1");
	      for (uint32_t i = 0; i < m_ipv4->GetNInterfaces (); i++)
	        {
	          // Use primary address, if multiple
	          Ipv4Address addr = m_ipv4->GetAddress (i, 0).GetLocal ();
	          if (addr != loopback)
	            {
	              m_mainAddress = addr;

	              // Create a socket to listen only on this interface
	              TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
	              Ptr<Node> theNode = GetObject<Node> ();
	              Ptr<Socket> socket = Socket::CreateSocket (theNode,tid);
	              InetSocketAddress inetAddr (m_ipv4->GetAddress (i, 0).GetLocal (), MASP_PORT_NUMBER);
	              if (socket->Bind (inetAddr))
	                {
	              	    NS_FATAL_ERROR ("Failed to bind() MASP socket");
	              	}
	              socket->BindToNetDevice (m_ipv4->GetNetDevice (i));
	              socket->GetAllowBroadcast();
	              m_socketAddresses[socket] = m_ipv4->GetAddress (i, 0);

	            //  NS_LOG_DEBUG ("Socket Binding on ip " << m_mainAddress << " interface " << i);

	              break;
	            }
	        }

	      NS_ASSERT (m_mainAddress != Ipv4Address ());
    }

  if (!m_recvSocket)
     {
       NS_LOG_LOGIC ("Masp: adding receiving socket");
       TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
       Ptr<Node> theNode = GetObject<Node> ();
       m_recvSocket = Socket::CreateSocket (theNode, tid);
       InetSocketAddress local (m_ipv4->GetAddress (1, 0).GetLocal (), MASP_PORT_NUMBER);
      // InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), MASP_PORT_NUMBER);
       if (m_recvSocket->Bind (local))
         {
       	   NS_FATAL_ERROR ("Failed to bind() MASP Recvsocket");
         }
       m_recvSocket->SetRecvCallback (MakeCallback (&MaspRoutingProtocol::RecvMasp, this));
       m_recvSocket->SetRecvPktInfo (true);

     }





  if(m_sinkFlag)
    {
	  NS_LOG_DEBUG (" Starting on node " << m_mainAddress << " (Sink Node)");
	  SendBroadCast1();
    }
  else
    NS_LOG_DEBUG (" Starting on node " << m_mainAddress);

}


void
MaspRoutingProtocol::	 (Ptr<Socket> socket)
{
	NS_LOG_DEBUG ("Packet Received on: " << m_mainAddress);
}




void
MaspRoutingProtocol::SendBroadCast1 ()
{


/*
	  if (m_bcst1Count == bcst1RateLimit)
	      {
		  NS_LOG_DEBUG (" bcstRate reached");

	        return;
	      }
	    else
	      m_bcst1Count++;

	  NS_LOG_DEBUG (" Sending broadcast1 from " << m_mainAddress);


	  Simulator::Schedule (m_bcst1RateLimitTimer.GetDelayLeft () + MicroSeconds (100),
	  	                             &MaspRoutingProtocol::SendBroadCast1,0);
*/



	  Ptr<Packet> packet = Create<Packet> ();
	  BcstMsgHeader bcst1Header;
	  Ipv4Address destination;

	  bcst1Header.SetZoneId(1);
	  packet->AddHeader (bcst1Header);



	  destination = Ipv4Address ("255.255.255.255");

	  for (std::map<Ptr<Socket>, Ipv4InterfaceAddress>::const_iterator i =
	           m_socketAddresses.begin (); i != m_socketAddresses.end (); i++)
	      {
		  NS_LOG_DEBUG ("trying to send on ip" );
		 	   i->first->SendTo (packet, 0, InetSocketAddress (destination, MASP_PORT_NUMBER));
	      }






}



void
MaspRoutingProtocol::Bcst1RateLimitTimerExpire ()
{
  NS_LOG_FUNCTION (this);
  m_bcst1Count = 0;
  m_bcst1RateLimitTimer.Schedule (Seconds (1));
}



void
MaspRoutingProtocol::StartTimers(){
	  m_bcst1RateLimitTimer.SetFunction (&MaspRoutingProtocol::Bcst1RateLimitTimerExpire,
	                                    this);
	  m_bcst1RateLimitTimer.Schedule (Seconds (1));

}


int64_t
MaspRoutingProtocol::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_uniformRandomVariable->SetStream (stream);
  return 1;
}





void
MaspRoutingProtocol::SetSink(){

 m_sinkFlag = true;
}


}//end of masp namespace
}//end of ns3 namespace
