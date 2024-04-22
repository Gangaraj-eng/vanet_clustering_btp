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
  // num Vanet Nodes = 100
  // num Fanet Nodes = 20
  static const int numVanetNodes = 90;
  static const int numFanetNodes = 9;
  static const int VanetNodeMaxSpeed = 30;
  static const int simulationDimension = 1000;
  static const int TotalSimulationTime = 20; // in seconds
  static const int ClusterRange = 5000;

  static const int VanetNodeRange = 500;  // in meters
  static const int FanetNodeRange = 1000; // in meters
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
  NONE = 0,
  HELLO_MESSAGE = 1,
  CLUSTER_ADVERTISEMENT = 2,
  CH_TOGGLE_INITIALIZE = 3,
  CH_TOGGLE_PARTICIPATION = 4,
  CH_TRANSFER_DATA = 5,
  CH_ToggledAck = 6,
  CH_CHANGE_ADVERTISEMENT = 7
};

enum MolsrMessageType
{

};

// inline functions
#define DELAY(time)                                  \
  (((time) < (Simulator::Now())) ? Seconds(0.000001) \
                                 : (time - Simulator::Now() + Seconds(0.000001)))

#endif