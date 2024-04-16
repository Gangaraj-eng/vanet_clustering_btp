#include "ClusterRoutingHeader.h"

// Cr - cluster routing
// size in bytes
#define CR_PKT_HEADER_SIZE 4
#define CR_MSG_HEADER_SIZE 16
#define IPV4_ADDRESS_SIZE 4

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
  ClusterMessageHeader::ClusterMessageHeader() {}

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
      size+=m_message.hello.GetSerializedSize();
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
      size+=this->m_message.hello.Deserialize(i,m_messageSize-CR_MSG_HEADER_SIZE);
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
    return 48;
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
    return 48;
  }
}