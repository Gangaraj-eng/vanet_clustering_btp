build_lib(
  LIBNAME btp 
  SOURCE_FILES  
  Nodes/VANET/VanetNode.cc
  Nodes/FANET/FanetNode.cc
  Nodes/RSU/RSUNode.cc
  HEADER_FILES
  Nodes/VANET/VanetNode.h
  Nodes/FANET/FanetNode.h
  Nodes/RSU/RSUNode.h
  LIBRARIES_TO_LINK ${libinternet}
)