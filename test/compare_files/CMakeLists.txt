# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 3.5)

# define project name and its language
project(compare_files CXX)

# define c++ standard and issue all the warning demanded by this standard
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
   add_definitions(-std=c++11 -pedantic -Wno-vla-extension)
else()
   add_definitions(-std=c++11 -pedantic -Wno-vla-extension -fext-numeric-literals)
endif()
set(CMAKE_CXX_STANDARD 11)

if (NOT DEFINED CMAKE_MACOSX_RPATH)
   set(CMAKE_MACOSX_RPATH 0)
endif()

# FIND LIBRARIES ===========================================================================

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../../cmake/Modules/")

# find hdf5
find_package(HDF5 COMPONENTS CXX)

# directories containing headers 
include_directories(${HDF5_INCLUDE_DIRS})

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

        compare_files 

        ${source_files}
)

# define libraries to be linked
target_link_libraries(

        compare_files 
        
	${HDF5_LIBRARIES}
)
