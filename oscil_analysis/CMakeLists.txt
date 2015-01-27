CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
message(STATUS ${CMAKE_MODULE_PATH})

project(OscilAnalysis)
add_executable(oscilloscope oscilloscope.cpp filter.cpp)

# Require ROOT and C++11
# does not work with the extension.
include(EnableCXX11)
include(FindROOT)
# ROOT stuff
include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})
target_link_libraries(oscilloscope ${ROOT_LIBRARIES})

# Boost
find_package(Boost 1.5 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(oscilloscope ${Boost_LIBRARIES})

find_package(OpenMP)
if(${OPENMP_FOUND})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
