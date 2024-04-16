#ifndef SIMULATION_PARAMETERS
#define SIMULATION_PARAMETERS

#define UAV_ICON_PATH "/home/raju1234/ns-allinone-3.38/ns-3.38/src/btp/uavIcon.png"
#define RSU_ICON_PATH "/home/raju1234/ns-allinone-3.38/ns-3.38/src/btp/roadSideUnitIcon.png"
#define VANETNODE_ICON_PATH "/home/raju1234/ns-allinone-3.38/ns-3.38/src/btp/vehicleNodeIcon.png"

// port number
#define CLUSTER_ROUTING_PORT_NUMBER 600

class SimulationParamters
{
  public:
  static const int numVanetNodes = 2;
  static const int VanetNodeMaxSpeed = 30;
  static const int simulationDimension = 100;
  static const int TotalSimulationTime = 300; // in seconds

  static const int VanetNodeRange = 100; // in meters
  static const int FanetNodeRange = 300; // in meters
};

// The type of the node in the network
enum NodeType
{
  VehicleNode = 1,
  Uav = 2,
  Rsu = 3
};

enum ClusterNodeType
{
  ClusterHead = 1,
  ClusterMember = 2,
  NonClusterNode = 3
};

enum ClusterMessageType
{
  HELLO_MESSAGE = 1,
  CLUSTER_ADVERTISEMENT = 2,
  CH_TOGGLE_INITIALIZE = 3,
  CH_TOGGLE_PARTICIPATION = 4,
  CH_TRANSFER_ADVERTISEMENT = 5,
  NEW_CH_DECISION = 6
};

enum MolsrMessageType
{

};

// inline functions
#define DELAY(time)                                  \
  (((time) < (Simulator::Now())) ? Seconds(0.000001) \
                                 : (time - Simulator::Now() + Seconds(0.000001)))

#endif