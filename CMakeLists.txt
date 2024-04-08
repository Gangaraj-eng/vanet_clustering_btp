build_lib(
  LIBNAME btp 
  SOURCE_FILES  
  Nodes/VANET/VanetNode.cc
  Nodes/FANET/FanetNode.cc
  Routing/ClusterRouting/helper/ClusterRoutingHelper.cc
  Routing/ClusterRouting/model/ClusterRoutingProtocol.cc
  Routing
  Nodes/RSU/RSUNode.cc
  HEADER_FILES
  Nodes/VANET/VanetNode.h
  Routing/ClusterRouting/helper/ClusterRoutingHelper.h
  Routing/ClusterRouting/model/ClusterRoutingProtocol.h
  Nodes/FANET/FanetNode.h
  Nodes/RSU/RSUNode.h
  LIBRARIES_TO_LINK ${libinternet}
)