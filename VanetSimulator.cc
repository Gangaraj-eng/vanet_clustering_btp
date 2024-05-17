#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/yans-wifi-helper.h"

// our header files
#include "ns3/SimulationParameters.h"
#include "ns3/VanetApplicationHelper.h"
#include "ns3/VanetRoutingHelper.h"

/**
 * Create and Run Vanet
 */
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("VanetSimulator");

// Helper class for Vanet Simulation
class VanetSimulator
{
public:
  VanetSimulator();
  ~VanetSimulator();
  void Run();

  // create nodes
  void CreateNodes();
  void InstallMobilityModel();
  void CreateNetDevices();
  void InstallEnergyModel();
  void InstallInternetStack();
  void InstallApplication();

  // Node specific mobilities
  void InstallVanetMobility();
  void InstallFanetMobility();
  void InstallRsuMobility();

  // Node specific devices
  void CreateCrDevices();
  void CreateOlsrDevices();
  void CreateQrDevices();

  void ConfigureAnimation(AnimationInterface &anim);

private:
  NodeContainer vanetNodes;
  NodeContainer fanetNodes;
  NodeContainer rsuNodes;

  NetDeviceContainer olsrDevices;
  NetDeviceContainer qrDevices;
  NetDeviceContainer crDevices;

  int m_numVanetNodes;
  int m_numFanetNodes;
  int m_numRsuNodes;

  Time totalSimulationTime;

  // to store the constant node positions
  std::vector<Vector> m_fanetPositions;
  std::vector<Vector> m_rsuPositions;
};

int main(int argc, char *argv[])
{
  VanetSimulator vanet;
  vanet.Run();
  return 0;
}

VanetSimulator::VanetSimulator()
{

  m_numVanetNodes = NUM_VANET_NODES;
  m_numFanetNodes = NUM_FANET_NODES;
  m_numRsuNodes = NUM_RSU_NODES;

  totalSimulationTime = Seconds(TOTAL_SIMULATION_TIME);
}

VanetSimulator::~VanetSimulator()
{
}

void VanetSimulator::Run()
{

  CreateNodes();
  CreateNetDevices();
  InstallInternetStack();
  InstallApplication();

  AnimationInterface anim("VanetSimulator.xml");
  ConfigureAnimation(anim);

  Simulator::Stop(totalSimulationTime);
  Simulator::Run();
  Simulator::Destroy();
}

void VanetSimulator::CreateNodes()
{
  vanetNodes.Create(m_numVanetNodes);
  fanetNodes.Create(m_numFanetNodes);
  rsuNodes.Create(m_numRsuNodes);
  InstallMobilityModel();
}

void VanetSimulator::InstallMobilityModel()
{
  InstallVanetMobility();
  InstallFanetMobility();
  InstallRsuMobility();
}

void VanetSimulator::CreateNetDevices()
{
  CreateCrDevices();
  CreateOlsrDevices();
  CreateQrDevices();
  // std::cout<<"Num cR Devices "<<crDevices.GetN()<<std::endl;
  // std::cout<<"Num OLSR Devices "<<olsrDevices.GetN()<<std::endl;
  // std::cout<<"Num QR Devices "<<qrDevices.GetN()<<std::endl;
}

void VanetSimulator::InstallEnergyModel()
{
}

void VanetSimulator::InstallInternetStack()
{

  LogComponentEnable("VanetRoutingProtocol",LOG_LEVEL_INFO);

  InternetStackHelper stack;
  btp::VanetRoutingHelper vanetRouting;
  Ipv4AddressHelper addressHelper;
  stack.SetRoutingHelper(vanetRouting);

  stack.Install(vanetNodes);
  stack.Install(fanetNodes);
  stack.Install(rsuNodes);

  addressHelper.SetBase("10.0.0.0", "255.255.0.0");
  addressHelper.Assign(crDevices);
  addressHelper.SetBase("10.1.0.0", "255.255.0.0");
  addressHelper.Assign(olsrDevices);
  addressHelper.SetBase("10.2.0.0", "255.255.0.0");
  addressHelper.Assign(qrDevices);
}

void VanetSimulator::InstallApplication()
{
  btp::VanetApplicationHelper vanetApp;
  ApplicationContainer app = vanetApp.Install(vanetNodes);
  app.Start(Seconds(VANET_APP_START_TIME));
  app.Stop(totalSimulationTime);
}

void VanetSimulator::InstallVanetMobility()
{
  if (m_numVanetNodes == 0)
  {
    return;
  }

  MobilityHelper mobility;
  ObjectFactory pos;
  pos.SetTypeId("ns3::RandomBoxPositionAllocator");
  std::stringstream ssDim;
  ssDim << "ns3::UniformRandomVariable[Min=0.0|Max=" << AREA_LENGTH << "]";
  pos.Set("X", StringValue(ssDim.str()));
  pos.Set("Y", StringValue(ssDim.str()));

  Ptr<PositionAllocator> positionAllocator = pos.Create()->GetObject<PositionAllocator>();
  mobility.SetPositionAllocator(positionAllocator);

  std::stringstream ssSpeed;
  ssSpeed << "ns3::UniformRandomVariable[Min=" << VEHICLE_MIN_SPEED << "|Max=" << VEHICLE_MAX_SPEED << "]";
  std::stringstream ssPause;
  ssPause << "ns3::ConstantRandomVariable[Constant=" << VEHICLE_PAUSE_TIME << "]";
  mobility.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                            "Speed",
                            StringValue(ssSpeed.str()),
                            "Pause",
                            StringValue(ssPause.str()),
                            "PositionAllocator",
                            PointerValue(positionAllocator));

  mobility.Install(vanetNodes);
}

