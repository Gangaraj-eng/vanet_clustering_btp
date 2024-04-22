#ifndef CLUSTER_NODE_STATE_H
#define CLUSTER_NODE_STATE_H

#include "ns3/mobility-model.h"
#include "ns3/node.h"
#include "ns3/UADCConstants.h"
#include "ClusterRoutingRepository.h"
#include "ns3/VanetNode.h"
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
    // cluster Id will be same as the id of its cluster head
    uint32_t m_clusterId;

    // Address of the current cluster head
    Ipv4Address clusterHeadAddress;

    // pointer of the node
    Ptr<VanetNode> m_node;

    // Node ID that we are using in this simulation
    uint32_t m_nodeId;

    ClusterNodeType m_clusterNodetype;

    NeighborList m_neighbors;

    ClusterMembers m_clusterMemberList;

    clusterTogglePariticipants m_clusterToggleParticipants;

    // map storing the node Address and its cluster Address
    // This data is maintained by constant communication with RSUs
    std::map<Ipv4Address,Ipv4Address> m_clusterMap;

  public:
    Ipv4Address newCHAddr;

    // functions for state management
    Vector GetNodeVelocity() const;

    Vector GetNodePosition() const;

    double GetEnergyLeft() const;

    void AddNeighbor(const NeighborEntry &nEntry);

    NeighborEntry *FindNeighborEntry(Ipv4Address neighborAddr);

    void EraseNeighborEntry(Ipv4Address neigborAddr);

    void EraseClusterMemberEntry(Ipv4Address addr);

    ClusterMemberEntry* FindClusterMember(Ipv4Address addr);

    void AddClusterMembers(Ipv4Address newMember);

    void AddClusterMembers(std::vector<Ipv4Address> newMembers);

    void AddClusterToggleParticipant(ClusterTogglePariticpant ctp);

    void eraseClusterToggleParticipants(){
      this->m_clusterToggleParticipants.clear();
    }

    void AddClusterMapEntry(Ipv4Address nodeAddr,Ipv4Address nodeCHAddr);

    // Getters and setters
    uint32_t GetNodeId() const;
    void SetNodeId(const uint32_t &nodeId);

    int GetClusterId() const;
    void SetClusterId(int clusterId);

    Ipv4Address GetClusterHeadAddress() const;
    void SetClusterHeadAddress(const Ipv4Address &ClusterHeadAddress);

    ClusterNodeType GetClusterNodetype() const;
    void SetClusterNodetype(const ClusterNodeType clusterNodetype);

    NeighborList GetNeighbors() const;
    void SetNeighbors(const NeighborList &neighbors);

    Ptr<VanetNode> GetNode() const;
    void SetNode(Ptr<Node> node);

    ClusterMembers GetClusterMemberList() const;
    void SetClusterMemberList(const ClusterMembers &clusterMemberList);

    clusterTogglePariticipants GetClusterToggleParticipants() const;
    void SetClusterToggleParticipants(const clusterTogglePariticipants &clusterToggleParticipants);

    std::map<Ipv4Address,Ipv4Address> GetClusterMap() const;
    void SetClusterMap(const std::map<Ipv4Address,Ipv4Address> &clusterMap);
  };
} // namespace ns3

#endif