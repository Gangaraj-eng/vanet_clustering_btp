

#ifndef VANET_NODE_H
#define VANET_NODE_H

#include "ns3/node.h"

namespace ns3
{
  class VanetNode : public Node
  {
    uint32_t m_nodeId;
    uint32_t m_clusterId;
    Ipv4Address m_clusterHeadAddress;
    
  public:
    VanetNode();

    uint32_t GetNodeId() const;
    void SetNodeId(const uint32_t &nodeId);

    uint32_t GetClusterId() const;
    void SetClusterId(const uint32_t &clusterId);

    Ipv4Address GetClusterHeadAddress() const;
    void SetClusterHeadAddress(const Ipv4Address &clusterHeadAddress);
  };
}
#endif