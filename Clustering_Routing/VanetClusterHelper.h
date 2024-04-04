#ifndef VANET_CLUSTER_HELPER_H
#define VANET_CLUSTER_HELPER_H
#include "ns3/ipv4-routing-helper.h"
#include "VanetClusterRoutingProtocol.h"

namespace ns3
{
    namespace vcr
    {

        class VanetClusterHelper : public Ipv4RoutingHelper
        {
            VanetClusterHelper();
            VanetClusterHelper(const VanetClusterHelper &o);
            VanetClusterHelper *Copy() const override;
            VanetClusterHelper &operator=(const VanetClusterHelper &) = delete;
            void Set(std::string name, const AttributeValue &value);


            Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const;

        private:
            // used for object creation and holding
            ObjectFactory m_agentFactory;
        };

    } // namespace vcr

} // namespace ns3

#endif