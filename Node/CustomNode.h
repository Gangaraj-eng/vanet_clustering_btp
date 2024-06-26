#ifndef CUSTOM_NODE_H
#define CUSTOM_NODE_H

#include "ns3/node.h"
#include "ns3/VanetRoutingRepository.h"

namespace ns3
{
  namespace btp
  {
      class CustomNode:public Node{
          public:
            CustomNode();
            NodeType mnodeType;
      };
  } // namespace btp

} // namespace ns3

#endif