#include "ClusterRoutingHelper.h"

#include "ns3/names.h"
#include "ns3/ptr.h"
#include "ns3/ClusterRoutingProtocol.h"

namespace ns3
{
  ClusterRoutingHelper::ClusterRoutingHelper()
  {
    m_agentFactory.SetTypeId("ns3::ClusterRoutingProtocol");
  }

  ClusterRoutingHelper *ClusterRoutingHelper::Copy() const
  {
    return new ClusterRoutingHelper(*this);
  }

  Ptr<Ipv4RoutingProtocol> ClusterRoutingHelper::Create(Ptr<Node> node) const
  {
    Ptr<ClusterRoutingProtocol> agent = m_agentFactory.Create<ClusterRoutingProtocol>();
    node->AggregateObject(agent);
    auto it = m_interfaceMaps.find(node);
    if (it != m_interfaceMaps.end())
    {
      agent->SetMainInterface(it->second);
    }
    return agent;
  }

  void ClusterRoutingHelper::Set(std::string name, const AttributeValue &value)
  {
    m_agentFactory.Set(name, value);
  }

  void ClusterRoutingHelper::SetMainInterface(Ptr<Node> node, uint32_t interface)
  {
    m_interfaceMaps[node] = interface;
  }

  // TODO: Complete this function
  int64_t ClusterRoutingHelper::AssignStreams(NodeContainer c, int64_t stream)
  {
    return 0;
  }
};