set(Headers 
    EsrBinDecayEvent.h 
    EsrDecayEvent.h 
    EsrIdList.h 
    EsrInjData.h 
    EsrSMSEvent.h)
set(Sources 
    EsrBinDecayEvent.cxx 
    EsrDecayEvent.cxx 
    EsrInjData.cxx 
    EsrSMSEvent.cxx)
set(Header_DIR ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY("${Headers}" "Linkdef.h" "Dict.cxx"
    "${Header_DIR}")
message(STATUS "Directory that contains headers: ${CMAKE_CURRENT_SOURCE_DIR}")
add_library(esr_dict SHARED Dict.cxx ${Sources})
# ROOT stuff
include_directories(${Header_DIR})
include_directories(${ROOT_INCLUDE_DIR})
