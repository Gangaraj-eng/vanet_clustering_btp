#ifndef QUERY_ROUTING_PROTOCOL_H
#define QUERY_ROUTING_PROTOCOL_H

// Required header files
#include "ns3/ipv4-routing-protocol.h"

namespace ns3
{
  namespace btp
  {
    class QueryRoutingProtocol : public Ipv4RoutingProtocol
    {
    public:
      QueryRoutingProtocol();
      ~QueryRoutingProtocol();

      static TypeId GetTypeId();

      // parent class methods
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

    protected:
      void DoInitialize() override;
      void DoDispose() override;

    private:
      Ipv4Address m_mainAddress;
      uint32_t m_mainInterfaceIndex; // index of the interface used 

      // main main things
      Ptr<Ipv4> m_ipv4;

    public:
      void SetMainInterface(uint32_t interfaceIndex);
    };
  } // namespace btp

} // namespace ns3

#endif