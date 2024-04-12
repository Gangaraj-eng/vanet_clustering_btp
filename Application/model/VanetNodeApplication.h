
#ifndef VANET_NODE_APPLICATION_H
#define VANET_NODE_APPLICATION_H

#include "ns3/application.h"
#include "ns3/socket-factory.h"

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
    void RecieveMessage(Ptr<Socket> socket);
    void sendPacket();
  protected:
     Ptr<Socket> m_sendSocket;
     Ptr<Socket> m_recSocket;
  };
} // namespace ns3

#endif