# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 3.2)

# define project name and its language
project(PARTONS_release CXX)

# define c++ standard and issue all the warning demanded by this standard
add_definitions(-std=c++11 -pedantic -Wno-vla-extension)
set(CMAKE_CXX_STANDARD 11)

if (NOT DEFINED CMAKE_MACOSX_RPATH)
   set(CMAKE_MACOSX_RPATH 0)
endif()

# AUTOMOC ==================================================================================

# tell cmake to run Qt moc when necessary
set(CMAKE_AUTOMOC ON)
  
# as moc files are generated in the binary dir, tell cmake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# FIND LIBRARIES ===========================================================================

# find libraries: Qt
find_package(Qt5 COMPONENTS Core Sql XmlPatterns REQUIRED)

# add some useful macros and variables
add_definitions(${QT_DEFINITIONS})

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: ElementaryUtils
find_package(ElementaryUtils REQUIRED)

# find libraries: NumA++
find_package(NumA++ REQUIRED)

# find libraries: SFML
find_package(SFML COMPONENTS system REQUIRED)

# find libraries: CLN
find_package(CLN REQUIRED)

# find libraries: GSL
find_package(GSL REQUIRED)

# find libraries: Apfel++
find_package(Apfel++ REQUIRED)

# find libraries: LHAPDF
find_package(LHAPDF REQUIRED)

# directories containing headers 
include_directories(${QT_INCLUDE_DIRS} ${SFML_INCLUDE_DIR} ${CLN_INCLUDE_DIR} ${ElementaryUtils_INCLUDE_DIR} ${NumA++_INCLUDE_DIR} ${Apfel++_INCLUDE_DIR} ${LHAPDF_INCLUDE_DIR})
include_directories(include)

# ENVIRONMENT VARIABLES ========================================================================

execute_process(COMMAND bash -c "cd ../elementary-utils/; git branch --show-current" OUTPUT_VARIABLE ELEMUTILS_BRANCH)
execute_process(COMMAND bash -c "cd ../elementary-utils/; git rev-parse HEAD;"             OUTPUT_VARIABLE ELEMUTILS_REVISION)
string(REGEX REPLACE "\n$" "" ELEMUTILS_BRANCH "${ELEMUTILS_BRANCH}")
string(REGEX REPLACE "\n$" "" ELEMUTILS_REVISION "${ELEMUTILS_REVISION}")

execute_process(COMMAND bash -c "cd ../numa/; git branch --show-current" OUTPUT_VARIABLE NUMA_BRANCH)
execute_process(COMMAND bash -c "cd ../numa/; git rev-parse HEAD;"             OUTPUT_VARIABLE NUMA_REVISION)
string(REGEX REPLACE "\n$" "" NUMA_BRANCH "${NUMA_BRANCH}")
string(REGEX REPLACE "\n$" "" NUMA_REVISION "${NUMA_REVISION}")

execute_process(COMMAND bash -c "cd ../partons/; git branch --show-current" OUTPUT_VARIABLE PARTONS_BRANCH)
execute_process(COMMAND bash -c "git rev-parse HEAD"             OUTPUT_VARIABLE PARTONS_REVISION)
string(REGEX REPLACE "\n$" "" PARTONS_BRANCH "${PARTONS_BRANCH}")
string(REGEX REPLACE "\n$" "" PARTONS_REVISION "${PARTONS_REVISION}")

configure_file("${CMAKE_SOURCE_DIR}/bin/environment_configuration.dat.in" "${CMAKE_SOURCE_DIR}/bin/environment_configuration.dat")

# FINALIZE ==================================================================================

# generate list of source files
file(

        GLOB_RECURSE

        source_files

        src/*
)

# define target executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(

        PARTONS_example

        ${source_files}
)

# define libraries to be linked
target_link_libraries(

        PARTONS_example

        ${PARTONS_LIBRARIES} 

	${QT_LIBRARIES}

	${SFML_LIBRARIES}

	${CLN_LIBRARIES}

	${ElementaryUtils_LIBRARIES}

	${NumA++_LIBRARIES}

	${GSL_LIBRARIES}

	${Apfel++_LIBRARIES}

	${LHAPDF_LIBRARIES}

	Qt5::Core

	Qt5::Sql

	Qt5::XmlPatterns
)
