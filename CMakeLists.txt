build_lib(
  LIBNAME btp

  SOURCE_FILES

  VanetRouting/helper/VanetRoutingHelper.cc
  VanetRouting/model/VanetRoutingProtocol.cc

  Application/helper/VanetApplicationHelper.cc
  Application/model/VanetApplication.cc
  
  HEADER_FILES
  
  SimulationParameters.h
  
  VanetRouting/helper/VanetRoutingHelper.h
  VanetRouting/model/VanetRoutingProtocol.h
  VanetRouting/model/VanetRoutingRepository.h

  Application/helper/VanetApplicationHelper.h
  Application/model/VanetApplication.h
  Application/model/VanetApplicationRepository.h

  LIBRARIES_TO_LINK ${libinternet}
)