cmake_minimum_required(VERSION 2.6)

#Configuration du projet

project(PARTONS_release CXX)

# Force C++98 warnings
add_definitions(-std=c++98 -pedantic)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
  
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

 # this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED QtCore QtSql )

 # add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE})
ADD_DEFINITIONS (${QT_DEFINITIONS})

#  placez vos exécutables dans des dossiers portant le nom du type de compilation (Release, Debug...). Ce dernier est défini par la variable CMAKE_BUILD_TYPE (qui n'a d'effet seulement si le générateur choisi est basé sur Make)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Recherche de librairies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
FIND_PACKAGE(ROOT COMPONENTS Core Hist REQUIRED)
find_package(SFML COMPONENTS system REQUIRED)

# NumA++
set(NUMA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../NumA++/include")
set(NUMA_LIB_DIR "${CMAKE_SOURCE_DIR}/../NumA++/lib")
set(NUMA_LIBRARIES NumA++)

# PARTONS
set(PARTONS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../PARTONS/include")
set(PARTONS_LIB_DIR "${CMAKE_SOURCE_DIR}/../PARTONS/lib")
set(PARTONS_LIBRARIES PARTONS)

# ElementaryUtils
set(ELEMENTARY_UTILS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../ElementaryUtils/include")
set(ELEMENTARY_UTILS_LIB_DIR "${CMAKE_SOURCE_DIR}/../ElementaryUtils/lib")
set(ELEMENTARY_UTILS_LIBRARIES ElementaryUtils)

# Inclusion de librairies

INCLUDE_DIRECTORIES( ${ELEMENTARY_UTILS_INCLUDE_DIR} ${QT_INCLUDE_DIRS} ${ROOT_INCLUDE_DIR} ${PARTONS_INCLUDE_DIR} ${NUMA_INCLUDE_DIR} ${SFML_INCLUDE_DIR})

LINK_DIRECTORIES( ${ELEMENTARY_UTILS_LIB_DIR} ${PARTONS_LIB_DIR} ${NUMA_LIB_DIR})

#Génération de la liste des fichiers sources

file(

        GLOB_RECURSE

        source_files

        src/*
)

add_executable(

        PARTONS_release

        ${source_files}
)

#Configuration de l'édition de liens
target_link_libraries(

        PARTONS_release

        ${PARTONS_LIBRARIES}

        ${ELEMENTARY_UTILS_LIBRARIES}

        ${ROOT_LIBRARIES}
 
        ${NUMA_LIBRARIES}
        
        ${QT_LIBRARIES}
        
        ${SFML_LIBRARIES}
)