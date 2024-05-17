#include "VanetApplicationHelper.h"

#include "ns3/names.h"
#include "ns3/string.h"

namespace ns3
{
  namespace btp
  {
    VanetApplicationHelper::VanetApplicationHelper()
    {
      m_factory.SetTypeId("ns3::VanetApplication");
    }
    VanetApplicationHelper::VanetApplicationHelper(std::string protocol, Address address)
    {
    }

    ApplicationContainer VanetApplicationHelper::Install(Ptr<Node> node) const
    {
      return ApplicationContainer(InstallPriv(node));
    }

    ApplicationContainer VanetApplicationHelper::Install(std::string nodeName) const
    {
      Ptr<Node> node = Names::Find<Node>(nodeName);
      return ApplicationContainer(InstallPriv(node));
    }

    ApplicationContainer VanetApplicationHelper::Install(NodeContainer c) const
    {
      ApplicationContainer apps;
      for (auto i = c.Begin(); i != c.End(); ++i)
      {
        apps.Add(InstallPriv(*i));
      }
      return apps;
    }

    Ptr<Application> VanetApplicationHelper::InstallPriv(Ptr<Node> node) const
    {
      Ptr<Application> app = m_factory.Create<Application>();
      node->AddApplication(app);
      return app;
    }

    void
    VanetApplicationHelper::SetAttribute(std::string name, const AttributeValue &value)
    {
      m_factory.Set(name, value);
    }

    int64_t
    VanetApplicationHelper::AssignStreams(NodeContainer c, int64_t stream)
    {
      return 0;
    }
  } // namespace btp
} // namespace ns3
