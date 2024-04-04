

#ifndef VANET_NODE_H
#define VANET_NODE_H

#include "ns3/node.h"

namespace ns3
{
    class VanetNode : public Node
    {
      enum NodeType
      {
        CLUSTER_HEAD,
        CLUSTER_MEMBER,
        UNCLUSTERED
      };

    public:
      VanetNode();
      NodeType vtype;
    };
}
#endif