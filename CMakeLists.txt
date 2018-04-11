# CMakeList FILE for evesim
#
# Created on: Jul 12, 2012
#    Author: dejan

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)

SET( CMAKE_VERBOSE_MAKEFILE OFF )

#################################################################################

SET( PROJECT_NAME "EVESIM" CACHE STRING "The name of the project" )
PROJECT( ${PROJECT_NAME} )

ENABLE_TESTING()
	
#
# Configure the build process (locate the necessary libraries and build tools, etc) 
#
INCLUDE( "buildConfig/config.cmake" )


INCLUDE_REGULAR_EXPRESSION("^.*(h|cpp)$")

INCLUDE_DIRECTORIES(
      "${PROJECT_SOURCE_DIR}/evesim"      
      "${PROJECT_SOURCE_DIR}/unittests"      
)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${PROJECT_SOURCE_DIR}/documentation/conf/Doxygen.conf ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/evesim
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

#
# Build all subdirs
#
SUBDIRS( evesim unittests python )  

if(CMAKE_COMPILER_IS_GNUCXX)
   message(STATUS "Found GNU compiler!")
   execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
   if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        message(STATUS "C++11 activated.")
        add_definitions("-std=gnu++11")
   elseif(GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
        message(STATUS "C++0x activated.")
        add_definitions("-std=gnu++0x")
   endif()
endif(CMAKE_COMPILER_IS_GNUCXX)

ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS evesim_test)

