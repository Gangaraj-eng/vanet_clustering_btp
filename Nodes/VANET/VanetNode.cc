
#include "VanetNode.h"

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("VanetNode");

  VanetNode::VanetNode()
  {
    // initially unclustered
    // NS_LOG_INFO("Hello from the Vanet Node constructor");
  }

  uint32_t VanetNode::GetClusterId() const {
    return m_clusterId;
  }

  void VanetNode::SetClusterId(const uint32_t &clusterId) {
    m_clusterId = clusterId;
  }

  uint32_t VanetNode::GetNodeId() const {
    return m_nodeId;
  }

  void VanetNode::SetNodeId(const uint32_t &nodeId) {
    m_nodeId = nodeId;
  }

  Ipv4Address VanetNode::GetClusterHeadAddress() const {
    return m_clusterHeadAddress;
  }

  void VanetNode::SetClusterHeadAddress(const Ipv4Address &clusterHeadAddress) {
    m_clusterHeadAddress = clusterHeadAddress;
  }
} // namespace ns3