
#include "VanetNode.h"

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("VanetNode");

  VanetNode::VanetNode()
  {
    // initially unclustered
    NS_LOG_INFO("Hello from the Vanet Node constructor");
    vtype = UNCLUSTERED;
  }
} // namespace ns3