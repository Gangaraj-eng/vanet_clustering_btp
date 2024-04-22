#include "ClusterRoutingHeader.h"

// Cr - cluster routing
// size in bytes
#define CR_PKT_HEADER_SIZE 4
#define CR_MSG_HEADER_SIZE 16
#define IPV4_ADDRESS_SIZE 4
#define VECTOR_SIZE 24

namespace ns3
{

#define CR_C 0.0625
  uint8_t
  SecondsToEmf(double seconds)
  {
    int a;
    int b = 0;
    // find the largest integer 'b' such that: T/C >= 2^b
    for (b = 1; (seconds / CR_C) >= (1 << b); ++b)
    {
    }
    b--;
    double tmp = 16 * (seconds / (CR_C * (1 << b)) - 1);
    a = (int)std::ceil(tmp - 0.5);
    if (a == 16)
    {
      b += 1;
      a = 0;
    }
    return (uint8_t)((a << 4) | b);
  }
  double
  EmfToSeconds(uint8_t olsrFormat)
  {
    int a = (olsrFormat >> 4);
    int b = (olsrFormat & 0xf);
    return CR_C * (1 + a / 16.0) * (1 << b);
  }

  NS_LOG_COMPONENT_DEFINE("ClusterPacketHeader");
  NS_OBJECT_ENSURE_REGISTERED(ClusterPacketHeader);
  ClusterPacketHeader::ClusterPacketHeader() {}
  ClusterPacketHeader::~ClusterPacketHeader() {}
  TypeId ClusterPacketHeader::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::ClusterPacketHeader")
                            .SetParent<Header>()
                            .SetGroupName("btp")
                            .AddConstructor<ClusterPacketHeader>();
    return tid;
  }
  TypeId ClusterPacketHeader::GetInstanceTypeId() const
  {
    return GetTypeId();
  }
  uint32_t ClusterPacketHeader::GetSerializedSize() const
  {
    return CR_PKT_HEADER_SIZE;
  }
  void ClusterPacketHeader::Print(std::ostream &os) const
  {
  }
  void ClusterPacketHeader::Serialize(Buffer::Iterator start) const
  {
    Buffer::Iterator i = start;
    i.WriteHtonU16(m_packetLength);
    i.WriteHtonU16(m_packetSequenceNumber);
  }
  uint32_t ClusterPacketHeader::Deserialize(Buffer::Iterator start)
  {
    Buffer::Iterator i = start;
    m_packetLength = i.ReadNtohU16();
    m_packetSequenceNumber = i.ReadNtohU16();
    return GetSerializedSize();
  }

  NS_OBJECT_ENSURE_REGISTERED(ClusterMessageHeader);
  ClusterMessageHeader::ClusterMessageHeader()
  {
    this->m_messageType = ClusterMessageType::NONE;
  }

  ClusterMessageHeader::~ClusterMessageHeader() {}

  TypeId ClusterMessageHeader::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::ClusterMessageHeader")
                            .SetParent<Header>()
                            .SetGroupName("btp")
                            .AddConstructor<ClusterMessageHeader>();
    return tid;
  }

  TypeId ClusterMessageHeader::GetInstanceTypeId() const { return GetTypeId(); }
  void ClusterMessageHeader::Print(std::ostream &os) const {}
  uint32_t ClusterMessageHeader::GetSerializedSize() const
  {
    uint32_t size = CR_MSG_HEADER_SIZE;
    switch (m_messageType)
    {
    case ClusterMessageType::HELLO_MESSAGE:
      size += m_message.hello.GetSerializedSize();
      break;
    case ClusterMessageType::CLUSTER_ADVERTISEMENT:
      size += m_message.clusterAdvertisement.GetSerializedSize();
      break;
    case ClusterMessageType::CH_TOGGLE_INITIALIZE:
      size += m_message.chToggleInitialize.GetSerializedSize();
      break;
    case ClusterMessageType::CH_TOGGLE_PARTICIPATION:
      size += m_message.chToggleParticipation.GetSerializedSize();
      break;
    case ClusterMessageType::CH_CHANGE_ADVERTISEMENT:
      size += m_message.chChangeAdvertisement.GetSerializedSize();
      break;
    case ClusterMessageType::CH_ToggledAck:
      size += m_message.chToggleAck.GetSerializedSize();
      break;
    case ClusterMessageType::CH_TRANSFER_DATA:
      size += m_message.chTransferData.GetSerializedSize();
      break;

    default:
      break;
    }
    return size;
  }
  void ClusterMessageHeader::Serialize(Buffer::Iterator start) const
  {
    Buffer::Iterator i = start;
    i.WriteU8(m_messageType);
    i.WriteU8(m_vTime);
    i.WriteHtonU16(GetSerializedSize());
    i.WriteHtonU32(m_originatorAddress.Get());
    i.WriteU8(m_timeToLive);
    i.WriteU8(m_hopCount);
    i.WriteHtonU16(m_messageSequenceNumber);
    i.WriteHtonU16(m_clusterId);
    i.WriteHtonU16(m_nodeId);
    switch (m_messageType)
    {
    case ClusterMessageType::HELLO_MESSAGE:
      m_message.hello.Serialize(i);
      break;
    case ClusterMessageType::CLUSTER_ADVERTISEMENT:
      m_message.clusterAdvertisement.Serialize(i);
      break;
    case ClusterMessageType::CH_TOGGLE_INITIALIZE:
      m_message.chToggleInitialize.Serialize(i);
      break;
    case ClusterMessageType::CH_TOGGLE_PARTICIPATION:
      m_message.chToggleParticipation.Serialize(i);
      break;
    case ClusterMessageType::CH_CHANGE_ADVERTISEMENT:
      m_message.chChangeAdvertisement.Serialize(i);
      break;
    case ClusterMessageType::CH_ToggledAck:
      m_message.chToggleAck.Serialize(i);
      break;
    case ClusterMessageType::CH_TRANSFER_DATA:
      m_message.chTransferData.Serialize(i);
      break;
    default:
      break;
    }
  }
  uint32_t ClusterMessageHeader::Deserialize(Buffer::Iterator start)
  {
    uint32_t size;
    Buffer::Iterator i = start;
    m_messageType = ClusterMessageType(i.ReadU8());
    m_vTime = i.ReadU8();
    m_messageSize = i.ReadNtohU16();
    m_originatorAddress = Ipv4Address(i.ReadNtohU32());
    m_timeToLive = i.ReadU8();
    m_hopCount = i.ReadU8();
    m_messageSequenceNumber = i.ReadNtohU16();
    m_clusterId = i.ReadNtohU16();
    m_nodeId = i.ReadNtohU16();
    size = CR_MSG_HEADER_SIZE;
    switch (m_messageType)
    {
    case ClusterMessageType::HELLO_MESSAGE:
      size += this->m_message.hello.Deserialize(i, m_messageSize - CR_MSG_HEADER_SIZE);
      break;
    case ClusterMessageType::CLUSTER_ADVERTISEMENT:
      size += this->m_message.clusterAdvertisement.Deserialize(i, m_messageSize - CR_MSG_HEADER_SIZE);
      break;
    case ClusterMessageType::CH_TOGGLE_INITIALIZE:
      size += this->m_message.chToggleInitialize.Deserialize(i, m_messageSize - CR_MSG_HEADER_SIZE);
      break;
    case ClusterMessageType::CH_TOGGLE_PARTICIPATION:
      size += this->m_message.chToggleParticipation.Deserialize(i, m_messageSize - CR_MSG_HEADER_SIZE);
      break;
    case ClusterMessageType::CH_CHANGE_ADVERTISEMENT:
      size += this->m_message.chChangeAdvertisement.Deserialize(i, m_messageSize - CR_MSG_HEADER_SIZE);
      break;
    case ClusterMessageType::CH_ToggledAck:
      size += this->m_message.chToggleAck.Deserialize(i, m_messageSize - CR_MSG_HEADER_SIZE);
      break;
    case ClusterMessageType::CH_TRANSFER_DATA:
      size += this->m_message.chTransferData.Deserialize(i, m_messageSize - CR_MSG_HEADER_SIZE);
      break;

    default:
      break;
    }
    return size;
  }

  //  Hell Message
  void ClusterMessageHeader::Hello::Print(std::ostream &os) const
  {
  }

  uint32_t ClusterMessageHeader::Hello::GetSerializedSize() const
  {
    return 2 * VECTOR_SIZE;
  }

  // 24 bytes for a vector
  void ClusterMessageHeader::Hello::Serialize(Buffer::Iterator start) const
  {
    Buffer::Iterator i = start;
    i.Write((uint8_t *)&nodePosition, sizeof(nodePosition));
    i.Write((uint8_t *)&nodeVelocity, sizeof(nodeVelocity));
  }

  uint32_t ClusterMessageHeader::Hello::Deserialize(Buffer::Iterator start, uint32_t messageSize)
  {
    Buffer::Iterator i = start;
    i.Read((uint8_t *)&this->nodePosition, sizeof(nodePosition));
    i.Read((uint8_t *)&this->nodeVelocity, sizeof(nodeVelocity));
    // NS_LOG_LOGIC("Recieved node "<<nodePosition<<" "<<nodeVelocity);
    return Hello::GetSerializedSize();
  }

  // Cluster Advertisement
  void ClusterMessageHeader::ClusterAdvertisement::Print(std::ostream &os) const
  {
  }

  uint32_t ClusterMessageHeader::ClusterAdvertisement::GetSerializedSize() const
  {
    return 4 + this->clusterMembers.size() * IPV4_ADDRESS_SIZE;
  }

  // 24 bytes for a vector
  void ClusterMessageHeader::ClusterAdvertisement::Serialize(Buffer::Iterator start) const
  {
    Buffer::Iterator i = start;
    i.WriteHtonU32(clusterId);
    for (std::vector<Ipv4Address>::const_iterator it = clusterMembers.begin();
         it != clusterMembers.end(); it++)
    {
      i.WriteHtonU32(it->Get());
    }
  }

  uint32_t ClusterMessageHeader::ClusterAdvertisement::Deserialize(Buffer::Iterator start, uint32_t messageSize)
  {
    Buffer::Iterator i = start;
    this->clusterId = i.ReadNtohU32();
    this->clusterMembers.clear();
    int numCMMembers = (messageSize - 4) / IPV4_ADDRESS_SIZE;
    for (int n = 0; n < numCMMembers; ++n)
    {
      this->clusterMembers.emplace_back(i.ReadNtohU32());
    }
    return messageSize;
  }

  // CH Toggle Initialize
  void ClusterMessageHeader::CH_Toggle_Initialize::Print(std::ostream &os) const
  {
  }

  uint32_t ClusterMessageHeader::CH_Toggle_Initialize::GetSerializedSize() const
  {
    return 0;
  }

  void ClusterMessageHeader::CH_Toggle_Initialize::Serialize(Buffer::Iterator start) const
  {
  }

  uint32_t ClusterMessageHeader::CH_Toggle_Initialize::Deserialize(Buffer::Iterator start, uint32_t messageSize)
  {
    return this->GetSerializedSize();
  }

  // CH Toggle Participation
  void ClusterMessageHeader::CH_Toggle_Participation::Print(std::ostream &os) const
  {
  }

  uint32_t ClusterMessageHeader::CH_Toggle_Participation::GetSerializedSize() const
  {
    // two double values
    return 8 + 4;
  }

  void ClusterMessageHeader::CH_Toggle_Participation::Serialize(Buffer::Iterator start) const
  {
    start.WriteHtonU32(this->coins);
    start.WriteHtonU64(this->energyLeft);
  }

  uint32_t ClusterMessageHeader::CH_Toggle_Participation::Deserialize(Buffer::Iterator start, uint32_t messageSize)
  {
    this->coins = start.ReadNtohU32();
    this->energyLeft = start.ReadNtohU64();
    return this->GetSerializedSize();
  }

  // CH Change advertisement
  void ClusterMessageHeader::CH_Change_Advertisement::Print(std::ostream &os) const
  {
  }

  uint32_t ClusterMessageHeader::CH_Change_Advertisement::GetSerializedSize() const
  {
    // int + double value
    return 4 + IPV4_ADDRESS_SIZE;
  }

  void ClusterMessageHeader::CH_Change_Advertisement::Serialize(Buffer::Iterator start) const
  {
    start.WriteHtonU32(this->newClusterId);
    start.WriteHtonU32(this->newClusterAddr.Get());
  }

  uint32_t ClusterMessageHeader::CH_Change_Advertisement::Deserialize(Buffer::Iterator start, uint32_t messageSize)
  {
    this->newClusterId = start.ReadNtohU32();
    this->newClusterAddr = Ipv4Address(start.ReadNtohU32());
    return this->GetSerializedSize();
  }

  // CH Toggled Acknowledgement
  void ClusterMessageHeader::CH_ToggledAcknowledgement::Print(std::ostream &os) const
  {
  }

  uint32_t ClusterMessageHeader::CH_ToggledAcknowledgement::GetSerializedSize() const
  {

    return 0;
  }

  void ClusterMessageHeader::CH_ToggledAcknowledgement::Serialize(Buffer::Iterator start) const
  {
  }

  uint32_t ClusterMessageHeader::CH_ToggledAcknowledgement::Deserialize(Buffer::Iterator start, uint32_t messageSize)
  {

    return this->GetSerializedSize();
  }

  // CH Transfer Data
  void ClusterMessageHeader::CH_Transfer_Data::Print(std::ostream &os) const
  {
  }

  uint32_t ClusterMessageHeader::CH_Transfer_Data::GetSerializedSize() const
  {

    return 4 + IPV4_ADDRESS_SIZE * (1 + clusterMembers.size() + 2 * clusterMap.size());
  }

  void ClusterMessageHeader::CH_Transfer_Data::Serialize(Buffer::Iterator start) const
  {
    Buffer::Iterator i = start;
    i.WriteHtonU32(this->numClusterMembers);
    i.WriteHtonU32(this->newChAddr.Get());
    for (auto it = clusterMembers.begin(); it != clusterMembers.end(); it++)
    {
      i.WriteHtonU32(it->Get());
    }
    for (auto it : clusterMap)
    {
      i.WriteHtonU32(it.first.Get());
      i.WriteHtonU32(it.second.Get());
    }
  }

  uint32_t ClusterMessageHeader::CH_Transfer_Data::Deserialize(Buffer::Iterator start, uint32_t messageSize)
  {
    Buffer::Iterator i = start;
    this->numClusterMembers = i.ReadNtohU32();
    this->newChAddr = Ipv4Address(i.ReadNtohU32());
    clusterMembers.clear();
    int n = numClusterMembers;
    while (n--)
    {
      clusterMembers.emplace_back(Ipv4Address(i.ReadNtohU32()));
    }
    clusterMap.clear();
    n = (messageSize - 4 - IPV4_ADDRESS_SIZE * (1 + numClusterMembers)) / (2 * IPV4_ADDRESS_SIZE);
    while (n--)
    {
      Ipv4Address nodeAddr = Ipv4Address(i.ReadNtohU32());
      Ipv4Address chAddr = Ipv4Address(i.ReadNtohU32());
      clusterMap[nodeAddr] = chAddr;
    }

    return this->GetSerializedSize();
  }

}