cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(artemis-test)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/ CACHE PATH "install dir" FORCE)

find_package(artemis HINTS ${TARTSYS} REQUIRED)
find_package(ROOT REQUIRED COMPONENTS RIO Net Physics Minuit Minuit2 Geom Gui Genetic)
find_package(yaml-cpp REQUIRED HINTS ${ARTEMIS_YAML_CPP_LIB_DIR}/../)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(${ROOT_USE_FILE})

add_subdirectory(src)
