
#ifndef CLUSTERROUTINGPROTOCOL_H
#define CLUSTERROUTINGPROTOCOL_H

#include "ClusterNodeState.h"
#include "ClusterRoutingHeader.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/node.h"
#include "ns3/timer.h"
#include "ns3/nstime.h"
#include "ns3/random-variable-stream.h"
#include "ns3/event-garbage-collector.h"

namespace ns3
{
  class ClusterRoutingProtocol : public Ipv4RoutingProtocol
  {
  public:
    ClusterRoutingProtocol();
    ~ClusterRoutingProtocol();
    static TypeId GetTypeId();

    // Functions of Ipv4routing protocol
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
    // required variables
    Ptr<Ipv4> m_ipv4;
    Ipv4Address m_mainAddress;
    ClusterNodeState m_state;
    uint16_t m_messageSequenceNumber;
    Ptr<UniformRandomVariable> m_uniformRandomVariable;
    EventGarbageCollector m_events;

    // sending functions
    void SendPacket(Ptr<Packet> packet);
    void SendHello();
    void SendClusterAdvertisement();
    void SendClusterToggleInitiateRequest();
    void SendClusterToggleParticipation();
    void SendChChangeAdvertisement(Ptr<Packet> packet);
    void RecieveMsg(Ptr<Socket> socket);

    // sockets
    // one for recieving and one for sending
    Ptr<Socket> m_sendSocket;
    Ptr<Socket> m_recvSocket;

    // interface on which this protocol runs
    uint32_t m_mainInterface = 1;

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

    // Timer functions
    void HelloTimerExpire();
    void NeighborTimerExpire(Ipv4Address neighborIfaceAddr);
    void ClusterAdvertisementTimerExpire();
    void EvaluateNewCluster();

    // process incoming messages
    void ProcessHello(ClusterMessageHeader message, Ipv4Address recievedIfaceAddress, Ipv4Address senderIfaceAddress);
    void ProcessClusterAdvertisement(ClusterMessageHeader message);
    void ProcessClusterToggleInitiate(ClusterMessageHeader message);
    void ProcessClusterToggleParticipation(ClusterMessageHeader message);
    void ProcessChChangeAdvertisement(ClusterMessageHeader message);


    uint16_t GetMessageSequenceNumber();
    bool IsMyOwnAddress(Ipv4Address addr);
    void PopulateMessageHeader(ClusterMessageHeader &msg);

  public:
    void SetMainInterface(uint32_t interface);
  };
} // namespace ns3

#endif