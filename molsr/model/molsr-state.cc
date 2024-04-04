
#include "molsr-state.h"

namespace ns3
{
  namespace molsr
  {
    MprSelectorTuple *molsrState::FindMprSelectorTuple(const Ipv4Address &mainAddr)
    {
      for (MprSelectorSet::iterator it = m_mprSelectorSet.begin(); it != m_mprSelectorSet.end(); it++)
      {
        if (it->mainAddr == mainAddr)
        {
          return &(*it);
        }
      }
    }

    void
    molsrState::EraseMprSelectorTuple(const MprSelectorTuple &tuple)
    {
      for (MprSelectorSet::iterator it = m_mprSelectorSet.begin(); it != m_mprSelectorSet.end(); it++)
      {
        if (*it == tuple)
        {
          m_mprSelectorSet.erase(it);
          break;
        }
      }
    }

    void
    molsrState::EraseMprSelectorTuples(const Ipv4Address &mainAddr)
    {
      for (MprSelectorSet::iterator it = m_mprSelectorSet.begin(); it != m_mprSelectorSet.end();)
      {
        if (it->mainAddr == mainAddr)
        {
          it = m_mprSelectorSet.erase(it);
        }
        else
        {
          it++;
        }
      }
    }

    void
    molsrState::InsertMprSelectorTuple(const MprSelectorTuple &tuple)
    {
      m_mprSelectorSet.push_back(tuple);
    }

    std::string
    molsrState::PrintMprSelectorSet() const
    {
      std::ostringstream os;
      os << "[";
      for (MprSelectorSet::const_iterator iter = m_mprSelectorSet.begin();
           iter != m_mprSelectorSet.end();
           iter++)
      {
        MprSelectorSet::const_iterator next = iter;
        next++;
        os << iter->mainAddr;
        if (next != m_mprSelectorSet.end())
        {
          os << ", ";
        }
      }
      os << "]";
      return os.str();
    }

