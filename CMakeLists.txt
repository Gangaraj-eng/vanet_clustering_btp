build_lib(
  LIBNAME btp 
  SOURCE_FILES  
  Nodes/VANET/VanetNode.cc
  Nodes/FANET/FanetNode.cc
  Routing/ClusterRouting/helper/ClusterRoutingHelper.cc
  Routing/ClusterRouting/model/ClusterRoutingProtocol.cc
  Routing/ClusterRouting/model/ClusterRoutingHeader.cc
  Routing/ClusterRouting/model/ClusterNodeState.cc
  Routing/MolsrRouting/helper/molsr-helper.cc
  Routing/MolsrRouting/model/molsr-header.cc
  Routing/MolsrRouting/model/molsr-state.cc
  Routing/MolsrRouting/model/molsr-routing-protocol.cc
  Application/model/VanetNodeApplication.cc
  Application/helper/VanetNodeApplicationHelper.cc
  Nodes/RSU/RSUNode.cc

  Routing/VanetRouting/helper/VanetRoutingHelper.cc
  Routing/VanetRouting/model/VanetRoutingProtocol.cc
  HEADER_FILES
  Nodes/VANET/VanetNode.h
  Nodes/FANET/FanetNode.h
  Nodes/RSU/RSUNode.h
  UADCConstants.h
  Routing/ClusterRouting/helper/ClusterRoutingHelper.h
  Routing/ClusterRouting/model/ClusterRoutingProtocol.h
  Routing/ClusterRouting/model/ClusterRoutingHeader.h
  Routing/ClusterRouting/model/ClusterNodeState.h
  Routing/ClusterRouting/model/ClusterRoutingRepository.h
  Routing/MolsrRouting/helper/molsr-helper.h
  Routing/MolsrRouting/model/molsr-header.h
  Routing/MolsrRouting/model/molsr-state.h
  Routing/MolsrRouting/model/molsr-repositories.h
  Routing/MolsrRouting/model/molsr-routing-protocol.h
  Application/model/VanetNodeApplication.h
  Application/helper/VanetNodeApplicationHelper.h

  Routing/VanetRouting/helper/VanetRoutingHelper.h
  Routing/VanetRouting/model/VanetRoutingProtocol.h
  LIBRARIES_TO_LINK ${libinternet}
)