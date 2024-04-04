/*
 * AUTHOR - Gangaraju Bopparm (202001107@daiict.ac.in)
 * VanetClusterHelper
 * Helper class to implement clustering routing protocol in VANETs
 * This Header file contains the definition for VanetClusterHelper
 */

#ifndef VANET_CLUSTER_HELPER_H
#define VANET_CLUSTER_HELPER_H

#include "ns3/ipv4-routing-helper.h"
#include "ns3/node-container.h"
#include "ns3/node.h"
#include "ns3/object-factory.h"

#include <map>

// Namespace to be used for my project
namespace ns3
{

    /**
     * \ingroup Vanet Clustering Routing Protocol
     * \brief Helper class that adds clustering routing protocol to VANET nodes
     *
     *  It should be used in conjuction with
     *  ns3::InternetStackHelper::SetRoutingHelper
     */

    class VanetClusterRoutingHelper : public Ipv4RoutingHelper
    {
    public:
        // Default Constructor
        VanetClusterRoutingHelper();

        // Copy Constructor
        VanetClusterRoutingHelper(const VanetClusterRoutingHelper &v);

        // Deleting assignment operator to avoid misuse
        VanetClusterRoutingHelper &operator=(const VanetClusterRoutingHelper &) = delete;

        VanetClusterRoutingHelper *Copy() const override;

        // To set an interface of the node to run Clustering
        void SetInterfaceToUse(Ptr<Node> node, uint32_t interface);

        /**
         * \param node the node on which the routing protocol will run
         * \returns a newly-created routing protocol
         *
         * This method will be called by ns3::InternetStackHelper::Install
         */
        Ptr<Ipv4RoutingProtocol> Create(Ptr<Node> node) const override;
        // To set an attribute
        void Set(std::string name, const AttributeValue &value);

    private:
        // Object Factory holds a set of attributes
        ObjectFactory m_agentFactory;

        // logic: Multiple interfaces exist for a device and different protocols
        // can run in different interfaces
        // This map stores the interface for a node on which to run clusterin routing protocol
        std::map<Ptr<Node>, uint32_t> m_interfaceMap;
    };

} // namespace ns3

#endif /* VANET_CLUSTER_HELPER_H */