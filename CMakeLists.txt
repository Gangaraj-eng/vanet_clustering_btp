build_lib(
  LIBNAME btp

  SOURCE_FILES

  VanetRouting/helper/VanetRoutingHelper.cc
  VanetRouting/model/VanetRoutingProtocol.cc

  VanetRouting/model/crRouting/ClusterRoutingProtocol.cc
  VanetRouting/model/qrRouting/QueryRoutingProtocol.cc
  VanetRouting/model/olsrRouting/OLSRRoutingProtocol.cc

  Application/helper/VanetApplicationHelper.cc
  Application/model/VanetApplication.cc

  Node/CustomNode.cc
  
  HEADER_FILES
  
  SimulationParameters.h
  
  VanetRouting/helper/VanetRoutingHelper.h
  VanetRouting/model/VanetRoutingProtocol.h
  VanetRouting/model/VanetRoutingRepository.h

  VanetRouting/model/crRouting/ClusterRoutingProtocol.h
  VanetRouting/model/qrRouting/QueryRoutingProtocol.h
  VanetRouting/model/olsrRouting/OLSRRoutingProtocol.h

  Application/helper/VanetApplicationHelper.h
  Application/model/VanetApplication.h
  Application/model/VanetApplicationRepository.h

  Node/CustomNode.h

  LIBRARIES_TO_LINK ${libinternet}
)