#ifndef CLUSTER_ROUTING_HEADER_H
#define CLUSTER_ROUTING_HEADER_H

#include "ns3/header.h"

namespace ns3
{
  class PacketHeader : public Header
  {
  public:
    PacketHeader();
    ~PacketHeader() override;
    static TypeId GetTypeId();
    TypeId GetInstanceTypeId() const override;
    void Print(std::ostream &os) const override;
    uint32_t GetSerializedSize() const override;
    void Serialize(Buffer::Iterator start) const override;
    uint32_t Deserialize(Buffer::Iterator start) override;

    void SetPacketLength(uint16_t length) { m_packetLength = length; }
    uint16_t GetPacketLength() const { return m_packetLength; }
    void SetPacketSequenceNumber(uint16_t seqnum) { m_packetSequenceNumber = seqnum; }
    uint16_t GetPacketSequenceNumber() const { return m_packetSequenceNumber; }

  private:
    uint16_t m_packetLength;
    uint16_t m_packetSequenceNumber;
  };

  class MessageHeader : public Header
  {
  public:
    enum MessageType
    {
      HELLO_MESSAGE = 1,
      TC_MESSAGE = 2
    };
    MessageHeader();
    ~MessageHeader() override;
  };
} // namespace ns3

#endif