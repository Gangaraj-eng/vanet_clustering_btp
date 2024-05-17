#ifndef VANET_ROUTING_PROTOCOL_H
#define VANET_ROUTING_PROTOCOL_H

/**
 * This header file contains the combined routing protocol
 * It uses three interface routing protocol objects
 * and controls them accordingly
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

  protected:
    void DoInitialize() override;
    void DoDispose() override;

    private:
      // flags to control routings
      bool m_crRunFlag;
      bool m_olsrRungFlag;
      bool m_qrRunFlag;

      // interface indices to run protocols
      int16_t m_crInterfaceIndex;
      int16_t m_olsrInterfaceIndex;
      int16_t m_qrInterfaceIndex;

      // main main things
      Ptr<Ipv4> m_ipv4;
    };
  } // namespace btp

} // namespace ns3

#endif