#ifndef CLUSTER_NODE_STATE_H
#define CLUSTER_NODE_STATE_H

#include "ns3/mobility-model.h"
#include "ns3/node.h"
#include "ns3/UADCConstants.h"
#include "ClusterRoutingRepository.h"

#include <set>

namespace ns3
{
  class ClusterNodeState
  {
  public:
    ClusterNodeState();
    ~ClusterNodeState();

  private:
    // Id of the cluster this node belongs to
    // -1 if doesn't belong to any cluster
    int m_clusterId;

    // Address of the current cluster head
    Ipv4Address clusterHeadAddress;

    // pointer of the node
    Ptr<Node> m_node;

    // Node ID that we are using in this simulation
    uint32_t m_nodeId;

    ClusterNodeType m_clusterNodetype;

    NeighborList m_neighbors;

    ClusterMembers m_clusterMemberList;

  public:
    // functions for state management
    Vector GetNodeVelocity() const;

    Vector GetNodePosition() const;

    void AddNeighbor(const NeighborEntry &nEntry);

    NeighborEntry *FindNeighborEntry(Ipv4Address neighborAddr);

    void EraseNeighborEntry(Ipv4Address neigborAddr);

    // Getters and setters
    uint32_t GetNodeId() const;
    void SetNodeId(const uint32_t &nodeId);

    int GetClusterId() const;
    void SetClusterId(int clusterId);

    Ipv4Address GetClusterHeadAddress() const;
    void SetClusterHeadAddress(const Ipv4Address &ClusterHeadAddress);

    ClusterNodeType GetClusterNodetype() const;
    void SetClusterNodetype(const ClusterNodeType &clusterNodetype);

    NeighborList GetNeighbors() const;
    void SetNeighbors(const NeighborList &neighbors);

    Ptr<Node> GetNode() const;
    void SetNode(Ptr<Node> node);
  };
} // namespace ns3

#endif