    NeighborTuple *
    molsrState::FindNeighborTuple(const Ipv4Address &mainAddr)
    {
      for (NeighborSet::iterator it = m_neighborSet.begin(); it != m_neighborSet.end(); it++)
      {
        if (it->neighborMainAddr == mainAddr)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    const NeighborTuple *
    molsrState::FindSymNeighborTuple(const Ipv4Address &mainAddr) const
    {
      for (NeighborSet::const_iterator it = m_neighborSet.begin(); it != m_neighborSet.end(); it++)
      {
        if (it->neighborMainAddr == mainAddr && it->status == NeighborTuple::STATUS_SYM)
        {
          return &(*it);
        }
      }
      return nullptr;
    }
    NeighborTuple *
    molsrState::FindNeighborTuple(const Ipv4Address &mainAddr, uint8_t willingness)
    {
      for (NeighborSet::iterator it = m_neighborSet.begin(); it != m_neighborSet.end(); it++)
      {
        if (it->neighborMainAddr == mainAddr && it->willingness == willingness)
        {
          return &(*it);
        }
      }
      return nullptr;
    }
    void
    molsrState::EraseNeighborTuple(const NeighborTuple &tuple)
    {
      for (NeighborSet::iterator it = m_neighborSet.begin(); it != m_neighborSet.end(); it++)
      {
        if (*it == tuple)
        {
          m_neighborSet.erase(it);
          break;
        }
      }
    }

    void
    molsrState::EraseNeighborTuple(const Ipv4Address &mainAddr)
    {
      for (NeighborSet::iterator it = m_neighborSet.begin(); it != m_neighborSet.end(); it++)
      {
        if (it->neighborMainAddr == mainAddr)
        {
          it = m_neighborSet.erase(it);
          break;
        }
      }
    }
    void
    molsrState::InsertNeighborTuple(const NeighborTuple &tuple)
    {
      for (NeighborSet::iterator it = m_neighborSet.begin(); it != m_neighborSet.end(); it++)
      {
        if (it->neighborMainAddr == tuple.neighborMainAddr)
        {
          // Update it
          *it = tuple;
          return;
        }
      }
      m_neighborSet.push_back(tuple);
    }
    TwoHopNeighborTuple *
    molsrState::FindTwoHopNeighborTuple(const Ipv4Address &neighborMainAddr,
                                        const Ipv4Address &twoHopNeighborAddr)
    {
      for (TwoHopNeighborSet::iterator it = m_twoHopNeighborSet.begin();
           it != m_twoHopNeighborSet.end();
           it++)
      {
        if (it->neighborMainAddr == neighborMainAddr &&
            it->twoHopNeighborAddr == twoHopNeighborAddr)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    void
    molsrState::EraseTwoHopNeighborTuple(const TwoHopNeighborTuple &tuple)
    {
      for (TwoHopNeighborSet::iterator it = m_twoHopNeighborSet.begin();
           it != m_twoHopNeighborSet.end();
           it++)
      {
        if (*it == tuple)
        {
          m_twoHopNeighborSet.erase(it);
          break;
        }
      }
    }

    void
    molsrState::EraseTwoHopNeighborTuples(const Ipv4Address &neighborMainAddr,
                                          const Ipv4Address &twoHopNeighborAddr)
    {
      for (TwoHopNeighborSet::iterator it = m_twoHopNeighborSet.begin();
           it != m_twoHopNeighborSet.end();)
      {
        if (it->neighborMainAddr == neighborMainAddr &&
            it->twoHopNeighborAddr == twoHopNeighborAddr)
        {
          it = m_twoHopNeighborSet.erase(it);
        }
        else
        {
          it++;
        }
      }
    }

    void
    molsrState::EraseTwoHopNeighborTuples(const Ipv4Address &neighborMainAddr)
    {
      for (TwoHopNeighborSet::iterator it = m_twoHopNeighborSet.begin();
           it != m_twoHopNeighborSet.end();)
      {
        if (it->neighborMainAddr == neighborMainAddr)
        {
          it = m_twoHopNeighborSet.erase(it);
        }
        else
        {
          it++;
        }
      }
    }

    void
    molsrState::InsertTwoHopNeighborTuple(const TwoHopNeighborTuple &tuple)
    {
      m_twoHopNeighborSet.push_back(tuple);
    }

    bool
    molsrState::FindMprAddress(const Ipv4Address &addr)
    {
      MprSet::iterator it = m_mprSet.find(addr);
      return (it != m_mprSet.end());
    }
    void
    molsrState::SetMprSet(MprSet mprSet)
    {
      m_mprSet = mprSet;
    }

    MprSet
    molsrState::GetMprSet() const
    {
      return m_mprSet;
    }

    DuplicateTuple *
    molsrState::FindDuplicateTuple(const Ipv4Address &addr, uint16_t sequenceNumber)
    {
      for (DuplicateSet::iterator it = m_duplicateSet.begin(); it != m_duplicateSet.end(); it++)
      {
        if (it->address == addr && it->sequenceNumber == sequenceNumber)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    void
    molsrState::EraseDuplicateTuple(const DuplicateTuple &tuple)
    {
      for (DuplicateSet::iterator it = m_duplicateSet.begin(); it != m_duplicateSet.end(); it++)
      {
        if (*it == tuple)
        {
          m_duplicateSet.erase(it);
          break;
        }
      }
    }

    void
    molsrState::InsertDuplicateTuple(const DuplicateTuple &tuple)
    {
      m_duplicateSet.push_back(tuple);
    }
    LinkTuple *
    molsrState::FindLinkTuple(const Ipv4Address &ifaceAddr)
    {
      for (LinkSet::iterator it = m_linkSet.begin(); it != m_linkSet.end(); it++)
      {
        if (it->neighborIfaceAddr == ifaceAddr)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    LinkTuple *
    molsrState::FindSymLinkTuple(const Ipv4Address &ifaceAddr, Time now)
    {
      for (LinkSet::iterator it = m_linkSet.begin(); it != m_linkSet.end(); it++)
      {
        if (it->neighborIfaceAddr == ifaceAddr)
        {
          if (it->symTime > now)
          {
            return &(*it);
          }
          else
          {
            break;
          }
        }
      }
      return nullptr;
    }

    void
    molsrState::EraseLinkTuple(const LinkTuple &tuple)
    {
      for (LinkSet::iterator it = m_linkSet.begin(); it != m_linkSet.end(); it++)
      {
        if (*it == tuple)
        {
          m_linkSet.erase(it);
          break;
        }
      }
    }

    LinkTuple &
    molsrState::InsertLinkTuple(const LinkTuple &tuple)
    {
      m_linkSet.push_back(tuple);
      return m_linkSet.back();
    }

    TopologyTuple *
    molsrState::FindTopologyTuple(const Ipv4Address &destAddr, const Ipv4Address &lastAddr)
    {
      for (TopologySet::iterator it = m_topologySet.begin(); it != m_topologySet.end(); it++)
      {
        if (it->destAddr == destAddr && it->lastAddr == lastAddr)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    TopologyTuple *
    molsrState::FindNewerTopologyTuple(const Ipv4Address &lastAddr, uint16_t ansn)
    {
      for (TopologySet::iterator it = m_topologySet.begin(); it != m_topologySet.end(); it++)
      {
        if (it->lastAddr == lastAddr && it->sequenceNumber > ansn)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    void
    molsrState::EraseTopologyTuple(const TopologyTuple &tuple)
    {
      for (TopologySet::iterator it = m_topologySet.begin(); it != m_topologySet.end(); it++)
      {
        if (*it == tuple)
        {
          m_topologySet.erase(it);
          break;
        }
      }
    }

    void
    molsrState::EraseOlderTopologyTuples(const Ipv4Address &lastAddr, uint16_t ansn)
    {
      for (TopologySet::iterator it = m_topologySet.begin(); it != m_topologySet.end();)
      {
        if (it->lastAddr == lastAddr && it->sequenceNumber < ansn)
        {
          it = m_topologySet.erase(it);
        }
        else
        {
          it++;
        }
      }
    }

    void
    molsrState::InsertTopologyTuple(const TopologyTuple &tuple)
    {
      m_topologySet.push_back(tuple);
    }

    IfaceAssocTuple *
    molsrState::FindIfaceAssocTuple(const Ipv4Address &ifaceAddr)
    {
      for (IfaceAssocSet::iterator it = m_ifaceAssocSet.begin(); it != m_ifaceAssocSet.end(); it++)
      {
        if (it->ifaceAddr == ifaceAddr)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    const IfaceAssocTuple *
    molsrState::FindIfaceAssocTuple(const Ipv4Address &ifaceAddr) const
    {
      for (IfaceAssocSet::const_iterator it = m_ifaceAssocSet.begin(); it != m_ifaceAssocSet.end();
           it++)
      {
        if (it->ifaceAddr == ifaceAddr)
        {
          return &(*it);
        }
      }
      return nullptr;
    }
    void
    molsrState::EraseIfaceAssocTuple(const IfaceAssocTuple &tuple)
    {
      for (IfaceAssocSet::iterator it = m_ifaceAssocSet.begin(); it != m_ifaceAssocSet.end(); it++)
      {
        if (*it == tuple)
        {
          m_ifaceAssocSet.erase(it);
          break;
        }
      }
    }

    void
    molsrState::InsertIfaceAssocTuple(const IfaceAssocTuple &tuple)
    {
      m_ifaceAssocSet.push_back(tuple);
    }

    std::vector<Ipv4Address>
    molsrState::FindNeighborInterfaces(const Ipv4Address &neighborMainAddr) const
    {
      std::vector<Ipv4Address> retval;
      for (IfaceAssocSet::const_iterator it = m_ifaceAssocSet.begin(); it != m_ifaceAssocSet.end();
           it++)
      {
        if (it->mainAddr == neighborMainAddr)
        {
          retval.push_back(it->ifaceAddr);
        }
      }
      return retval;
    }

    AssociationTuple *
    molsrState::FindAssociationTuple(const Ipv4Address &gatewayAddr,
                                     const Ipv4Address &networkAddr,
                                     const Ipv4Mask &netmask)
    {
      for (AssociationSet::iterator it = m_associationSet.begin(); it != m_associationSet.end(); it++)
      {
        if (it->gatewayAddr == gatewayAddr and it->networkAddr == networkAddr and
            it->netmask == netmask)
        {
          return &(*it);
        }
      }
      return nullptr;
    }

    void
    molsrState::EraseAssociationTuple(const AssociationTuple &tuple)
    {
      for (AssociationSet::iterator it = m_associationSet.begin(); it != m_associationSet.end(); it++)
      {
        if (*it == tuple)
        {
          m_associationSet.erase(it);
          break;
        }
      }
    }

    void
    molsrState::InsertAssociationTuple(const AssociationTuple &tuple)
    {
      m_associationSet.push_back(tuple);
    }

    void
    molsrState::EraseAssociation(const Association &tuple)
    {
      for (Associations::iterator it = m_associations.begin(); it != m_associations.end(); it++)
      {
        if (*it == tuple)
        {
          m_associations.erase(it);
          break;
        }
      }
    }

    void
    molsrState::InsertAssociation(const Association &tuple)
    {
      m_associations.push_back(tuple);
    }
  }
}