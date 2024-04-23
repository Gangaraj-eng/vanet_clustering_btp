#ifndef VANET_ROUTING_REPOSITORY_H
#define VANET_ROUTING_REPOSITORY_H

/**
 * Vanet Routing Repository
 * Contains definitions required for Routing
 */

// port numbers used by routing protocols
#define CR_PORT_NUMBER 600
#define OLSR_PORT_NUMBER 601
#define QR_PORT_NUMBER 602

// enumeration for different types of nodes
enum NodeType
{
  NONE = 0, // this is invalid
  CLUSTER_HEAD = 1,
  CLUSTER_MEMBER = 2,
  UAV = 3,
  RSU = 4
};

#endif