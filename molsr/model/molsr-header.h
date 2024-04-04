
#ifndef MOLSR_HEADER_H
#define MOLSR_HEADER_H

#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"

namespace ns3
{
  namespace molsr
  {
    double EmfToSeconds(uint8_t emf);
    uint8_t SecondsToEmf(double seconds);

    class PacketHeader : public Header
    {
    public:
      PacketHeader();
      ~PacketHeader() override;
      void SetPacketLength(uint16_t length)
      {
        m_packetLength = length;
      }
      uint16_t GetPacketLength() const
      {
        return m_packetLength;
      }
      void SetPacketSequenceNumber(uint16_t seqnum)
      {
        m_packetSequenceNumber = seqnum;
      }
      uint16_t GetPacketSequenceNumber() const
      {
        return m_packetSequenceNumber;
      }

    private:
      uint16_t m_packetLength;         //!< The packet length.
      uint16_t m_packetSequenceNumber; //!< The packet sequence number.
    public:
      static TypeId GetTypeId();
      TypeId GetInstanceTypeId() const override;
      void Print(std::ostream &os) const override;
      uint32_t GetSerializedSize() const override;
      void Serialize(Buffer::Iterator start) const override;
      uint32_t Deserialize(Buffer::Iterator start) override;
    };

    // Message header contains the message part
    class MessageHeader : public Header
    {
    public:
      enum MessageType
      {
        HELLO_MESSAGE = 1,
        TC_MESSAGE = 2,
        MID_MESSAGE = 3,
        HNA_MESSAGE = 4,
      };

      MessageHeader();
      ~MessageHeader() override;

      void SetMessageType(MessageType messageType)
      {
        m_messageType = messageType;
      }
      MessageType GetMessageType() const
      {
        return m_messageType;
      }
      void SetVTime(Time time)
      {
        m_vTime = SecondsToEmf(time.GetSeconds());
      }
      Time GetVTime() const
      {
        return Seconds(EmfToSeconds(m_vTime));
      }
      void SetOriginatorAddress(Ipv4Address originatorAddress)
      {
        m_originatorAddress = originatorAddress;
      }
      Ipv4Address GetOriginatorAddress() const
      {
        return m_originatorAddress;
      }
      void SetTimeToLive(uint8_t timeToLive)
      {
        m_timeToLive = timeToLive;
      }
      uint8_t GetTimeToLive() const
      {
        return m_timeToLive;
      }
      void SetHopCount(uint8_t hopCount)
      {
        m_hopCount = hopCount;
      }
      uint8_t GetHopCount() const
      {
        return m_hopCount;
      }
      void SetMessageSequenceNumber(uint16_t messageSequenceNumber)
      {
        m_messageSequenceNumber = messageSequenceNumber;
      }
      uint16_t GetMessageSequenceNumber() const
      {
        return m_messageSequenceNumber;
      }

    private:
      MessageType m_messageType;        //!< The message type
      uint8_t m_vTime;                  //!< The validity time.
      Ipv4Address m_originatorAddress;  //!< The originator address.
      uint8_t m_timeToLive;             //!< The time to live.
      uint8_t m_hopCount;               //!< The hop count.
      uint16_t m_messageSequenceNumber; //!< The message sequence number.
      uint16_t m_messageSize;           //!< The message size.

    public:
      /**
       * \brief Get the type ID.
       * \return The object TypeId.
       */
      static TypeId GetTypeId();
      TypeId GetInstanceTypeId() const override;
      void Print(std::ostream &os) const override;
      uint32_t GetSerializedSize() const override;
      void Serialize(Buffer::Iterator start) const override;
      uint32_t Deserialize(Buffer::Iterator start) override;

      struct Mid
      {
        std::vector<Ipv4Address> interfaceAddresses;
        void Print(std::ostream &os) const;
        uint32_t GetSerializedSize() const;
        void Serialize(Buffer::Iterator start) const;
        uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
      };

      struct Hello
      {
        struct LinkMessage
        {
          uint8_t linkCode;
          std::vector<Ipv4Address> neighborInterfaceAddresses;
        };

        uint8_t hTime;
        void SetHTime(Time time)
        {
          this->hTime = SecondsToEmf(time.GetSeconds());
        }

        Time GetHTime() const
        {
          return Seconds(EmfToSeconds(this->hTime));
        }

        uint8_t willingness;
        std::vector<LinkMessage> linkMessages;
        void Print(std::ostream &os) const;
        uint32_t GetSerializedSize() const;
        void Serialize(Buffer::Iterator start) const;
        uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
      };

      struct Tc
      {
        std::vector<Ipv4Address> neighborAddresses;
        uint16_t ansn;
        void Print(std::ostream &os) const;
        uint32_t GetSerializedSize() const;
        void Serialize(Buffer::Iterator start) const;
        uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
      };

      struct Hna
      {
        struct Association
        {
          Ipv4Address address;
          Ipv4Mask mask;
        };
        std::vector<Association> associations;

        void Print(std::ostream &os) const;
        uint32_t GetSerializedSize() const;
        void Serialize(Buffer::Iterator start) const;
        uint32_t Deserialize(Buffer::Iterator start, uint32_t messageSize);
      };

    private:
      struct
      {
        Mid mid;
        Hello hello;
        Tc tc;
        Hna hna;
      } m_message;

    public:
      Mid &GetMid()
      {
        if (m_messageType == 0)
        {
          m_messageType = MID_MESSAGE;
        }
        else
        {
          NS_ASSERT(m_messageType == MID_MESSAGE);
        }
        return m_message.mid;
      }
      Hello &GetHello()
      {
        if (m_messageType == 0)
        {
          m_messageType = HELLO_MESSAGE;
        }
        else
        {
          NS_ASSERT(m_messageType == HELLO_MESSAGE);
        }
        return m_message.hello;
      }
      Tc &GetTc()
      {
        if (m_messageType == 0)
        {
          m_messageType = TC_MESSAGE;
        }
        else
        {
          NS_ASSERT(m_messageType == TC_MESSAGE);
        }
        return m_message.tc;
      }
      Hna &GetHna()
      {
        if (m_messageType == 0)
        {
          m_messageType = HNA_MESSAGE;
        }
        else
        {
          NS_ASSERT(m_messageType == HNA_MESSAGE);
        }
        return m_message.hna;
      }
      const Mid &GetMid() const
      {
        NS_ASSERT(m_messageType == MID_MESSAGE);
        return m_message.mid;
      }
      const Hello &GetHello() const
      {
        NS_ASSERT(m_messageType == HELLO_MESSAGE);
        return m_message.hello;
      }
      const Tc &GetTc() const
      {
        NS_ASSERT(m_messageType == TC_MESSAGE);
        return m_message.tc;
      }
      const Hna &GetHna() const
      {
        NS_ASSERT(m_messageType == HNA_MESSAGE);
        return m_message.hna;
      }
    };

    static inline std::ostream &
    operator<<(std::ostream &os, const MessageHeader &message)
    {
      message.Print(os);
      return os;
    }
    typedef std::vector<MessageHeader> MessageList;
    static inline std::ostream &
    operator<<(std::ostream &os, const MessageList &messages)
    {
      os << "[";
      for (std::vector<MessageHeader>::const_iterator messageIter = messages.begin();
           messageIter != messages.end();
           messageIter++)
      {
        messageIter->Print(os);
        if (messageIter + 1 != messages.end())
        {
          os << ", ";
        }
      }
      os << "]";
      return os;
    }

  }
}

#endif