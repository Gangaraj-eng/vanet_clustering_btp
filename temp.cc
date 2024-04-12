#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("temp");

int main()
{
  NodeContainer nc;
  nc.Create(2);
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;
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
  devices = wifi.Install(wifiPhy, wifiMac, nc);
  Ptr<Socket> s1 = Socket::CreateSocket(nc.Get(0), UdpSocketFactory::GetTypeId());
  Ptr<Socket> s2 = Socket::CreateSocket(nc.Get(1), UdpSocketFactory::GetTypeId());
  const int port = 12345;

  InternetStackHelper stack;
  stack.Install(nc);
  Ipv4AddressHelper address;
  address.SetBase("10.0.0.0", "255.0.0.0");
  interfaces = address.Assign(devices);

  InetSocketAddress inetAddr1("10.0.0.1", port);
  InetSocketAddress inetAddr2("10.0.0.2", port);

  s1->BindToNetDevice(devices.Get(0));
  s1->SetAllowBroadcast(true);

  s1->Bind(inetAddr1);
  s2->Bind(inetAddr2);
  s2->SetAllowBroadcast(true);
  s2->BindToNetDevice(devices.Get(1));
  
  return 0;
}