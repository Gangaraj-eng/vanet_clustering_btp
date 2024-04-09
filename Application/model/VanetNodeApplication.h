
#ifndef VANET_NODE_APPLICATION_H
#define VANET_NODE_APPLICATION_H

#include "ns3/application.h"

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

  protected:
  };
} // namespace ns3

#endif