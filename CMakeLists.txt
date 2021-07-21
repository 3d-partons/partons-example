# BASIC DEFINITIONS ========================================================================

# define minimum version of cmake
cmake_minimum_required(VERSION 3.2)

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

# AUTOMOC ==================================================================================

# tell cmake to run Qt moc when necessary
set(CMAKE_AUTOMOC ON)
  
# as moc files are generated in the binary dir, tell cmake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# FIND LIBRARIES ===========================================================================

# find libraries: additional modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# find libraries: Qt
find_package(Qt5 COMPONENTS Core Sql XmlPatterns QUIET)

if(${Qt5_FOUND})

   find_package(Qt5Widgets QUIET)
   
   if (Qt5Widgets_FOUND)
   	  set(QT_VERSION ${Qt5Widgets_VERSION})
   else()
      set(QT_VERSION "5.??.??")
   endif()

   set(QT_LIBRARY_CORE "Qt5::Core")
   set(QT_LIBRARY_SQL "Qt5::Sql")
   set(QT_LIBRARY_XML "Qt5::XmlPatterns")
   
   message("-- Found Qt5: " ${QT_VERSION})
   
else()

   find_package(Qt4 COMPONENTS QtCore QtSql QtXmlPatterns QUIET)

   if(${Qt4_FOUND})
   
   	  set(QT_VERSION ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH})

      set(QT_LIBRARY_CORE "Qt4::QtCore")
      set(QT_LIBRARY_SQL "Qt4::QtSql")
      set(QT_LIBRARY_XML "Qt4::QtXmlPatterns")
      
      message("-- Found Qt4: " ${QT_VERSION})

   else()
      message(FATAL_ERROR "Neither Qt5 nor Qt4 found in this system")
   endif()
endif()

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

# directories containing headers 
include_directories(${QT_INCLUDE_DIRS} ${ElementaryUtils_INCLUDE_DIR} ${NumA++_INCLUDE_DIR} ${PARTONS_INCLUDE_DIR} ${SFML_INCLUDE_DIR} ${CLN_INCLUDE_DIR} $(GSL_INCLUDE_DIRS) ${Apfel++_INCLUDE_DIR} ${LHAPDF_INCLUDE_DIR})

# ENVIRONMENT VARIABLES ========================================================================

file(READ ${ElementaryUtils_INCLUDE_DIR}/ElementaryUtils/ElementaryUtilsVersion.h VERSION_STR)

string(REGEX MATCH "ELEMENTARY_UTILS_GIT_BRANCH[ \t]*[^\t\n]*\n" ELEMUTILS_BRANCH "${VERSION_STR}")
string(REGEX REPLACE "ELEMENTARY_UTILS_GIT_BRANCH[ \t]*" "" ELEMUTILS_BRANCH "${ELEMUTILS_BRANCH}")
string(REGEX REPLACE "\"" "" ELEMUTILS_BRANCH "${ELEMUTILS_BRANCH}")
string(STRIP ${ELEMUTILS_BRANCH} ELEMUTILS_BRANCH)
string(REGEX MATCH "ELEMENTARY_UTILS_GIT_REVISION[ \t]*[^\t\n]*\n" ELEMUTILS_REVISION ${VERSION_STR})
string(REGEX REPLACE "ELEMENTARY_UTILS_GIT_REVISION[ \t]*" "" ELEMUTILS_REVISION "${ELEMUTILS_REVISION}")
string(REGEX REPLACE "\"" "" ELEMUTILS_REVISION "${ELEMUTILS_REVISION}")
string(STRIP "${ELEMUTILS_REVISION}" ELEMUTILS_REVISION)

file(READ ${NumA++_INCLUDE_DIR}/NumA/NumAVersion.h VERSION_STR)

string(REGEX MATCH "NUMA_GIT_BRANCH[ \t]*[^\t\n]*\n" NUMA_BRANCH ${VERSION_STR})
string(REGEX REPLACE "NUMA_GIT_BRANCH[ \t]*" "" NUMA_BRANCH "${NUMA_BRANCH}")
string(REGEX REPLACE "\"" "" NUMA_BRANCH "${NUMA_BRANCH}")
string(STRIP "${NUMA_BRANCH}" NUMA_BRANCH)
string(REGEX MATCH "NUMA_GIT_REVISION[ \t]*[^\t\n]*\n" NUMA_REVISION ${VERSION_STR})
string(REGEX REPLACE "NUMA_GIT_REVISION[ \t]*" "" NUMA_REVISION "${NUMA_REVISION}")
string(REGEX REPLACE "\"" "" NUMA_REVISION "${NUMA_REVISION}")
string(STRIP "${NUMA_REVISION}" NUMA_REVISION)

file(READ ${PARTONS_INCLUDE_DIR}/partons/PartonsVersion.h VERSION_STR)

string(REGEX MATCH "PARTONS_GIT_BRANCH[ \t]*[^\t\n]*\n" PARTONS_BRANCH ${VERSION_STR})
string(REGEX REPLACE "PARTONS_GIT_BRANCH[ \t]*" "" PARTONS_BRANCH "${PARTONS_BRANCH}")
string(REGEX REPLACE "\"" "" PARTONS_BRANCH "${PARTONS_BRANCH}")
string(STRIP "${PARTONS_BRANCH}" PARTONS_BRANCH)
string(REGEX MATCH "PARTONS_GIT_REVISION[ \t]*[^\t\n]*\n" PARTONS_REVISION ${VERSION_STR})
string(REGEX REPLACE "PARTONS_GIT_REVISION[ \t]*" "" PARTONS_REVISION "${PARTONS_REVISION}")
string(REGEX REPLACE "\"" "" PARTONS_REVISION "${PARTONS_REVISION}")
string(STRIP "${PARTONS_REVISION}" PARTONS_REVISION)

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
        
    	${SFML_LIBRARIES}
          
		${CLN_LIBRARIES}
          
		${ElementaryUtils_LIBRARIES}
          
		${NumA++_LIBRARIES}
		
		${PARTONS_LIBRARIES}

		${GSL_LIBRARIES}

		${Apfel++_LIBRARIES}

		${LHAPDF_LIBRARIES}

		${QT_LIBRARY_CORE}
		
		${QT_LIBRARY_SQL}
		
		${QT_LIBRARY_XML}
)
