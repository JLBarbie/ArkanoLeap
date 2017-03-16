CMAKE_MINIMUM_REQUIRED (VERSION 3.0)

## Set all Required var
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
SET(CMAKE_VERBOSE_MAKEFILE FALSE)
SET(CMAKE_INSTALL_PREFIX /usr/bin)
SET(CMAKE_CXX_COMPILER_ID "GNU")
# SET (SHAREDLIB_DIR "${PROJECT_SOURCE_DIR}/shared")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Main project name
PROJECT (arkanoLeap)

INCLUDE (Colors)

INCLUDE_DIRECTORIES ("include")

SET (SRCS
    src/main.cpp
    src/Game.cpp
    src/LeapControl.cpp
)

# Adds sources to the Solution Explorer
add_executable (arkanoleap ${SRCS})

# Packages finding
MESSAGE("-- Find packages")
MESSAGE ("-- CPP Flags: " ${CMAKE_CXX_FLAGS})
if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    MESSAGE( STATUS "Setting the flags for ${CMAKE_CXX_COMPILER_ID} compiler" )
    ADD_DEFINITIONS( --std=c++1z )
endif()

# SET (CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -O3 --pedantic")
SET (CMAKE_CXX_FLAGS "-g -Wall -Wextra -O3")

find_package(GLFW)
if(GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_DIR})
    target_link_libraries(arkanoleap ${GLFW_LIBRARIES})
    message("GLFW Found")
endif()

find_package(Leap REQUIRED)
if(LEAP_FOUND)
   include_directories(${LEAP_INCLUDE_DIRS})
   target_link_libraries(arkanoleap ${LEAP_LIBRARIES})
endif()


# LINKER ( -lnlp )
target_link_libraries (arkanoleap
    GL
    GLU
    glut
    Leap
)

set_property(TARGET arkanoleap PROPERTY FOLDER "executables")

install (TARGETS arkanoleap
RUNTIME DESTINATION bin)
