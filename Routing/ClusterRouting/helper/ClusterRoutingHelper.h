
#ifndef CLUSTER_ROUTING_HELPER_H
#define CLUSTER_ROUTING_HELPER_H

#include "ns3/ipv4-routing-helper.h"
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/object-factory.h"

namespace ns3
{
  class ClusterRoutingHelper : public Ipv4RoutingHelper
  {
  public:
    ClusterRoutingHelper();
    Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const override;
    ClusterRoutingHelper *Copy() const override;
    int64_t AssignStreams(NodeContainer c, int64_t stream);
    void Set(std::string name, const AttributeValue &value);

  private:
    // Factory to create ClusterRouting object
    // can also be used to set attributes
    ObjectFactory m_agentFactory;
  };
}

#endif