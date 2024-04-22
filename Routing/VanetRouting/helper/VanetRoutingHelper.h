#ifndef VANET_ROUTING_HELPER_H
#define VANET_ROUTING_HELPER_H

/**
 * Helper class to install Vanet Routing in Nodes
 */

// Required header
#include "ns3/ipv4-routing-helper.h"
#include "ns3/node-container.h"

namespace ns3
{
  namespace btp
  {
    class VanetRoutingHelper : public Ipv4RoutingHelper
    {
    public:
      VanetRoutingHelper();
      VanetRoutingHelper *Copy() const override;
      void Set(std::string name, const AttributeValue &value);

      // create and add a routing protocol to node
      Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const override;

      // Assign a fixed random variable stream number to the random variables
      // used by this model - get reproducible results on every run
      int64_t AssignStreams(NodeContainer c, int64_t stream);
    private:
      // Factory to create VanetRouting object
      // can also be used to store attributes
      ObjectFactory m_agentFactory;
    };
  } // namespace btp

} // namespace ns3

#endif