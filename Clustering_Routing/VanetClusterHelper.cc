#include "VanetClusterHelper.h"
#include "VanetClusterRoutingProtocol.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/names.h"
#include "ns3/node-list.h"
#include "ns3/ptr.h"

namespace ns3
{
  namespace vcr
  {
    VanetClusterHelper::VanetClusterHelper()
    {
      m_agentFactory.SetTypeId("ns3::vcr::RoutingProtocol");
    }

    VanetClusterHelper *VanetClusterHelper::Copy() const
    {
      return new VanetClusterHelper(*this);
    }

    Ptr<Ipv4RoutingProtocol> VanetClusterHelper::Create(Ptr<Node> node) const
    {
      Ptr<VanetClusterRoutingProtocol> routing_agent = m_agentFactory.Create<VanetClusterRoutingProtocol>();
      node->AggregateObject(routing_agent);
    }

    void VanetClusterHelper::Set(std::string name, const AttributeValue &value)
    {
      m_agentFactory.Set(name, value);
    }
  } // namespace vcr

} // namespace ns3
