cmake_minimum_required(VERSION 2.8)
project(analyse)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()
MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "-std=c++11 -O3" )

find_package(OpenCV 3.0 QUIET)
 if(NOT OpenCV_FOUND)
  find_package(OpenCV 2.4.3 QUIET)
    if(NOT OpenCV_FOUND)
       message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
    endif()
endif()

include_directories(${CMAKE_INCLUDE_DIRECTORIES}
${EIGEN3_INCLUDE_DIR}
)

add_executable(analyse analyse.cc)
target_link_libraries(analyse ${OpenCV_LIBS})
