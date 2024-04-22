
#include "ClusterRoutingProtocol.h"
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
#include "ns3/ipv4-list-routing.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/UADCConstants.h"

#define CR_ROUTING_MAX_SEQ_NUM 65535
#define CR_REFRESH_INTERVAL m_helloInterval
#define CR_ROUTING_NEIGHB_HOLD_TIME Time(3 * CR_REFRESH_INTERVAL)
#define CR_MAX_MSGS_PER_PACKET 64

// A random variable delay so that brodcasting won't happen at exact same
// time which leads to packet collision
#define CR_ROUTING_MAXJITTER (m_helloInterval.GetSeconds() / 4)
#define JITTER (Seconds(m_uniformRandomVariable->GetValue(0, CR_ROUTING_MAXJITTER)))

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("ClusterRoutingProtocol");
  NS_OBJECT_ENSURE_REGISTERED(ClusterRoutingProtocol);

  ClusterRoutingProtocol::ClusterRoutingProtocol()
      : m_helloTimer(Timer::CANCEL_ON_DESTROY),
        m_clusterAdvertisementTimer(Timer::CANCEL_ON_DESTROY)
  {
    m_uniformRandomVariable = CreateObject<UniformRandomVariable>();
    m_mainInterface = 1;
  }

  ClusterRoutingProtocol::~ClusterRoutingProtocol()
  {
  }

  TypeId ClusterRoutingProtocol::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::ClusterRoutingProtocol")
                            .SetParent<Ipv4RoutingProtocol>()
                            .SetGroupName("ClusterRouting")
                            .AddConstructor<ClusterRoutingProtocol>()
                            .AddAttribute("HelloInterval",
                                          "Hello messages emission interval",
                                          TimeValue(Seconds(2)),
                                          MakeTimeAccessor(&ClusterRoutingProtocol::m_helloInterval),
                                          MakeTimeChecker())
                            .AddAttribute("ClusterAdvertisementInterval",
                                          "Cluster Advertisement emission interval",
                                          TimeValue(Seconds(5)),
                                          MakeTimeAccessor(&ClusterRoutingProtocol::m_clusterAdvertisementInterval),
                                          MakeTimeChecker())
                            .AddAttribute("CHToggleInitiateWaitTime",
                                          "Time to wait after sending cluster toggling initiation request",
                                          TimeValue(Seconds(5)),
                                          MakeTimeAccessor(&ClusterRoutingProtocol::m_chToggleIntiateWaitTime),
                                          MakeTimeChecker());
    return tid;
  }

  Ptr<Ipv4Route>
  ClusterRoutingProtocol::RouteOutput(Ptr<Packet> p,
                                      const Ipv4Header &header,
                                      Ptr<NetDevice> oif,
                                      Socket::SocketErrno &sockerr)
  {
    Ipv4Address destAddr = header.GetDestination();
    Ptr<Ipv4Route> rtentry = Create<Ipv4Route>();
    if (m_state.GetClusterNodetype() == ClusterNodeType::ClusterMember)
    {
      // send it to cluster head
      rtentry->SetSource(m_mainAddress);
      rtentry->SetDestination(m_state.GetClusterHeadAddress());
      rtentry->SetGateway(m_state.GetClusterHeadAddress());
      rtentry->SetOutputDevice(m_ipv4->GetNetDevice(m_mainInterface));
    }
    else if (m_state.GetClusterNodetype() == ClusterNodeType::ClusterHead)
    {
      rtentry->SetSource(header.GetSource());
      // if destination is in same cluster, send to it directly
      if (m_state.FindClusterMember(header.GetDestination()))
      {
        rtentry->SetGateway(header.GetDestination());
        rtentry->SetOutputDevice(m_ipv4->GetNetDevice(m_mainInterface));
        rtentry->SetDestination(header.GetDestination());
      }
      else
      {
        // Get the CH Address of destination and send it using MOLSR
      }
    }
    return rtentry;
  }

  bool ClusterRoutingProtocol::RouteInput(Ptr<const Packet> p,
                                          const Ipv4Header &header,
                                          Ptr<const NetDevice> idev,
                                          UnicastForwardCallback ucb,
                                          MulticastForwardCallback mcb,
                                          LocalDeliverCallback lcb,
                                          ErrorCallback ecb)
  {
    Ipv4Address dst = header.GetDestination();
    Ipv4Address origin = header.GetSource();

    // consume self packets
    if (IsMyOwnAddress(origin) == true)
    {
      return true;
    }

    NS_ASSERT(m_ipv4->GetInterfaceForDevice(idev) >= 0);
    uint32_t iif = m_ipv4->GetInterfaceForDevice(idev);

    if (dst == m_mainAddress)
    {
      if (m_state.GetClusterNodetype() == ClusterNodeType::ClusterHead)
      {
        // if cluster head, then check whether it should be redirected to some
        // cluster member or not
        Ptr<Packet> packetCopy = p->Copy();
        lcb(packetCopy, header, iif);
      }
      return true;
    }

    if (m_ipv4->IsDestinationAddress(dst, iif))
    {
      if (!lcb.IsNull())
      {
        lcb(p, header, iif);
        return true;
      }
      else
      {
        return false;
      }
    }
    return true;
  }

  void ClusterRoutingProtocol::NotifyInterfaceUp(uint32_t interface)
  {
  }

  void ClusterRoutingProtocol::NotifyInterfaceDown(uint32_t interface)
  {
  }

  void ClusterRoutingProtocol::NotifyAddAddress(uint32_t interface, Ipv4InterfaceAddress address)
  {
  }

  void ClusterRoutingProtocol::NotifyRemoveAddress(uint32_t interface, Ipv4InterfaceAddress address)
  {
  }

  void ClusterRoutingProtocol::SetIpv4(Ptr<Ipv4> ipv4)
  {
    m_ipv4 = ipv4;
    m_messageSequenceNumber = CR_ROUTING_MAX_SEQ_NUM;
    m_state.SetNode(ipv4->GetObject<Node>());

    m_helloTimer.SetFunction(&ClusterRoutingProtocol::HelloTimerExpire, this);
    m_clusterAdvertisementTimer.SetFunction(&ClusterRoutingProtocol::ClusterAdvertisementTimerExpire, this);
    m_clusterToggleInitiateTimer.SetFunction(&ClusterRoutingProtocol::SendClusterToggleInitiateRequest, this);
    m_evaluateNewClusterTimer.SetFunction(&ClusterRoutingProtocol::EvaluateNewCluster, this);
    m_sendClsuterToggleParticipation.SetFunction(&ClusterRoutingProtocol::SendClusterToggleParticipation, this);
  }

  void ClusterRoutingProtocol::PrintRoutingTable(Ptr<OutputStreamWrapper> stream, Time::Unit unit) const
  {
  }

  void ClusterRoutingProtocol::DoInitialize()
  {
    // NS_LOG_INFO(m_ipv4->GetAddress(1,0));
    if (m_mainAddress == Ipv4Address())
    {
      m_mainAddress = m_ipv4->GetAddress(m_mainInterface, 0).GetLocal();
      m_state.SetClusterHeadAddress(m_mainAddress);
    }
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    Ptr<Node> theNode = GetObject<Node>();

    if (!m_recvSocket)
    {
      m_recvSocket = Socket::CreateSocket(theNode, tid);
      m_recvSocket->SetAllowBroadcast(true);
      InetSocketAddress local(Ipv4Address::GetAny(), CLUSTER_ROUTING_PORT_NUMBER);
      m_recvSocket->SetRecvCallback(MakeCallback(&ClusterRoutingProtocol::RecieveMsg, this));
      m_recvSocket->BindToNetDevice(m_ipv4->GetNetDevice(m_mainInterface));
      if (m_recvSocket->Bind(local))
      {
        NS_LOG_INFO("Failed to bind() Recvsocket");
      }
      m_recvSocket->SetRecvPktInfo(true);
      m_recvSocket->ShutdownSend();
    }

    if (!m_sendSocket)
    {
      m_sendSocket = Socket::CreateSocket(theNode, tid);
      m_sendSocket->SetAllowBroadcast(true);
      m_sendSocket->SetIpTtl(1);
      m_sendSocket->BindToNetDevice(m_ipv4->GetNetDevice(m_mainInterface));
      InetSocketAddress inetAddr(m_mainAddress, CLUSTER_ROUTING_PORT_NUMBER);
      if (m_sendSocket->Bind(inetAddr))
      {
        NS_LOG_INFO("Failed to bind() socket");
      }
      m_sendSocket->SetRecvPktInfo(true);
    }

    HelloTimerExpire();
    m_clusterAdvertisementTimer.Schedule(m_clusterAdvertisementInterval + JITTER);
  }

  void ClusterRoutingProtocol::DoDispose()
  {
    if (m_recvSocket)
      m_recvSocket->Close();
    if (m_sendSocket)
      m_sendSocket->Close();
    Ipv4RoutingProtocol::DoDispose();
  }

  void ClusterRoutingProtocol::SendHello()
  {
    // NS_LOG_INFO("Sending from "<<m_mainAddress<<": "<<Simulator::Now());
    ClusterMessageHeader msg;
    Time now = Simulator::Now();
    Ptr<Packet> packet = Create<Packet>();
    PopulateMessageHeader(msg);

    ClusterMessageHeader::Hello hello;
    hello.nodePosition = m_state.GetNodePosition();
    hello.nodeVelocity = m_state.GetNodeVelocity();
    msg.SetHello(hello);
    packet->AddHeader(msg);

    // int sz=m_sendSocket->SendTo(packet, 0, InetSocketAddress(destination, CLUSTER_ROUTING_PORT_NUMBER));
    SendPacket(packet);
  }

  void ClusterRoutingProtocol::SendClusterAdvertisement()
  {

    if (m_state.GetClusterNodetype() != ClusterNodeType::ClusterHead)
    {
      return;
    }
    // NS_LOG_INFO(m_mainAddress<<" "<<m_state.GetClusterNodetype()<<" - "<<ClusterNodeType::ClusterHead);
    Ptr<Packet> packet = Create<Packet>();
    ClusterMessageHeader msg;
    PopulateMessageHeader(msg);

    ClusterMessageHeader::ClusterAdvertisement cadv;
    ClusterMembers clusterMembers = m_state.GetClusterMemberList();
    cadv.clusterId = m_state.GetClusterId();
    for (auto cm : clusterMembers)
    {
      cadv.clusterMembers.emplace_back(cm.memberAddress);
    }
    msg.SetClusterAdvertisement(cadv);
    packet->AddHeader(msg);

    Ipv4Address clusterHeadAddr = m_state.GetClusterHeadAddress();
    // NS_LOG_INFO(clusterHeadAddr << " " << m_mainAddress);
    if (clusterHeadAddr != m_mainAddress)
    {
      // This is a CH but cluster addr != main address, so it just merget into other
      m_state.SetClusterNodetype(ClusterNodeType::ClusterMember);
    }
    SendPacket(packet);

    // schedule now
  }

  void ClusterRoutingProtocol::SendClusterToggleInitiateRequest()
  {
    if (m_state.GetClusterNodetype() != ClusterNodeType::ClusterHead)
    {
      return;
    }
    Ptr<Packet> packet = Create<Packet>();
    ClusterMessageHeader msg;
    PopulateMessageHeader(msg);
    m_evaluateNewClusterTimer.Schedule(m_chToggleIntiateWaitTime + JITTER);
    ClusterMessageHeader::CH_Toggle_Initialize cht;
    msg.SetChToggleInitialize(cht);
    packet->AddHeader(msg);
    SendPacket(packet);
  }

  void ClusterRoutingProtocol::SendClusterToggleParticipation()
  {
    if (m_state.GetClusterNodetype() == ClusterNodeType::ClusterMember)
    {
      Ptr<Packet> packet = Create<Packet>();
      ClusterMessageHeader msg;
      PopulateMessageHeader(msg);
      ClusterMessageHeader::CH_Toggle_Participation ctp;
      ctp.coins = 0;
      ctp.energyLeft = 0;
      msg.SetChToggleParticipation(ctp);
      packet->AddHeader(msg);
      SendPacket(packet);
    }
  }

  void ClusterRoutingProtocol::SendChChangeAdvertisement()
  {
    Ptr<Packet> packet = Create<Packet>();
    ClusterMessageHeader msg;
    PopulateMessageHeader(msg);
    ClusterMessageHeader::CH_Change_Advertisement chadv;
    chadv.newClusterAddr = m_state.GetClusterHeadAddress();
    chadv.newClusterId = m_state.GetClusterId();
    msg.SetChChangeAdvertisement(chadv);
    packet->AddHeader(msg);
    m_state.eraseClusterToggleParticipants();
    SendPacket(packet);
  }

  void ClusterRoutingProtocol::SendCHToggleAck()
  {
    Ptr<Packet> packet = Create<Packet>();
    ClusterMessageHeader msg;
    PopulateMessageHeader(msg);
    ClusterMessageHeader::CH_ToggledAcknowledgement chack;
    msg.SetChToggledAck(chack);
    packet->AddHeader(msg);
    SendPacket(packet);
  }

  void ClusterRoutingProtocol::SendCHTransferData(Ipv4Address addr)
  {
    NS_LOG_LOGIC(m_mainAddress << " sending ch transfer data " << addr);
    std::vector<Ipv4Address> cms;
    ClusterMembers cmEntries = m_state.GetClusterMemberList();
    for (auto it : cmEntries)
    {
      if (it.memberAddress != addr)
        cms.emplace_back(it.memberAddress);
    }
    PrintCluster();
    cms.emplace_back(m_mainAddress);
    NS_LOG_LOGIC("From sender " << cmEntries.size() << " " << cms.size());
    Ptr<Packet> packet = Create<Packet>();
    ClusterMessageHeader msg;
    PopulateMessageHeader(msg);
    ClusterMessageHeader::CH_Transfer_Data ctd;
    ctd.clusterMembers = cms;
    ctd.newChAddr = addr;
    ctd.numClusterMembers = cms.size();
    ctd.clusterMap = m_state.GetClusterMap();
    msg.SetChTransferData(ctd);
    packet->AddHeader(msg);
    SendPacket(packet, ctd.newChAddr);
  }

  // Broadcasts a packet to its neighbors
  void ClusterRoutingProtocol::SendPacket(Ptr<Packet> packet, Ipv4Address addr)
  {
    Ipv4Address destination = addr;
    if (addr == Ipv4Address())
    {
      destination = m_ipv4->GetAddress(m_mainInterface, 0).GetLocal().GetSubnetDirectedBroadcast(m_ipv4->GetAddress(m_mainInterface, 0).GetMask());
    }
    m_sendSocket->SendTo(packet, 0, InetSocketAddress(destination, CLUSTER_ROUTING_PORT_NUMBER));
  }

  void ClusterRoutingProtocol::RecieveMsg(Ptr<Socket> socket)
  {
    Ipv4PacketInfoTag interfaceInfo;
    Ptr<Packet> receivedPacket;
    Address sourceAddress;
    receivedPacket = socket->RecvFrom(sourceAddress);

    if (!receivedPacket->RemovePacketTag(interfaceInfo))
    {
      NS_ABORT_MSG("No incoming interface on OLSR message, aborting.");
    }
    uint32_t incomingIf = interfaceInfo.GetRecvIf();
    Ptr<Node> node = this->GetObject<Node>();
    Ptr<NetDevice> dev = node->GetDevice(incomingIf);
    uint32_t recvInterfaceIndex = m_ipv4->GetInterfaceForDevice(dev);

    InetSocketAddress inetSourceAddr = InetSocketAddress::ConvertFrom(sourceAddress);
    Ipv4Address senderIfaceAddr = inetSourceAddr.GetIpv4();
    Ipv4Address receiverIfaceAddr = m_ipv4->GetAddress(recvInterfaceIndex, 0).GetLocal();
    NS_LOG_LOGIC(senderIfaceAddr << " " << receiverIfaceAddr);
    ClusterMessageHeader msgHeader;
    receivedPacket->RemoveHeader(msgHeader);
    // NS_LOG_INFO(senderIfaceAddr<<" "<< msgHeader.GetOriginatorAddress()<<" - "<<msgHeader.GetNodeId()<<" "<<msgHeader.GetMessageType());
    // NS_LOG_INFO(senderIfaceAddr<<" "<<msgHeader.GetMessageType()<<" "<<m_mainAddress<<" "<<Simulator::Now());
    switch (msgHeader.GetMessageType())
    {
    case ClusterMessageType::HELLO_MESSAGE:
      ProcessHello(msgHeader, receiverIfaceAddr, senderIfaceAddr);
      break;

    case ClusterMessageType::CLUSTER_ADVERTISEMENT:
      ProcessClusterAdvertisement(msgHeader);
      break;
    case ClusterMessageType::CH_TOGGLE_INITIALIZE:
      ProcessClusterToggleInitiate(msgHeader);
      break;
    case ClusterMessageType::CH_TOGGLE_PARTICIPATION:
      ProcessClusterToggleParticipation(msgHeader);
      break;
    case ClusterMessageType::CH_CHANGE_ADVERTISEMENT:
      ProcessChChangeAdvertisement(msgHeader);
      break;
    case ClusterMessageType::CH_TRANSFER_DATA:
      ProcessChTransferData(msgHeader);
      break;
    case ClusterMessageType::CH_ToggledAck:
      ProcessChToggleAck(msgHeader);
      break;
    default:
      break;
    }
  }

  void ClusterRoutingProtocol::HelloTimerExpire()
  {
    SendHello();
    m_helloTimer.Schedule(m_helloInterval + JITTER);
  }

  void ClusterRoutingProtocol::NeighborTimerExpire(Ipv4Address neighborIfaceAddress)
  {
    Time now = Simulator::Now();

    NeighborEntry *entry = m_state.FindNeighborEntry(neighborIfaceAddress);
    if (entry == nullptr)
    {
      return;
    }
    if (entry->time < now)
    {
      m_state.EraseNeighborEntry(neighborIfaceAddress);
    }
    else
    {
      m_events.Track(Simulator::Schedule(DELAY(entry->time), &ClusterRoutingProtocol::NeighborTimerExpire, this, neighborIfaceAddress));
    }
  }

  void ClusterRoutingProtocol::ClusterAdvertisementTimerExpire()
  {
    SendClusterAdvertisement();
    m_clusterAdvertisementTimer.Schedule(m_clusterAdvertisementInterval + JITTER);
  }

  void ClusterRoutingProtocol::EvaluateNewCluster()
  {
    clusterTogglePariticipants ctps = m_state.GetClusterToggleParticipants();
    NS_LOG_LOGIC(m_mainAddress << " - " << m_state.GetClusterToggleParticipants().size());
    if (ctps.size() == 0)
      return;
    Ipv4Address newChAddr = ctps[0].memberAdr;
    NS_LOG_LOGIC(m_mainAddress << " " << newChAddr);
    m_state.newCHAddr = newChAddr;
    Simulator::Schedule(JITTER, &ClusterRoutingProtocol::SendCHTransferData, this, newChAddr);
  }

  double findDistance(Vector a, Vector b)
  {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
  }

  void ClusterRoutingProtocol::ProcessHello(ClusterMessageHeader helloMessage, Ipv4Address recievedIfaceAddress, Ipv4Address senderIfaceAddress)
  {
    NS_LOG_LOGIC(m_mainAddress << " " << helloMessage.GetOriginatorAddress() << " " << helloMessage.GetNodeId());
    NeighborEntry *n_entry = m_state.FindNeighborEntry(senderIfaceAddress);
    ClusterMessageHeader::Hello hello = helloMessage.GetHello();
    Vector currPosition = m_state.GetNodePosition();
    Vector senderPosition = hello.nodePosition;
    double distance = findDistance(currPosition, senderPosition);
    if (distance > 10000.0 / 2)
      return;
    if (n_entry != nullptr)
    {
      // Neighbor already exists
      // update link expire time
      n_entry->time = Simulator::Now() + helloMessage.GetVTime();
      n_entry->neighborPosition = hello.nodePosition;
      n_entry->neighborVelocity = hello.nodeVelocity;
      m_events.Track(Simulator::Schedule(DELAY(n_entry->time),
                                         &ClusterRoutingProtocol::NeighborTimerExpire, this, n_entry->neighborAddress));
      return;
    }
    else
    {
      n_entry = new NeighborEntry();
      n_entry->neighborAddress = senderIfaceAddress;
      n_entry->time = Simulator::Now() + helloMessage.GetVTime();
      n_entry->neighborPosition = hello.nodePosition;
      n_entry->neighborVelocity = hello.nodeVelocity;
      m_state.AddNeighbor(*n_entry);
      m_events.Track(Simulator::Schedule(DELAY(n_entry->time),
                                         &ClusterRoutingProtocol::NeighborTimerExpire, this, n_entry->neighborAddress));
    }
  }

  void ClusterRoutingProtocol::ProcessClusterAdvertisement(ClusterMessageHeader message)
  {
    // NS_LOG_INFO(m_mainAddress<<" "<<message.GetOriginatorAddress());
    // NS_LOG_INFO(message.GetClusterId()<<" "<<m_state.GetNodeId());
    if (message.GetClusterId() == m_state.GetNodeId())
    {
      // it means they want to merge
      // NS_LOG_INFO(m_mainAddress<<"_"<<message.GetOriginatorAddress());
      std::vector<Ipv4Address> adjacentCMs = message.GetClusterAdvertisement().clusterMembers;
      adjacentCMs.emplace_back(message.GetOriginatorAddress());
      m_state.AddClusterMembers(adjacentCMs);
      if (!m_clusterToggleInitiateTimer.IsRunning())
        m_clusterToggleInitiateTimer.Schedule(Seconds(5) + JITTER);
    }
    else if (message.GetClusterId() != message.GetNodeId())
    {
    }
    else
    {
      // NS_LOG_INFO("me "<<m_mainAddress<<" merging into "<<message.GetOriginatorAddress());
      // else let's merge
      if (m_clusterAdvertisementTimer.IsRunning())
        m_clusterAdvertisementTimer.Cancel();
      m_state.SetClusterHeadAddress(message.GetOriginatorAddress());
      m_state.SetClusterId(message.GetClusterId());
      m_state.SetClusterMemberList({});

      // state updated, so instantly send it
      m_clusterAdvertisementTimer.SetDelay(JITTER);
      m_clusterAdvertisementTimer.Schedule();
      // SendClusterAdvertisement();
    }
  }

  void ClusterRoutingProtocol::ProcessClusterToggleInitiate(ClusterMessageHeader message)
  {

    // message sender is not its cluster head
    if (m_state.GetClusterId() != message.GetNodeId())
    {
      return;
    }
    // NS_LOG_INFO("Node " << m_mainAddress << " recieved cht from " << message.GetOriginatorAddress());
    // SendClusterToggleParticipation();
    if (not m_sendClsuterToggleParticipation.IsRunning())
    {
      m_sendClsuterToggleParticipation.SetDelay(JITTER);
      m_sendClsuterToggleParticipation.Schedule();
    }
  }

  void ClusterRoutingProtocol::ProcessClusterToggleParticipation(ClusterMessageHeader message)
  {
    // only CH will process
    if (m_state.GetClusterNodetype() != ClusterNodeType::ClusterHead)
    {
      return;
    }
    // if the sender is its clustermember
    if (m_state.GetNodeId() == message.GetClusterId())
    {
      ClusterMessageHeader::CH_Toggle_Participation ctpMsg = message.GetChToggleParticipation();
      Ipv4Address cmAddr = message.GetOriginatorAddress();
      ClusterMembers cms = m_state.GetClusterMemberList();
      ClusterMemberEntry *cmtr = m_state.FindClusterMember(cmAddr);
      if (cmtr != nullptr)
      {
        ClusterTogglePariticpant ctp;
        ctp.memberAdr = cmtr->memberAddress;
        ctp.position = cmtr->position;
        ctp.velocity = cmtr->velocity;
        ctp.EnergyLeft = ctpMsg.energyLeft;
        ctp.coins = ctpMsg.coins;
        ctp.nodeId = message.GetNodeId();
        ctp.Reputation = cmtr->Reputation;
        m_state.AddClusterToggleParticipant(ctp);
      }
    }
  }

  void ClusterRoutingProtocol::ProcessChChangeAdvertisement(ClusterMessageHeader message)
  {
    if (m_state.GetClusterNodetype() == ClusterHead)
      return;
    // if send from CH, then only update
    if (message.GetNodeId() == m_state.GetClusterId() and message.GetOriginatorAddress() == m_state.GetClusterHeadAddress())
    {
      ClusterMessageHeader::CH_Change_Advertisement chadv = message.GetChChangeAdvertisement();
      NS_LOG_INFO(m_mainAddress << " | " << message.GetOriginatorAddress() << " " << chadv.newClusterAddr << " " << chadv.newClusterId);
      if (chadv.newClusterAddr == m_mainAddress)
      {
        m_state.SetClusterHeadAddress(m_mainAddress);
        m_state.SetClusterNodetype(ClusterNodeType::ClusterHead);
        m_state.SetClusterId(m_state.GetNodeId());
      }
      else
      {
        m_state.SetClusterId(chadv.newClusterId);
        m_state.SetClusterHeadAddress(chadv.newClusterAddr);
        m_state.SetClusterNodetype(ClusterNodeType::ClusterMember);
        m_state.SetClusterMemberList({});
      }
    }
  }

  void ClusterRoutingProtocol::ProcessChToggleAck(ClusterMessageHeader msg)
  {
    if (msg.GetOriginatorAddress() == m_state.newCHAddr)
    {
      m_state.SetClusterHeadAddress(m_state.newCHAddr);
      m_state.newCHAddr = Ipv4Address();
      m_state.SetClusterNodetype(ClusterNodeType::ClusterMember);
      m_state.SetClusterMemberList({});
      m_state.SetClusterId(msg.GetNodeId());
      // sned this new ch info to all

      Simulator::Schedule(JITTER, &ClusterRoutingProtocol::SendChChangeAdvertisement, this);
    }
  }

  void ClusterRoutingProtocol::ProcessChTransferData(ClusterMessageHeader msg)
  {
    ClusterMessageHeader::CH_Transfer_Data ctd = msg.GetChTransferData();
    if (ctd.newChAddr == m_mainAddress and msg.GetNodeId() == m_state.GetClusterId())
    {
      NS_LOG_LOGIC(m_mainAddress << " elected as new CH");
      m_state.SetClusterHeadAddress(m_mainAddress);
      m_state.SetClusterNodetype(ClusterNodeType::ClusterHead);
      m_state.SetClusterId(m_state.GetNodeId());
      std::vector<Ipv4Address> cmsAddrs = ctd.clusterMembers;
      ClusterMembers cms;
      for (auto it = cmsAddrs.begin(); it != cmsAddrs.end(); it++)
      {
        NeighborEntry *ntr = m_state.FindNeighborEntry(*it);
        if (ntr != nullptr)
        {
          ClusterMemberEntry cmcurr;
          cmcurr.memberAddress = ntr->neighborAddress;
          cmcurr.position = ntr->neighborPosition;
          cmcurr.velocity = ntr->neighborVelocity;
          cmcurr.Reputation = 0;
          cms.emplace_back(cmcurr);
        }
      }
      m_state.SetClusterMemberList(cms);
      m_state.SetClusterMap(ctd.clusterMap);
      PrintCluster();
      Simulator::Schedule(JITTER, &ClusterRoutingProtocol::SendCHToggleAck, this);
    }
  }

  bool ClusterRoutingProtocol::IsMyOwnAddress(Ipv4Address addr)
  {
    return addr == m_ipv4->GetAddress(m_mainInterface, 0).GetLocal();
  }

  void ClusterRoutingProtocol::PopulateMessageHeader(ClusterMessageHeader &msg)
  {
    msg.setVTime(CR_ROUTING_NEIGHB_HOLD_TIME);
    msg.setOriginatorAddress(m_mainAddress);
    msg.setHopCount(0);
    msg.setTimeToLive(1);
    msg.setMessageSequenceNumber(GetMessageSequenceNumber());
    msg.SetClusterId(m_state.GetClusterId());
    msg.setNodeId(m_state.GetNodeId());
  }

  void ClusterRoutingProtocol::PrintCluster()
  {
    if (ClusterNodeType::ClusterHead == m_state.GetClusterNodetype())
    {
      NS_LOG_INFO("CH :" << m_mainAddress);
      for (auto i : m_state.GetClusterMemberList())
      {
        NS_LOG_INFO("  CM " << i.memberAddress);
      }
    }
  }

  void ClusterRoutingProtocol::SetMainInterface(uint32_t interface)
  {
    m_mainInterface = interface;
    m_mainAddress = m_ipv4->GetAddress(interface, 0).GetLocal();
  }

  uint16_t ClusterRoutingProtocol::GetMessageSequenceNumber()
  {
    m_messageSequenceNumber = (m_messageSequenceNumber + 1) % CR_ROUTING_MAX_SEQ_NUM;
    return m_messageSequenceNumber;
  }
} // namespace ns3