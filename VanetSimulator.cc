#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/VanetNode.h"
#include "ns3/FanetNode.h"
#include "ns3/ClusterRoutingHelper.h"
#include "ns3/VanetNodeApplicationHelper.h"
#include "ns3/molsr-helper.h"
#include "ns3/UADCConstants.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("VanetSimulator");

class VanetSimulator
{
public:
  VanetSimulator();
  void Run();

private:
  int nVanetNodes;
  int nfanetNodes;
  NodeContainer vanetNodes;
  NodeContainer fanetNodes;
  NetDeviceContainer vanetDevices;
  NetDeviceContainer fanetDevices;
  std::vector<Vector> fanetPositions;
  double simulationTime; // in seconds
  Ipv4InterfaceContainer interfaces;
  SimulationParamters mParameters;

private:
  void CreateNodes();
  void InstallMobilityModel();
  void InstallFanetMobilityModel();
  void InstallEnergyModel(); // TODO
  void CreateVanetDevices();
  void CreateUAVDevices();
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
  nVanetNodes = mParameters.numVanetNodes;
  nfanetNodes = mParameters.numFanetNodes;
  simulationTime = mParameters.TotalSimulationTime;
}

void VanetSimulator::Run()
{
  CreateNodes();
  CreateVanetDevices();
  CreateUAVDevices();
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

void VanetSimulator::ConfigureAnimation(AnimationInterface &anim)
{
  int vehicleNodeIconId = anim.AddResource(VANETNODE_ICON_PATH);
  // int rsuNodeIconId = anim.AddResource(RSU_ICON_PATH);
  int uavNodeIconId = anim.AddResource(UAV_ICON_PATH);
  for (int i = 0; i < nVanetNodes; i++)
  {
    anim.UpdateNodeImage(vanetNodes.Get(i)->GetId(), vehicleNodeIconId);
    anim.UpdateNodeSize(vanetNodes.Get(i)->GetId(), 30, 30);
  }
  for (int i = 0; i < nfanetNodes; i++)
  {
    anim.UpdateNodeImage(fanetNodes.Get(i)->GetId(), uavNodeIconId);
    anim.SetConstantPosition(fanetNodes.Get(i), fanetPositions[i].x, fanetPositions[i].y, fanetPositions[i].z);
    anim.UpdateNodeSize(fanetNodes.Get(i)->GetId(), 70, 70);
  }
}

void VanetSimulator::CreateNodes()
{
  // Create nodes and name them
  for (int i = 0; i < nVanetNodes; i++)
  {
    Ptr<VanetNode> node = CreateObject<VanetNode>();
    node->SetNodeId(i);
    // std::string nodeId = "vnode" + std::to_string(i + 1);
    // Names::Add(nodeId, vanetNodes.Get(i));
    vanetNodes.Add(node);
  }

  for (int i = 0; i < nfanetNodes; i++)
  {
    Ptr<FanetNode> node = CreateObject<FanetNode>();
    fanetNodes.Add(node);
  }

  InstallMobilityModel();
  InstallFanetMobilityModel();
}

void VanetSimulator::InstallMobilityModel()
{
  MobilityHelper mobility;
  ObjectFactory pos;
  pos.SetTypeId("ns3::RandomBoxPositionAllocator");
  std::stringstream ssDimX;
  ssDimX << "ns3::UniformRandomVariable[Min=0.0|Max=" << mParameters.simulationDimension << "]";
  pos.Set("X", StringValue(ssDimX.str()));
  pos.Set("Y", StringValue(ssDimX.str()));
  // we need antenna height uniform [1.0 .. 2.0] for loss model
  // pos.Set("Z", StringValue("ns3::UniformRandomVariable[Min=1.0|Max=2.0]"));

  Ptr<PositionAllocator> positionAllocator = pos.Create()->GetObject<PositionAllocator>();
  mobility.SetPositionAllocator(positionAllocator);
  std::stringstream ssSpeed;
  ssSpeed << "ns3::UniformRandomVariable[Min=0.0|Max=" << 30 << "]";
  std::stringstream ssPause;
  ssPause << "ns3::ConstantRandomVariable[Constant=" << 5 << "]";
  mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                            "Speed",
                            StringValue(ssSpeed.str()),
                            "Pause",
                            StringValue(ssPause.str()),
                            "PositionAllocator",
                            PointerValue(positionAllocator));

  mobility.Install(vanetNodes);
}

