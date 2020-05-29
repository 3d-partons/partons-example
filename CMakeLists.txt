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

# find libraries: gsl
find_package(GSL)

# find libraries: Qt
find_package(Qt5 COMPONENTS Core Sql REQUIRED)

# add some useful macros and variables
add_definitions(${QT_DEFINITIONS})

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: SFML
find_package(SFML COMPONENTS system REQUIRED)

# find libraries: ElementaryUtils
find_package(ElementaryUtils REQUIRED)

# find libraries: NumA++
find_package(NumA++ REQUIRED)

# find libraries: PARTONS
find_package(PARTONS REQUIRED)

# directories containing headers 
include_directories($(GSL_INCLUDE_DIRS) ${QT_INCLUDE_DIRS} ${SFML_INCLUDE_DIR} ${ElementaryUtils_INCLUDE_DIR} ${NumA++_INCLUDE_DIR} ${PARTONS_INCLUDE_DIR})

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
        
        ${GSL_LIBRARIES}
        
        ${QT_LIBRARIES}

	${SFML_LIBRARIES}

        ${ElementaryUtils_LIBRARIES}
 
        ${NumA++_LIBRARIES}
        
        ${PARTONS_LIBRARIES} 

	Qt5::Core

	Qt5::Sql
)
