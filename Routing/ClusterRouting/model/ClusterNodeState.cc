#include "ClusterNodeState.h"

namespace ns3
{
  ClusterNodeState::ClusterNodeState()
  {
    m_clusterNodetype = ClusterNodeType::ClusterHead;
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

  double ClusterNodeState::GetEnergyLeft() const{
    return 0.0;
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

    ClusterMemberEntry *ClusterNodeState::FindClusterMember(Ipv4Address cmAddr){
        for(auto it=m_clusterMemberList.begin();it!=m_clusterMemberList.end();it++){
          if(it->memberAddress==cmAddr){
            return &(*it);
          }
        }
        return nullptr;
    }

  void
  ClusterNodeState::AddClusterMembers(Ipv4Address newMember)
  {
    NeighborEntry *n_entry = FindNeighborEntry(newMember);
    if (n_entry != nullptr)
    {
      ClusterMemberEntry cmEntry;
      cmEntry.memberAddress = n_entry->neighborAddress;
      cmEntry.position = n_entry->neighborPosition;
      cmEntry.velocity = n_entry->neighborVelocity;
      cmEntry.Reputation = 0;
      m_clusterMemberList.emplace_back(cmEntry);
    }
  }

  void
  ClusterNodeState::AddClusterMembers(std::vector<Ipv4Address> newMember)
  {
    for (auto it = newMember.begin(); it != newMember.end(); it++)
    {
      AddClusterMembers(*it);
    }
  }

  void ClusterNodeState::AddClusterToggleParticipant(ClusterTogglePariticpant ctp)
  {
    m_clusterToggleParticipants.emplace_back(ctp);
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

  void ClusterNodeState::SetClusterNodetype(const ClusterNodeType clusterNodetype)
  {
    m_clusterNodetype = clusterNodetype;
  }

  Ptr<VanetNode> ClusterNodeState::GetNode() const
  {
    return m_node;
  }

  void ClusterNodeState::SetNode(Ptr<Node> node)
  {
    m_node = DynamicCast<VanetNode>(node);
    m_nodeId = m_node->GetNodeId();
    m_clusterId = m_nodeId;
  }

  clusterTogglePariticipants ClusterNodeState::GetClusterToggleParticipants() const
  {
    return m_clusterToggleParticipants;
  }

  void ClusterNodeState::SetClusterToggleParticipants(const clusterTogglePariticipants &clusterToggleParticipants)
  {
    m_clusterToggleParticipants = clusterToggleParticipants;
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

  ClusterMembers ClusterNodeState::GetClusterMemberList() const
  {
    return m_clusterMemberList;
  }

  void ClusterNodeState::SetClusterMemberList(const ClusterMembers &clusterMemberList)
  {
    m_clusterMemberList = clusterMemberList;
  }
} // namespace ns3
