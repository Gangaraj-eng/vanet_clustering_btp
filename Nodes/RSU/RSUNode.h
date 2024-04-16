#ifndef RSU_NODE_H
#define RSU_NODE_H

#include "ns3/node.h"
using namespace ns3;

namespace ns3
{
    class RSUNode : public Node
    {
    private:
        // map storing reputations of each node in VANET
        // key is node number or ID
        std::map<int, double> nodeReputations;

    public:
        RSUNode();
    };
}
#endif