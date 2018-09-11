# We need cmake version 3.2.2 or higher
cmake_minimum_required(VERSION 3.2.2)

# Create a new solution
project(SimpleVolumeRenderer)

# Define add_subproject and add_package to create XXConfig.cmake and XXConfigVersion.cmake
include(cmake_scripts/add_package.cmake)

# Disable in-source builds
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "Please create a separate directory for build files.")
endif()

########################
# Some default settings
########################

# Tell CMake to install binaries and exports locally
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "../export" CACHE PATH "Default install path" FORCE )
endif()

# Distinguish debug/realse binaries and put everything in the ./bin directory.
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

####################
# External Packages
####################

# Add Qt5
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add GetSet
find_package(GetSet REQUIRED)
include_directories(${GetSet_INCLUDE_DIRS})

########################
# Libraries "./code"
########################

# Important stuff is located in the "code" directory
include_directories(./code)

# The following libraries are always being built and a find_package(...) script is generated
add_package(LibSimple "code"  0 0 1)

#####################################################
# Executables "./tools" (enable/disable in CMake)
#####################################################

add_subproject(VolumeRendering "tools" ON)

