/*
 * AUTHOR - Gangaraju Bopparm (202001107@daiict.ac.in)
 * VanetClusterHelper
 * Helper class to implement clustering routing protocol in VANETs
 * This c++ file contains the implementation for VanetClusterHelper
 */

#include "VanetClusterHelper.h"

#include "ns3/ipv4-list-routing.h"
#include "ns3/names.h"
#include "ns3/node-list.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/ptr.h"

namespace ns3
{
    VanetClusterRoutingHelper::VanetClusterRoutingHelper()
    {
        m_agentFactory.SetTypeId("ns3::VanetClustering::RoutingProtocol");
    }

    // copy constructor
    VanetClusterRoutingHelper::VanetClusterRoutingHelper(const VanetClusterRoutingHelper &v)
        : m_agentFactory(v.m_agentFactory)
    {
        m_interfaceMap = v.m_interfaceMap;
    }

    VanetClusterRoutingHelper *VanetClusterRoutingHelper::Copy() const
    {
        return new VanetClusterRoutingHelper(*this);
    }

    void VanetClusterRoutingHelper::SetInterfaceToUse(Ptr<Node> node, uint32_t interface)
    {
        m_interfaceMap.insert(std::make_pair(node, interface));
    }

    // setting attributes
    void VanetClusterRoutingHelper::Set(std::string name, const AttributeValue &value)
    {
        m_agentFactory.Set(name, value);
    }

} // namespace ns3