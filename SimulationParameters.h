#ifndef SIMULATION_PARAMETERS_H
#define SIMULATION_PARAMETERS_H

// image paths for node icons
#define UAV_ICON_PATH "/home/raju1234/ns-allinone-3.38/ns-3.38/src/btp/Res/uavIcon.png"
#define RSU_ICON_PATH "/home/raju1234/ns-allinone-3.38/ns-3.38/src/btp/Res/roadSideUnitIcon.png"
#define VANETNODE_ICON_PATH "/home/raju1234/ns-allinone-3.38/ns-3.38/src/btp/Res/vehicleNodeIcon.png"

// number of nodes
// experiment with different number of vanet  nodes
#define NUM_VANET_NODES 50
#define NUM_FANET_NODES 9
#define NUM_RSU_NODES 16

// Dimensions of simulation
#define AREA_LENGTH 1000

// simulation time in seconds
#define TOTAL_SIMULATION_TIME 30 

// speed of vanet nodes
#define VEHICLE_MIN_SPEED 0
#define VEHICLE_MAX_SPEED 30
#define VEHICLE_PAUSE_TIME 5

// 100 meters above from the ground
#define UAV_ALTITUDE 100

// device ranges
#define VANET_RANGE 1000
#define UAV_RANGE 1000
#define RSU_RANGE 1000

#endif