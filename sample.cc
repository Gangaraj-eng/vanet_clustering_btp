#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/FanetNode.h"
#include "ns3/RSUNode.h"
#include "ns3/VanetNode.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("SampleUsage");

int main()
{
  LogComponentEnable("FanetNode", LOG_LEVEL_INFO);
  LogComponentEnable("RSUNode", LOG_LEVEL_INFO);
  LogComponentEnable("VanetNode", LOG_LEVEL_INFO);
  Ptr<FanetNode> f = CreateObject<FanetNode>();
  Ptr<VanetNode> v = CreateObject<VanetNode>();

  // adding our custom nodes to node container
  NodeContainer c;
  c.Add(v);
  c.Add(f);

  std::cout<<"Total nodes = "<<c.GetN()<<std::endl;

  Simulator::Run();
  Simulator::Destroy();
  return 0;
}