#ifndef VANET_ROUTING_PROTOCOL_H
#define VANET_ROUTING_PROTOCOL_H

/**
 * This header file contains the combined routing protocol
 * Three interfaces used with each of them running a different protocol
 * Appropriate interfaces are turned On and Off according to the requirement
 */

// Required header files
#include "ns3/ipv4-routing-protocol.h"

namespace ns3
{
  namespace btp
  {
    class VanetRoutingProtocol : public Ipv4RoutingProtocol
    {
    public:
      VanetRoutingProtocol();
      ~VanetRoutingProtocol();
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

      private:
        Ptr<Socket> m_crSendSocket;
        Ptr<Socket> m_olsrSendSocket;
        Ptr<Socket> m_qrSendSocket;
    };
  } // namespace btp

} // namespace ns3

#endif