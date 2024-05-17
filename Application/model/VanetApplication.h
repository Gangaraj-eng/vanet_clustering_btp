
#ifndef VANET_NODE_APPLICATION_H
#define VANET_NODE_APPLICATION_H

#include "ns3/application.h"
#include "ns3/socket-factory.h"

#include "ns3/VanetApplicationRepository.h"

namespace ns3
{
  namespace btp
  {
    class VanetApplication : public Application
    {
    public:
      static TypeId GetTypeId();
      VanetApplication();
      ~VanetApplication() override;

    private:
      void StartApplication() override;
      void StopApplication() override;
      void SendPacket();
      void RecieveMessage(Ptr<Socket> socket);

    protected:
      // sockets to send and recieve sockets
      Ptr<Socket> m_sendSocket;
      Ptr<Socket> m_recvSocket;
    };
  } // namespace btp
} // namespace ns3

#endif