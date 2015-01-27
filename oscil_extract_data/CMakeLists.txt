CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(Extract)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(EnableCXX11)
include(FindROOT)

##### This was a nice idea but it doesn't work because necessary libs get stripped
#if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#    message(STATUS "Found OS Linux")
#    message(STATUS "Adding linker flag: --as-needed")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--as-needed")
#    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--as-needed")
#endif()

add_executable(macro macro.cxx)
add_subdirectory(source)
target_link_libraries(macro esr_dict)

# Require ROOT and C++11
# ROOT stuff
include_directories(source)
include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})
target_link_libraries(macro ${ROOT_LIBRARIES})
