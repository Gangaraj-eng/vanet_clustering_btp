#ifndef CLUSTER_ROUTING_PROTOCOL_H
#define CLUSTER_ROUTING_PROTOCOL_H

#include "ns3/ipv4-routing-protocol.h"
#include "ns3/timer.h"
#include "ns3/nstime.h"
#include "ns3/VanetRoutingRepository.h"

namespace ns3
{
  namespace btp
  {
    class ClusterRoutingProtocol : public Ipv4RoutingProtocol
    {
    public:
      ClusterRoutingProtocol();
      ~ClusterRoutingProtocol();

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

      // required state variables
    private:
      NodeType mNodeType;       // type of cluster node - CH/CM
      Ipv4Address m_mainAddress; // interface address used for this protocol
      uint32_t m_mainInterfaceIndex; // index of the interface used 

      // sockets
      // one for recieving and one for sending
      Ptr<Socket> m_sendSocket;
      Ptr<Socket> m_recvSocket;

      // Times
      Time m_helloInterval;
      Time m_clusterAdvertisementInterval; // time interval between cluster advertisements
      Time m_chToggleIntiateWaitTime;

      // Timers
      Timer m_helloTimer;
      Timer m_clusterAdvertisementTimer;
      Timer m_clusterToggleInitiateTimer;
      Timer m_evaluateNewClusterTimer;
      Timer m_sendClsuterToggleParticipation;

       // main main things
      Ptr<Ipv4> m_ipv4;

    private:
    void RecieveMsg(Ptr<Socket> socket);

    public:
      void SetMainInterface(uint32_t interfaceIndex);

    };
  } // namespace btp

} // namespace ns3

#endif