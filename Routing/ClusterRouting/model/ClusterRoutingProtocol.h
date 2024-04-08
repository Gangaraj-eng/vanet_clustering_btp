
#ifndef CLUSTERROUTINGPROTOCOL_H
#define CLUSTERROUTINGPROTOCOL_H

#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/node.h"

namespace ns3
{
  class ClusterRoutingProtocol : public Ipv4RoutingProtocol
  {
  public:
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

  private:
  };
} // namespace ns3

#endif