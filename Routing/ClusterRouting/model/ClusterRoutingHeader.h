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
      // If cluster Id is its own
      uint32_t clusterId;

      // get populated while sending
      std::vector<Ipv4Address> clusterMembers;

      // serialization and printing
      void Print(std::ostream &os) const;
      uint32_t GetSerializedSize() const;
      void Serialize(Buffer::Iterator start) const;
      uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
    };

    // Toggling of CH Request
    // CH Sends to all  its CMs
    struct CH_Toggle_Initialize
    {
      void Print(std::ostream &os) const;
      uint32_t GetSerializedSize() const;
      void Serialize(Buffer::Iterator start) const;
      uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
    };

    struct CH_Transfer_Data
    {
    };

    // CMs send this as a reply to CH_Toggle_Initialize
    // indicating their willingness to participate
    struct CH_Toggle_Participation
    {
      int coins;
      double energyLeft;

      void Print(std::ostream &os) const;
      uint32_t GetSerializedSize() const;
      void Serialize(Buffer::Iterator start) const;
      uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
    };

    struct CH_Change_Advertisement
    {
      int newClusterId;
      Ipv4Address newClusterAddr;

      void Print(std::ostream &os) const;
      uint32_t GetSerializedSize() const;
      void Serialize(Buffer::Iterator start) const;
      uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
    };

  private:
    struct
    {
      Hello hello;
      ClusterAdvertisement clusterAdvertisement;
      CH_Toggle_Initialize chToggleInitialize;
      CH_Toggle_Participation chToggleParticipation;
      CH_Change_Advertisement chChangeAdvertisement;
      CH_Transfer_Data chTransferData;
    } m_message;

  public:
    // Getters and Setters
    void SetHello(Hello hello)
    {
      this->m_messageType = ClusterMessageType::HELLO_MESSAGE;
      this->m_message.hello = hello;
    }

    void SetClusterAdvertisement(ClusterAdvertisement cadv)
    {
      this->m_messageType = ClusterMessageType::CLUSTER_ADVERTISEMENT;
      this->m_message.clusterAdvertisement = cadv;
    }

    void SetChToggleInitialize(CH_Toggle_Initialize chtz)
    {
      this->m_messageType = ClusterMessageType::CH_TOGGLE_INITIALIZE;
      this->m_message.chToggleInitialize = chtz;
    }

    void SetChToggleParticipation(CH_Toggle_Participation ctp)
    {
      this->m_messageType = ClusterMessageType::CH_TOGGLE_PARTICIPATION;
      this->m_message.chToggleParticipation = ctp;
    }

    void SetChTransferData(CH_Transfer_Data ctd)
    {
      this->m_messageType = ClusterMessageType::CH_TRANSFER_DATA;
      this->m_message.chTransferData = ctd;
    }

    void SetChChangeAdvertisement(CH_Change_Advertisement ccadv)
    {
      this->m_messageType = ClusterMessageType::CH_CHANGE_ADVERTISEMENT;
      this->m_message.chChangeAdvertisement = ccadv;
    }

    Hello GetHello() const
    {
      return this->m_message.hello;
    }

    ClusterAdvertisement GetClusterAdvertisement() const
    {
      return this->m_message.clusterAdvertisement;
    }

    CH_Toggle_Initialize GetChToggleInitialize() const
    {
      return m_message.chToggleInitialize;
    }

    CH_Toggle_Participation GetChToggleParticipation() const
    {
      return m_message.chToggleParticipation;
    }

    CH_Transfer_Data GetChTransferData() const
    {
      return m_message.chTransferData;
    }

    CH_Change_Advertisement GetChChangeAdvertisement() const
    {
      return m_message.chChangeAdvertisement;
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