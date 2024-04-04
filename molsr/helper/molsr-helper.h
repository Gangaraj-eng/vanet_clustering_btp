#ifndef MOLSR_HELPER_H
#define MOLSR_HELPER_H

#include "ns3/ipv4-routing-helper.h"
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/object-factory.h"

#include <map>
#include <set>

namespace ns3
{
  namespace molsr
  {

    class MolsrHelper : public Ipv4RoutingHelper
    {
    public:
      MolsrHelper();
      MolsrHelper(const MolsrHelper &o);
      MolsrHelper &operator=(const MolsrHelper &) = delete;
      MolsrHelper *Copy() const override;
      
      // interface on which olsr runs
      void SetInterface(Ptr<Node> node, uint32_t interface);
      Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const override;
      
      void Set(std::string name, const AttributeValue &value);
      int64_t AssignStreams(NodeContainer c, int64_t stream);

      private:
      ObjectFactory m_agentFactory;

      // contains node with its corresponding olsr interface
      std::map<Ptr<Node>, uint32_t> m_interfaceMap;
    };
  } // namespace molsr

} // namespace ns3

#endif