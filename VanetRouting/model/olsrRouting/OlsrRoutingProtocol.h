#ifndef OLSR_ROUTING_PROTOCOL
#define OLSR_ROUTING_PROTOCOL

/**
 * This is the header file for olsr routing protocol
 * A modified version of the standard olsr routing protocol
 */

#include "ns3/ipv4-routing-protocol.h"

namespace ns3
{
  namespace btp
  {
    class OlsrRoutingProtocol : public Ipv4RoutingProtocol
    {
    public:
      OlsrRoutingProtocol();
      ~OlsrRoutingProtocol();

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
    };

  } // namespace btp
} // namespace ns3

#endif