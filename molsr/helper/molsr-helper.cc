#include "molsr-helper.h"
#include "ns3/ipv4-list-routing.h"
#include "ns3/names.h"
#include "ns3/node-list.h"
#include "ns3/olsr-routing-protocol.h"
#include "ns3/ptr.h"

namespace ns3
{
  namespace molsr
  {

    MolsrHelper::MolsrHelper()
    {
      m_agentFactory.SetTypeId("ns3::molsr::RoutingProtocol");
    }

    MolsrHelper::MolsrHelper(const MolsrHelper &o)
        : m_agentFactory(o.m_agentFactory)
    {
      m_interfaceMap = o.m_interfaceMap;
    }

    MolsrHelper *MolsrHelper::Copy() const
    {
      return new MolsrHelper(*this);
    }

    void MolsrHelper::SetInterface(Ptr<Node> node, uint32_t interface)
    {
      m_interfaceMap.insert({node, interface});
    }
    
    
    Ptr<Ipv4RoutingProtocol>
    MolsrHelper::Create(Ptr<Node> node) const{
        
    }

    void MolsrHelper::Set(std::string name, const AttributeValue &value){
      m_agentFactory.Set(name, value);
    }
  } // namespace molsr

} // namespace ns3