void VanetSimulator::InstallFanetMobility()
{
  if (m_numFanetNodes == 0)
  {
    return;
  }

  double spacing = AREA_LENGTH/4;

  m_fanetPositions.resize(m_numFanetNodes);

  Ptr<ListPositionAllocator> m_listPositionAlloc = CreateObject<ListPositionAllocator>();
  for (int i = 0; i < m_numFanetNodes; i++)
  {
    Vector pos;
    int row=i/3,col=i%3;
    pos.x = (row+1)*spacing;
    pos.y = (col+1)*spacing;
    pos.z = UAV_ALTITUDE;
    m_fanetPositions[i] = pos;
    m_listPositionAlloc->Add(pos);
  }

  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(m_listPositionAlloc);
  mobility.Install(fanetNodes);
}

void VanetSimulator::InstallRsuMobility()
{
  if (m_numRsuNodes == 0)
  {
    return;
  }

  double spacing = AREA_LENGTH / sqrt(m_numRsuNodes);

  m_rsuPositions.resize(m_numRsuNodes);
  Ptr<ListPositionAllocator> m_listPositionAlloc = CreateObject<ListPositionAllocator>();
  for (int i = 0; i < m_numRsuNodes; i++)
  {
    Vector pos;
    int row = i / (int)sqrt(m_numRsuNodes);
    int col = i % (int)sqrt(m_numRsuNodes);
    pos.x = row*spacing + spacing / 2;
    pos.y = col*spacing + spacing / 2;
    pos.z = 0;
    m_rsuPositions[i] = pos;
    m_listPositionAlloc->Add(pos);
  }

  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(m_listPositionAlloc);
  mobility.Install(rsuNodes);
}

void VanetSimulator::CreateCrDevices()
{
  // creating wifi devices
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac");

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
  wifiPhy.SetChannel(wifiChannel.Create());

  wifiPhy.Set("TxPowerStart", DoubleValue(15.0 * log10(VANET_RANGE)));
  wifiPhy.Set("TxPowerEnd", DoubleValue(15.0 * log10(VANET_RANGE)));
  WifiHelper wifi;
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode",
                               StringValue("OfdmRate6Mbps"),
                               "RtsCtsThreshold",
                               UintegerValue(0));
  crDevices = wifi.Install(wifiPhy, wifiMac, vanetNodes);
}

void VanetSimulator::CreateOlsrDevices()
{
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac");

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
  wifiPhy.SetChannel(wifiChannel.Create());

  wifiPhy.Set("TxPowerStart", DoubleValue(15.0 * log10(UAV_RANGE)));
  wifiPhy.Set("TxPowerEnd", DoubleValue(15.0 * log10(UAV_RANGE)));
  WifiHelper wifi;
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode",
                               StringValue("OfdmRate6Mbps"),
                               "RtsCtsThreshold",
                               UintegerValue(0));
  olsrDevices.Add(wifi.Install(wifiPhy, wifiMac, fanetNodes));

  wifiPhy.Set("TxPowerStart", DoubleValue(15.0 * log10(VANET_RANGE)));
  wifiPhy.Set("TxPowerEnd", DoubleValue(15.0 * log10(VANET_RANGE)));

  olsrDevices.Add(wifi.Install(wifiPhy, wifiMac, vanetNodes));
}

void VanetSimulator::CreateQrDevices()
{
  // creating wifi devices
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac");

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
  wifiPhy.SetChannel(wifiChannel.Create());

  wifiPhy.Set("TxPowerStart", DoubleValue(15.0 * log10(RSU_RANGE)));
  wifiPhy.Set("TxPowerEnd", DoubleValue(15.0 * log10(RSU_RANGE)));
  WifiHelper wifi;
  wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode",
                               StringValue("OfdmRate6Mbps"),
                               "RtsCtsThreshold",
                               UintegerValue(0));
  qrDevices.Add(wifi.Install(wifiPhy, wifiMac, rsuNodes));

  wifiPhy.Set("TxPowerStart", DoubleValue(15.0 * log10(VANET_RANGE)));
  wifiPhy.Set("TxPowerEnd", DoubleValue(15.0 * log10(VANET_RANGE)));

  qrDevices.Add(wifi.Install(wifiPhy, wifiMac, vanetNodes));
}

void VanetSimulator::ConfigureAnimation(AnimationInterface &anim)
{
  int vehicleIconId = anim.AddResource(VANETNODE_ICON_PATH);
  int UavIconId = anim.AddResource(UAV_ICON_PATH);
  int rsuIconId = anim.AddResource(RSU_ICON_PATH);

  // setting icons
  for (int i = 0; i < m_numVanetNodes; i++)
  {
    anim.UpdateNodeImage(vanetNodes.Get(i)->GetId(), vehicleIconId);
    anim.UpdateNodeSize(vanetNodes.Get(i)->GetId(), 30, 30);
  }

  for (int i = 0; i < m_numFanetNodes; i++)
  {
    anim.UpdateNodeImage(fanetNodes.Get(i)->GetId(), UavIconId);
    anim.UpdateNodeSize(fanetNodes.Get(i)->GetId(), 60, 60);
    anim.SetConstantPosition(fanetNodes.Get(i), m_fanetPositions[i].x, m_fanetPositions[i].y, m_fanetPositions[i].z);
  }

  for (int i = 0; i < m_numRsuNodes; i++)
  {
    anim.UpdateNodeImage(rsuNodes.Get(i)->GetId(), rsuIconId);
    anim.UpdateNodeSize(rsuNodes.Get(i)->GetId(), 40, 40);
    anim.SetConstantPosition(rsuNodes.Get(i), m_rsuPositions[i].x, m_rsuPositions[i].y, m_rsuPositions[i].z);
  }
}
