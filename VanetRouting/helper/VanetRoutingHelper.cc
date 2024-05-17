#include "VanetRoutingHelper.h"

// Required header files
#include "ns3/VanetRoutingProtocol.h"

namespace ns3
{
  namespace btp
  {
    VanetRoutingHelper::VanetRoutingHelper()
    {
      m_agentFactory.SetTypeId("ns3::btp::VanetRoutingProtocol");
    }

    VanetRoutingHelper *VanetRoutingHelper::Copy() const
    {
      return new VanetRoutingHelper(*this);
    }

    Ptr<Ipv4RoutingProtocol> VanetRoutingHelper::Create(Ptr<Node> node) const
    {
      Ptr<VanetRoutingProtocol> routingAgent = m_agentFactory.Create<VanetRoutingProtocol>();
      node->AggregateObject(routingAgent);
      return routingAgent;
    }

    void VanetRoutingHelper::Set(std::string name, const AttributeValue &value)
    {
      m_agentFactory.Set(name, value);
    }

    // TODO: Complete this function to Assign streams
    int64_t VanetRoutingHelper::AssignStreams(NodeContainer c, int64_t stream)
    {
      return 0;
    }
  } // namespace btp

} // namespace ns3
