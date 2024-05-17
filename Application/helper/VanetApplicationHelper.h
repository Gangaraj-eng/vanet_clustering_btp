#ifndef VANET_NODE_APPLICATION_HELPER_H
#define VANET_NODE_APPLICATION_HELPER_H

#include "ns3/address.h"
#include "ns3/address.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/VanetApplication.h"
#include "ns3/application-container.h"

namespace ns3
{
  namespace btp
  {
    class VanetApplicationHelper
    {
    public:
      VanetApplicationHelper();
      VanetApplicationHelper(std::string protocol, Address address);
      void SetAttribute(std::string nae, const AttributeValue &value);
      ApplicationContainer Install(NodeContainer c) const;
      ApplicationContainer Install(Ptr<Node> node) const;
      ApplicationContainer Install(std::string nodeName) const;
      int64_t AssignStreams(NodeContainer c, int64_t stream);

    private:
      Ptr<Application> InstallPriv(Ptr<Node> node) const;
      ObjectFactory m_factory;
    };
  } //  namespace btp
} // namespace ns3

#endif