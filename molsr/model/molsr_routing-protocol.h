/**
 * Author: Gangaraju Bopparam
 * Modified version of OLSR Routing protocol
 * This header file contains the method declarations for implementing modified
 * olsr routing protocol
 */

#ifndef MOLSR_AGENT_IMPL_H
#define MOLSR_AGENT_IMPL_H

#include "molsr_repositories.h"
#include "molsr-header.h"
#include "molsr-state.h"

#include "ns3/event-garbage-collector.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/ipv4-static-routing.h"
#include "ns3/ipv4.h"
#include "ns3/node.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/random-variable-stream.h"
#include "ns3/socket.h"
#include "ns3/test.h"
#include "ns3/timer.h"
#include "ns3/traced-callback.h"

#include <map>
#include <vector>

namespace ns3
{
  namespace molsr
  {

    struct RoutingTableEntry
    {
      Ipv4Address destAddr;
      Ipv4Address nextAddr;
      uint32_t interface;
      uint32_t distance;
      RoutingTableEntry() : destAddr(), nextAddr(), interface(0), distance(0)
      {
      }
    };

    class RoutingProtocol : public Ipv4RoutingProtocol
    {
    private: 
      std::set<uint32_t> m_interfaceMap;
      Ptr<Ipv4StaticRouting> m_routing;
    public:
      static const uint16_t MOLSR_PORT_NUMBER;
      RoutingProtocol();
      ~RoutingProtocol() override;
      
    };

  } // namespace molsr
} // namespace ns3

#endif