// place UAVs uniformly in the grid
void VanetSimulator::InstallFanetMobilityModel()
{
  int numNodes = nfanetNodes;
  double gridSize = mParameters.simulationDimension;
  double spacing = gridSize / 3.0;

  // Define node positions based on grid coordinates
  Ptr<ListPositionAllocator> positionAlloc_n = CreateObject<ListPositionAllocator>();

  fanetPositions.resize(numNodes);
  for (int i = 0; i < numNodes; ++i)
  {
    uint32_t row = i / 3;
    uint32_t col = i % 3;
    double x = col * spacing + spacing / 2;
    double y = row * spacing + spacing / 2;
    fanetPositions[i] = Vector(x, y, 100.0);
    positionAlloc_n->Add(fanetPositions[i]);
  }

  // Create and set constant position mobility model for each node
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(positionAlloc_n);
  mobility.Install(fanetNodes);
}

void VanetSimulator::InstallEnergyModel()
{
}

void VanetSimulator::CreateVanetDevices()
{

  // creating wifi devices
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac");

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
  wifiPhy.SetChannel(wifiChannel.Create());

  wifiPhy.Set("TxPowerStart", DoubleValue(15.0 * log10(mParameters.VanetNodeRange)));
  wifiPhy.Set("TxPowerEnd", DoubleValue(15.0 * log10(mParameters.VanetNodeRange)));
  WifiHelper wifi;
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode",
                               StringValue("OfdmRate6Mbps"),
                               "RtsCtsThreshold",
                               UintegerValue(0));
  vanetDevices = wifi.Install(wifiPhy, wifiMac, vanetNodes);
}

void VanetSimulator::CreateUAVDevices()
{

  // creating wifi devices
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac");

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
  wifiPhy.SetChannel(wifiChannel.Create());

  wifiPhy.Set("TxPowerStart", DoubleValue(15.0 * log10(mParameters.FanetNodeRange)));
  wifiPhy.Set("TxPowerEnd", DoubleValue(15.0 * log10(mParameters.FanetNodeRange)));
  WifiHelper wifi;
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode",
                               StringValue("OfdmRate6Mbps"),
                               "RtsCtsThreshold",
                               UintegerValue(0));
  fanetDevices = wifi.Install(wifiPhy, wifiMac, fanetNodes);
}

// Set your routing protocol in internet stack
void VanetSimulator::InstallInternetStack()
{
  ClusterRoutingHelper crhelper;
  MolsrHelper mhelper;
  LogComponentEnable("ClusterRoutingProtocol", LOG_LEVEL_INFO);
  LogComponentEnable("ClusterPacketHeader", LOG_LEVEL_INFO);
  LogComponentEnable("MolsrRoutingProtocol", LOG_LEVEL_ERROR);
  InternetStackHelper stack;
  // stack.SetRoutingHelper(crhelper);
  // stack.SetRoutingHelper(mhelper);
  // stack.Install(vanetNodes);
  stack.SetRoutingHelper(mhelper);
  stack.Install(fanetNodes);
  Ipv4AddressHelper address;
  // NetDeviceContainer combined;
  // combined.Add(fanetDevices);
  // combined.Add(vanetDevices);
  address.SetBase("10.0.0.0", "255.0.0.0");
  interfaces = address.Assign(fanetDevices);
}

// set your application
void VanetSimulator::InstallApplications()
{
  VanetNodeApplicationHelper vnodeapphelper;
  LogComponentEnable("VanetNodeApplication", LOG_LEVEL_INFO);
  // ApplicationContainer ac = vnodeapphelper.Install(vanetNodes);
  ApplicationContainer ac2 = vnodeapphelper.Install(fanetNodes);
  ac2.Start(Seconds(0));
  ac2.Stop(Seconds(10));
}
