#include "VanetNodeApplicationHelper.h"

#include "ns3/names.h"
#include "ns3/string.h"

namespace ns3
{

  VanetNodeApplicationHelper::VanetNodeApplicationHelper()
  {
    m_factory.SetTypeId("ns3::VanetNodeApplication");
  }
  VanetNodeApplicationHelper::VanetNodeApplicationHelper(std::string protocol, Address address)
  {
  }

  ApplicationContainer VanetNodeApplicationHelper::Install(Ptr<Node> node) const
  {
    return ApplicationContainer(InstallPriv(node));
  }

  ApplicationContainer VanetNodeApplicationHelper::Install(std::string nodeName) const
  {
    Ptr<Node> node = Names::Find<Node>(nodeName);
    return ApplicationContainer(InstallPriv(node));
  }

  ApplicationContainer VanetNodeApplicationHelper::Install(NodeContainer c) const
  {
    ApplicationContainer apps;
    for (auto i = c.Begin(); i != c.End(); ++i)
    {
      apps.Add(InstallPriv(*i));
    }
    return apps;
  }

  Ptr<Application> VanetNodeApplicationHelper::InstallPriv(Ptr<Node> node) const
  {
    Ptr<Application> app = m_factory.Create<Application>();
    node->AddApplication(app);
    return app;
  }

  void
  VanetNodeApplicationHelper::SetAttribute(std::string name, const AttributeValue &value)
  {
    m_factory.Set(name, value);
  }

  int64_t
  VanetNodeApplicationHelper::AssignStreams(NodeContainer c, int64_t stream)
  {
    return 0;
  }
} // namespace ns3
