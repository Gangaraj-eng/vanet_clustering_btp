
#ifndef CLUSTERROUTINGPROTOCOL_H
#define CLUSTERROUTINGPROTOCOL_H

#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/node.h"
#include "ns3/timer.h"
#include "ns3/nstime.h"

namespace ns3
{
  class ClusterRoutingProtocol : public Ipv4RoutingProtocol
  {
  public:
    static const uint16_t CLUSTER_ROUTING_PORT_NUMBER;
    static TypeId GetTypeId();
    ClusterRoutingProtocol();
    ~ClusterRoutingProtocol();
    Ptr<Ipv4Route> RouteOutput(Ptr<Packet> p,
                               const Ipv4Header &header,
                               Ptr<NetDevice> oif,
                               Socket::SocketErrno &sockerr) override;
    bool RouteInput(Ptr<const Packet> p,
                    const Ipv4Header &header,
                    Ptr<const NetDevice> idev,
                    UnicastForwardCallback ucb,
                    MulticastForwardCallback mcb,
                    LocalDeliverCallback lcb,
                    ErrorCallback ecb) override;
    void NotifyInterfaceUp(uint32_t interface) override;
    void NotifyInterfaceDown(uint32_t interface) override;
    void NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address) override;
    void NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address) override;
    void SetIpv4(Ptr<Ipv4> ipv4) override;
    void PrintRoutingTable(Ptr<OutputStreamWrapper> stream,
                           Time::Unit unit = Time::S) const override;

  public:
    bool IsMyOwnAddress(Ipv4Address addr);

  private:
    Time m_helloInterval;

    Timer m_helloTimer;

    void HelloTimerExpire();

    void SendHello();

    void RecieveMsg(Ptr<Socket> socket);

    // sockets
    // one for recieving and one for sending
    Ptr<Socket> m_sendSocket;
    Ptr<Socket> m_recvSocket;

  private:
    // required variables
    Ptr<Ipv4> m_ipv4;
    Ipv4Address m_mainAddress;

    // interface on which this protocol runs
    uint32_t m_mainInterface = 1;

  protected:
    void DoInitialize() override;
    void DoDispose() override;

  public:
    void SetMainInterface(uint32_t interface);
  };
} // namespace ns3

#endif