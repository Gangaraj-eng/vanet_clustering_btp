#ifndef CLUSTER_ROUTING_HEADER_H
#define CLUSTER_ROUTING_HEADER_H

#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/UADCConstants.h"
#include "ns3/vector.h"

namespace ns3
{

  double EmfToSeconds(uint8_t emf);
  uint8_t SecondsToEmf(double seconds);

  class ClusterPacketHeader : public Header
  {
  public:
    ClusterPacketHeader();
    ~ClusterPacketHeader() override;
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

  /**
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Message Type  | Vtime  | Message Size     |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |        originator Address                 |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  TTl          | Hop Count |Message seq num|
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   Cluster Index     |   Cluster Node Id   |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

  */
  class ClusterMessageHeader : public Header
  {
  public:
    static TypeId GetTypeId();
    TypeId GetInstanceTypeId() const override;
    void Print(std::ostream &os) const override;
    uint32_t GetSerializedSize() const override;
    void Serialize(Buffer::Iterator start) const override;
    uint32_t Deserialize(Buffer::Iterator start) override;
    ClusterMessageHeader();
    ~ClusterMessageHeader() override;

  private:
    ClusterMessageType m_messageType;
    uint8_t m_vTime; // validity time of the message
    uint16_t m_messageSize;
    Ipv4Address m_originatorAddress;
    uint8_t m_timeToLive;
    uint8_t m_hopCount;
    uint16_t m_messageSequenceNumber;
    uint16_t m_clusterId;
    uint16_t m_nodeId;

  public:
    // Hello message or becaon message indicating its existence
    struct Hello
    {
      Vector nodePosition;
      Vector nodeVelocity;

      // serialization and printing
      void Print(std::ostream &os) const;
      uint32_t GetSerializedSize() const;
      void Serialize(Buffer::Iterator start) const;
      uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
    };

    // Cluster heads will advertise its clusters
    // CHs will transfer between each other
    struct ClusterAdvertisement
    {
    };

    // Toggling of CH Request
    // CH Sends to all  its CMs
    struct CH_Toggle_Initialize
    {
    };

    // CMs send this as a reply to CH_Toggle_Initialize
    // indicating their willingness to participate
    struct CH_Toggle_Participation
    {
    };

    struct CH_Change_Advertisement
    {
    };

  private:
    struct
    {
      Hello hello;
    } m_message;

  public:
    // Getters and Setters
    void SetHello(Hello hello)
    {
      this->m_message.hello = hello;
    }

    Hello GetHello() const
    {
      return this->m_message.hello;
    }

    ClusterMessageType
    GetMessageType() const
    {
      return m_messageType;
    }
    void setMessageType(const ClusterMessageType &messageType) { m_messageType = messageType; }

    Time GetVTime() const { return Seconds(EmfToSeconds(m_vTime)); }
    void setVTime(Time vTime)
    {
      m_vTime = SecondsToEmf(vTime.GetSeconds());
      ;
    }

    uint16_t GetMessageSize() const { return GetSerializedSize(); }
    void setMessageSize(const uint16_t &messageSize) { m_messageSize = messageSize; }

    Ipv4Address GetOriginatorAddress() const { return m_originatorAddress; }
    void setOriginatorAddress(const Ipv4Address &originatorAddress) { m_originatorAddress = originatorAddress; }

    uint8_t GetTimeToLive() const { return m_timeToLive; }
    void setTimeToLive(const uint8_t &timeToLive) { m_timeToLive = timeToLive; }

    uint8_t GetHopCount() const { return m_hopCount; }
    void setHopCount(const uint8_t &hopCount) { m_hopCount = hopCount; }

    uint16_t GetMessageSequenceNumber() const { return m_messageSequenceNumber; }
    void setMessageSequenceNumber(const uint16_t &messageSequenceNumber) { m_messageSequenceNumber = messageSequenceNumber; }

    uint16_t GetClusterId() const { return m_clusterId; }
    void SetClusterId(const uint16_t &clusterIndex) { m_clusterId = clusterIndex; }

    uint16_t GetNodeId() const { return m_nodeId; }
    void setNodeId(const uint16_t &nodeId) { m_nodeId = nodeId; }
  };
} // namespace ns3

#endif