# check the minimum version
cmake_minimum_required( VERSION 2.8.12 )

# the project name
project( WRAPPER )

################################
#### General configure section
################################

# force the Release build if not already set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# setting common c++ flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -Wl,--hash-style=both,--as-needed -pthread -fopenmp -ftemplate-depth=1100" )

# setting debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g3 -O0 -ftemplate-depth=1100")

# setting release with debug info flags
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -march=native -mtune=native -g3 -O2 -ftemplate-depth=1100")

# setting release flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native -O3 -ftemplate-depth=1100")

###############################################
##             EXTERNAL LIB                  ##
###############################################

# add the path to the modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/margot_heel_if/cmake")

# find the required packages
find_package(MARGOT_HEEL)


# add the required include directories
include_directories(${MARGOT_HEEL_INCLUDES})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

###############################################
##             CONFIG FILES                  ##
###############################################
#used to generate the .clang_complete file that is used in vim/atom plugins for autocomplete.
#can be safely removed if not needed.
#configure_file(
#  "${CMAKE_CURRENT_SOURCE_DIR}/config/clang_autocomplete.in"
#  "${PROJECT_SOURCE_DIR}/.clang_complete"
#  @ONLY
#  )
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

###############################################
##             BUILD SECTION                 ##
###############################################

# Set the application name
set( APP_NAME "Wrapper")

# Build probability executable
set(SOURCE_FILES src/wrapper.cpp)

add_executable(${APP_NAME} ${SOURCE_FILES})
target_link_libraries(${APP_NAME} ${MARGOT_HEEL_LIBRARIES})


if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (
      CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}"
      CACHE PATH "default install path"
      FORCE )
endif()

install( TARGETS ${APP_NAME} DESTINATION bin )
