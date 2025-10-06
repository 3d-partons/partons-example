# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 3.5)

# define project name and its language
project(PARTONS_example CXX)

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
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: ElementaryUtils
find_package(ElementaryUtils REQUIRED)

# find libraries: NumA++
find_package(NumA++ REQUIRED)

# find libraries: PARTONS
find_package(PARTONS REQUIRED)

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

# find libxml2
find_package(LibXml2)

# directories containing headers 
include_directories(${QT_INCLUDE_DIRS} ${ElementaryUtils_INCLUDE_DIR} ${NumA++_INCLUDE_DIR} ${PARTONS_INCLUDE_DIR} ${SFML_INCLUDE_DIR} ${CLN_INCLUDE_DIR} ${GSL_INCLUDE_DIRS} ${Apfel++_INCLUDE_DIR} ${LHAPDF_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIRS})

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
        
    	${SFML_LIBRARIES}
          
	${CLN_LIBRARIES}
          
	${ElementaryUtils_LIBRARIES}
          
	${NumA++_LIBRARIES}
		
	${PARTONS_LIBRARIES}

	${GSL_LIBRARIES}

	${Apfel++_LIBRARIES}

	${LHAPDF_LIBRARIES}

	${LIBXML2_LIBRARIES}
)
