N - Total number of nodes in the network
U - Number of UAVs  (Nodes in FANET)
V - Number of Vehicles (Nodes in VANET)
R - Number of RSUs (Road side units)

Routing protocols:
1 - InterClusterRouting - Modified OLSR routing happening between CHs(Cluster Head) and UAVs
2 - IntraClusterRouting - Any simple routing between the members of a cluster 
3 - QueryRoutings - Routing between VANET nodes and RSUs


Modified OLSR Routing:
Tables: 
    1 Neighbor Table
    2 Two hop neighbor Table
    3 Routing Table

Simulation Environment: 
    - Mobility Model: Random 2d walk
    - Traffic Model: An appllication is added to nodes to run traffic

Design: 
    - Implementing Routing protocols
    - Creating a custom appllication
    - Using NS3 energy model to track energy