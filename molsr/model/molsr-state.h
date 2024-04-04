

#ifndef MOLSR_STATE_H
#define MOLSR_STATE_H

#include "molsr_repositories.h"

namespace ns3
{
  namespace molsr
  {

    // This class encapsulates all data structures needed for
    // maintaining state of Modified OLSR
    class molsrState
    {
    protected:
      LinkSet m_linkSet;
      NeighborSet m_neighborSet;
      TwoHopNeighborSet m_twoHopNeighborSet;
      TopologySet m_topologySet;
      MprSet m_mprSet;
      MprSelectorSet m_mprSelectorSet;
      DuplicateSet m_duplicateSet;
      IfaceAssocSet m_ifaceAssocSet;
      AssociationSet m_associationSet;

      Associations m_associations;

    public:
      molsrState()
      {
      }

      /**
       * Gets the MPR selectors.
       * \returns The MPR selectors.
       */
      const MprSelectorSet &GetMprSelectors() const
      {
        return m_mprSelectorSet;
      }

      MprSelectorTuple *FindMprSelectorTuple(const Ipv4Address &mainAddr);

      // erases a mpr selector tuple
      void EraseMprSelectorTuple(const MprSelectorTuple &tuple);

      // erase all mpr selector tuples belonging to the same address
      void EraseMprSelectorTuples(const Ipv4Address &mainAddr);

      // inserts a mpr selector tuple
      void InsertMprSelectorTuple(const MprSelectorTuple &tuple);

      // prints MPR selector sets
      std::string PrintMprSelectorSet() const;

      NeighborSet &GetNeighbors()
      {
        return m_neighborSet;
      }

      // Find a neighbor tuple with neighbor address
      NeighborTuple *FindNeighborTuple(const Ipv4Address &mainAddr);

      const NeighborTuple *FindSymNeighborTuple(const Ipv4Address &mainAddr) const;

      NeighborTuple *FindNeighborTuple(const Ipv4Address &mainAddr, uint8_t willingness);

      void EraseNeighborTuple(const NeighborTuple &neighborTuple);

      void EraseNeighborTuple(const Ipv4Address &mainAddr);

      void InsertNeighborTuple(const NeighborTuple &tuple);

      const TwoHopNeighborSet &GetTwoHopNeighbors() const
      {
        return m_twoHopNeighborSet;
      }

      // get a mutuable reference
      TwoHopNeighborSet &GetTwoHopNeighbors()
      {
        return m_twoHopNeighborSet;
      }

      TwoHopNeighborTuple *FindTwoHopNeighborTuple(const Ipv4Address &neighbor,
                                                   const Ipv4Address &twoHopNeighbor);

      void EraseTwoHopNeighborTuple(const TwoHopNeighborTuple &tuple);

      // erase the 2-hop neighbor tuples with the same 1-hop neighbor
      void EraseTwoHopNeighborTuples(const Ipv4Address &neighbor);

      void EraseTwoHopNeighborTuples(const Ipv4Address &neighbor, const Ipv4Address &twoHopNeighbor);
      void InsertTwoHopNeighborTuple(const TwoHopNeighborTuple &tuple);

      // check if there is a MPR with a specific address
      bool FindMprAddress(const Ipv4Address &address);

      void SetMprSet(MprSet mprSet);

      MprSet GetMprSet() const;

      DuplicateTuple *FindDuplicateTuple(const Ipv4Address &address, uint16_t sequenceNumber);
      void EraseDuplicateTuple(const DuplicateTuple &tuple);
      void InsertDuplicateTuple(const DuplicateTuple &tuple);

      const LinkSet &GetLinks() const
      {
        return m_linkSet;
      }

      // find a link tuple with given interface address
      LinkTuple *FindLinkTuple(const Ipv4Address &ifaceAddr);

      LinkTuple *FindSymLinkTuple(const Ipv4Address &ifaceAddr, Time time);
      void EraseLinkTuple(const LinkTuple &tuple);
      LinkTuple &InsertLinkTuple(const LinkTuple &tuple);
      const TopologySet &GetTopologySet() const
      {
        return m_topologySet;
      }
      TopologyTuple *FindTopologyTuple(const Ipv4Address &destAddr, const Ipv4Address &lastAddr);

      // find any newer topology tuple, that can be reached from lastAddr
      // which is basically toplogy tuples with sequqnce number greater than ansn
      TopologyTuple *FindNewerTopologyTuple(const Ipv4Address &lastAddr, uint16_t ansn);

      void EraseTopologyTuple(const TopologyTuple &tuple);

      // erase all topology tuples reacheable from the lastAddr address
      // which is tuples having sequqnce number less than ansn
      void EraseOlderTopologyTuples(const Ipv4Address &lastAddr, uint16_t ansn);
      void InsertTopologyTuple(const TopologyTuple &tuple);

      const IfaceAssocSet &GetIfaceAssocSet() const
      {
        return m_ifaceAssocSet;
      }

      // get a mutable reference
      IfaceAssocSet &GetIfaceAssocSetMutable()
      {
        return m_ifaceAssocSet;
      }
      IfaceAssocTuple *FindIfaceAssocTuple(const Ipv4Address &ifaceAddr);
      const IfaceAssocTuple *FindIfaceAssocTuple(const Ipv4Address &ifaceAddr) const;
      void EraseIfaceAssocTuple(const IfaceAssocTuple &tuple);
      void InsertIfaceAssocTuple(const IfaceAssocTuple &tuple);

      const AssociationSet &GetAssociationSet() const // Associations known to the node
      {
        return m_associationSet;
      }
      const Associations &GetAssociations() const // Set of associations that the node has
      {
        return m_associations;
      }
      AssociationTuple *FindAssociationTuple(const Ipv4Address &gatewayAddr,
                                             const Ipv4Address &networkAddr,
                                             const Ipv4Mask &netmask);

      void EraseAssociationTuple(const AssociationTuple &tuple);
      void InsertAssociationTuple(const AssociationTuple &tuple);
      void EraseAssociation(const Association &tuple);
      void InsertAssociation(const Association &tuple);

      //return all interfaces of neighbor exception the main one
      std::vector<Ipv4Address> FindNeighborInterfaces(const Ipv4Address &neighborMainAddr) const;
    };
  }
}

#endif