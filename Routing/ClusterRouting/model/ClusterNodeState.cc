#include "ClusterNodeState.h"

namespace ns3
{
  ClusterNodeState::ClusterNodeState()
  {
    m_clusterId = -1;
  }

  ClusterNodeState::~ClusterNodeState()
  {
  }

  Vector ClusterNodeState::GetNodePosition() const
  {
    Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel>();
    Vector position = mobility->GetPosition();
    return position;
  }

  Vector ClusterNodeState::GetNodeVelocity() const
  {
    Ptr<MobilityModel> mobility = m_node->GetObject<MobilityModel>();
    Vector velocity = mobility->GetVelocity();
    return velocity;
  }

  void ClusterNodeState::AddNeighbor(const NeighborEntry &nEntry)
  {
    m_neighbors.push_back(nEntry);
  }

  NeighborEntry *ClusterNodeState::FindNeighborEntry(Ipv4Address neighborAddr)
  {
    for (std::vector<NeighborEntry>::iterator it = m_neighbors.begin();
         it != m_neighbors.end(); it++)
    {
      if (neighborAddr == it->neighborAddress)
      {
        return &(*it);
      }
    }
    return nullptr;
  }

  void ClusterNodeState::EraseNeighborEntry(Ipv4Address neighborAddr)
  {
    for (NeighborList::iterator it = m_neighbors.begin();
         it != m_neighbors.end(); it++)
    {
      if (it->neighborAddress == neighborAddr)
      {
        m_neighbors.erase(it);
        break;
      }
    }
  }

  // Getters and setters
  int ClusterNodeState::GetClusterId() const
  {
    return m_clusterId;
  }

  void ClusterNodeState::SetClusterId(int clusterId)
  {
    m_clusterId = clusterId;
  }

  ClusterNodeType ClusterNodeState::GetClusterNodetype() const
  {
    return m_clusterNodetype;
  }

  void ClusterNodeState::SetClusterNodetype(const ClusterNodeType &clusterNodetype)
  {
    m_clusterNodetype = clusterNodetype;
  }

  Ptr<Node> ClusterNodeState::GetNode() const {
    return m_node;
  }

  void ClusterNodeState::SetNode(Ptr<Node> node) {
    m_node = node;
  }

  uint32_t ClusterNodeState::GetNodeId() const
  {
    return m_nodeId;
  }

  void ClusterNodeState::SetNodeId(const uint32_t &nodeId)
  {
    m_nodeId = nodeId;
  }

  Ipv4Address ClusterNodeState::GetClusterHeadAddress() const
  {
    return clusterHeadAddress;
  }

  void ClusterNodeState::SetClusterHeadAddress(const Ipv4Address &ClusterHeadAddress)
  {
    clusterHeadAddress = ClusterHeadAddress;
  }

  NeighborList ClusterNodeState::GetNeighbors() const
  {
    return m_neighbors;
  }

  void ClusterNodeState::SetNeighbors(const NeighborList &neighbors)
  {
    m_neighbors = neighbors;
  }
} // namespace ns3
