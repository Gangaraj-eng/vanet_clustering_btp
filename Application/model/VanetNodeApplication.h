
#ifndef VANET_NODE_APPLICATION_H
#define VANET_NODE_APPLICATION_H

#include "ns3/application.h"
#include "ns3/socket-factory.h"
#include "ns3/VanetNode.h"

namespace ns3
{
  class VanetNodeApplication : public Application
  {
  public:
    static TypeId GetTypeId();
    VanetNodeApplication();
    ~VanetNodeApplication() override;

  private:
    void StartApplication() override;
    void StopApplication() override;
    void SendPacket();
    void RecieveMessage(Ptr<Socket> socket);
    bool IsNodeClusterHead();

  protected:
     // sockets to send and recieve sockets
     Ptr<Socket> m_sendSocket;
     Ptr<Socket> m_recvSocket;
     Ptr<VanetNode> m_node;
  };
} // namespace ns3

#endif