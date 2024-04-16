#ifndef CLUSTER_ROUTING_REPOSITORY_H
#define CLUSTER_ROUTING_REPOSITORY_H

#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/UADCConstants.h"

#include <set>
#include <vector>

namespace ns3
{

  struct NeighborEntry
  {
    Ipv4Address neighborAddress;
    Vector neighborPosition;
    Vector neighborVelocity;
    uint16_t neighborNodeId;
    // status of the neighbor node
    ClusterNodeType neighborStatus;

    // Time at which this tuple expires and must be removed
    Time time;
  };

  struct ClusterMemberEntry
  {
      Ipv4Address memberAddress;
      double Reputation;
  };


  // type defines
  // Neighbor list
  typedef std::vector<NeighborEntry> NeighborList;
  typedef std::vector<ClusterMemberEntry> ClusterMembers;

  // comparision and printing overloads

} // namespace ns3

#endif