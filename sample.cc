#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/VanetNode.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("SampleUsage");

int main()
{
  int NNodes = 30;
  // adding our custom nodes to node container
  NodeContainer c;
  for (int i = 0; i < NNodes; i++)
  {
    c.Add(CreateObject<VanetNode>());
  }

  // std::cout<<"Total nodes = "<<c.GetN()<<std::endl;
  // std::cout<<(*v).vtype<<std::endl;
  MobilityHelper mobility;
  ObjectFactory pos;
  pos.SetTypeId("ns3::RandomDiscPositionAllocator");
  // X and Y are centers
  pos.Set("X", StringValue("100.0"));
  pos.Set("Y", StringValue("100.0"));
  // Rho is the radius
  pos.Set("Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=100]"));

  Ptr<PositionAllocator> positionAllocator = pos.Create()->GetObject<PositionAllocator>();
  mobility.SetPositionAllocator(positionAllocator);
  std::stringstream ssSpeed;
  ssSpeed << "ns3::UniformRandomVariable[Min=0.0|Max=" << 50 << "]";
  std::stringstream ssPause;
  ssPause << "ns3::ConstantRandomVariable[Constant=" << 0 << "]";
  mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                            "Speed",
                            StringValue(ssSpeed.str()),
                            "Pause",
                            StringValue(ssPause.str()),
                            "PositionAllocator",
                            PointerValue(positionAllocator));
  mobility.Install(c);
  AnimationInterface anim("Sample.xml");
  Simulator::Stop(Seconds(10.0));
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}