#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/VanetNode.h"
#include "ns3/ClusterRoutingHelper.h"
#include "ns3/VanetNodeApplicationHelper.h"
#include "ns3/molsr-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("VanetSimulator");

class VanetSimulator
{
public:
  VanetSimulator();
  void Run();

private:
  int nNodes;
  NodeContainer vanetNodes;
  NetDeviceContainer devices;
  double simulationTime; // in seconds
  Ipv4InterfaceContainer interfaces;

private:
  void CreateNodes();
  void InstallMobilityModel();
  void InstallEnergyModel(); // TODO
  void CreateDevices();
  void InstallInternetStack();
  void InstallApplications();
  void ConfigureAnimation(AnimationInterface &anim);
};

int main()
{
  VanetSimulator vanet;
  vanet.Run();
  return 0;
}

VanetSimulator::VanetSimulator()
{
  nNodes = 3;
  simulationTime = 30; // 30 seconds
}

void VanetSimulator::Run()
{
  CreateNodes();
  CreateDevices();
  InstallInternetStack();
  InstallApplications();

  std::cout << "Initialized VANET\n";
  // output file for NetAnim
  AnimationInterface anim("VanetSimulator.xml");
  ConfigureAnimation(anim);

  Simulator::Stop(Seconds(simulationTime));
  Simulator::Run();
  Simulator::Destroy();
}

void VanetSimulator::ConfigureAnimation(AnimationInterface &anim){
    int imgId = anim.AddResource("/home/raju1234/Downloads/v3.png");
    for(int i=0;i<nNodes;i++){
      anim.UpdateNodeImage(vanetNodes.Get(i)->GetId(),imgId);
      anim.UpdateNodeSize(vanetNodes.Get(i)->GetId(),50,50);
    }
}

void VanetSimulator::CreateNodes()
{

  // Create nodes and name them
  for (int i = 0; i < nNodes; i++)
  {
    vanetNodes.Add(CreateObject<VanetNode>());
    std::string nodeId = "vnode" + std::to_string(i + 1);
    Names::Add(nodeId, vanetNodes.Get(i));
  }
  InstallMobilityModel();
}

void VanetSimulator::InstallMobilityModel()
{
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

  mobility.Install(vanetNodes);
}

void VanetSimulator::InstallEnergyModel()
{
}

void VanetSimulator::CreateDevices()
{

  // creating wifi devices
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac");

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
  wifiPhy.SetChannel(wifiChannel.Create());

  WifiHelper wifi;
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode",
                               StringValue("OfdmRate6Mbps"),
                               "RtsCtsThreshold",
                               UintegerValue(0));
  devices = wifi.Install(wifiPhy, wifiMac, vanetNodes);
}

// Set your routing protocol in internet stack
void VanetSimulator::InstallInternetStack()
{
  ClusterRoutingHelper crhelper;
  MolsrHelper mhelper;
  LogComponentEnable("ClusterRoutingProtocol", LOG_LEVEL_INFO);
  LogComponentEnable("MolsrRoutingProtocol",LOG_LEVEL_ERROR);
  InternetStackHelper stack;
  stack.SetRoutingHelper(crhelper);
  stack.Install(vanetNodes);
  Ipv4AddressHelper address;
  address.SetBase("10.0.0.0", "255.0.0.0");
  interfaces = address.Assign(devices);
}

// set your application
void VanetSimulator::InstallApplications()
{
  VanetNodeApplicationHelper vnodeapphelper;
  LogComponentEnable("VanetNodeApplication", LOG_LEVEL_INFO);
  ApplicationContainer ac = vnodeapphelper.Install(vanetNodes);
  ac.Start(Seconds(0));
  ac.Stop(Seconds(10));
}
