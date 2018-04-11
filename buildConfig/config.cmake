####################################################################################
#                   Platform, Compiler etc.                                        #
####################################################################################


IF (NOT CMAKE_BUILD_TYPE)
    MESSAGE( STATUS "No build type (CMAKE_BUILD_TYPE) specified. Assuming Release." )
    SET( CMAKE_BUILD_TYPE "Release" CACHE STRING "The build type: release or debug" FORCE )
ENDIF(NOT CMAKE_BUILD_TYPE)

MESSAGE( "   Build type   = ${CMAKE_BUILD_TYPE}" )

#------------------------------------------------------------------------------------
#  get compiler toolset and version (used for composing the boost library name)
#

MARK_AS_ADVANCED( CMAKE_BACKWARDS_COMPATIBILITY )

#------------------------------------------------------------------------------------
STRING( TOLOWER "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}" PLATFORMSTR )

SET( PLATFORM ${PLATFORMSTR} CACHE STRING "Platform specifier" )
MARK_AS_ADVANCED( PLATFORM )

MESSAGE( "   Platform     = ${PLATFORM}" )

#------------------------------------------------------------------------------------

SET( MAKE_TOOL ${CMAKE_BUILD_TOOL} CACHE INTERNAL "Build tool to use for thisplatform" )
MESSAGE( "   Make tool    = ${MAKE_TOOL}" )

#------------------------------------------------------------------------------------

BUILD_COMMAND( MAKE_ALL_CMD ${CMAKE_BUILD_TOOL} )
SET( MAKE_ALL_CMD ${MAKE_ALL_CMD} CACHE INTERNAL "Build tool to use for thisplatform" )
MESSAGE( "   Make command = ${MAKE_ALL_CMD}" )

MACRO( SPLIT_LIBRARY_NAME LIBPATH PATH NAME )
      GET_FILENAME_COMPONENT( ${PATH} ${LIBPATH} PATH )
      GET_FILENAME_COMPONENT( ${NAME} ${LIBPATH} NAME )
      IF( NOT PLATFORM STREQUAL "windows-x86" )
          STRING(REGEX REPLACE "^lib" "" ${NAME} ${${NAME}} )
          STRING(REGEX REPLACE "\\.[^\\.]*$" "" ${NAME} ${${NAME}} )
      ENDIF( NOT PLATFORM STREQUAL "windows-x86" )    
ENDMACRO( SPLIT_LIBRARY_NAME )

MACRO( GET_BUILD_PROPERTIES PREFIX )

        GET_DIRECTORY_PROPERTY( TMP_PROP MACROS )
        SET( ${PREFIX}_MACROS ${TMP_PROP} CACHE INTERNAL "Macros used to build ${PREFIX}" FORCE )
        
        GET_DIRECTORY_PROPERTY( TMP_PROP INCLUDE_DIRECTORIES )
        SET( ${PREFIX}_INCLUDE_DIRECTORIES ${TMP_PROP} CACHE INTERNAL "Include directories used to build ${PREFIX}" FORCE )
        
        GET_DIRECTORY_PROPERTY( TMP_PROP LINK_DIRECTORIES )
        SET( ${PREFIX}_LINK_DIRECTORIES ${TMP_PROP} CACHE INTERNAL "Link directories used to build ${PREFIX}" FORCE )

        GET_DIRECTORY_PROPERTY( TMP_PROP DEFINITIONS )
        SET( ${PREFIX}_DEFINITIONS ${TMP_PROP} CACHE INTERNAL "Defines used to build ${PREFIX}" FORCE )
        
        SET( ${PREFIX}_LINK_LIBRARIES ${ARGN} CACHE INTERNAL "Extra libraries used to build ${PREFIX}" FORCE )

        SET( TMP_PROP )
        
ENDMACRO( GET_BUILD_PROPERTIES )


####################################################################################
#                             Python executable                                    #
####################################################################################

IF( NOT PYTHON_EXECUTABLE )
    MESSAGE( STATUS "Looking for python executable ..." )
ENDIF( NOT PYTHON_EXECUTABLE )

SET( PYTHON_EXECUTABLE )
FIND_PROGRAM( PYTHON_EXECUTABLE
  NAMES python python25 python2.5 python24 python2.4  
  PATHS ${CMAKE_PROGRAM_PATH} $ENV{PATH}
  NO_DEFAULT_PATH
)

FIND_PROGRAM( PYTHON_EXECUTABLE
	NAMES python python25 python2.5 python24 python2.4
	PATHS /usr/bin /usr/local/bin 
)

Find_Package( PythonInterp )

IF( PYTHON_EXECUTABLE )
  GET_FILENAME_COMPONENT( PYTHON_EXEC_PATH ${PYTHON_EXECUTABLE} PATH )
  GET_FILENAME_COMPONENT( PYTHON_EXEC_DIRNAME ${PYTHON_EXEC_PATH} NAME )
  
  IF (PYTHON_EXEC_DIRNAME STREQUAL "bin")
  	GET_FILENAME_COMPONENT( PYTHON_INSTALL_PATH ${PYTHON_EXEC_PATH} PATH )
  ELSE(PYTHON_EXEC_DIRNAME STREQUAL "bin")
  	SET(PYTHON_INSTALL_PATH ${PYTHON_EXEC_PATH})
  ENDIF(PYTHON_EXEC_DIRNAME STREQUAL "bin")
  
ENDIF( PYTHON_EXECUTABLE )

# Get the version of python interpreter (will be needed in the search of the python libraries)
EXEC_PROGRAM( python ARGS "-V" OUTPUT_VARIABLE OUTPUT_PYTHON_INTERP_VERSION )              
STRING(REGEX REPLACE "Python ([0-9])\\.([0-9]).*" "\\1.\\2" PYTHON_INTERP_VER ${OUTPUT_PYTHON_INTERP_VERSION}  )
STRING(REGEX REPLACE "Python ([0-9])\\.([0-9]).*" "\\1\\2" PYTHON_INTERP_VER_SHORT ${OUTPUT_PYTHON_INTERP_VERSION}  )

IF( PYTHON_EXECUTABLE )
    MESSAGE( "   Python executable : ${PYTHON_EXECUTABLE} version ${PYTHON_INTERP_VER} ")
ELSE( PYTHON_EXECUTABLE )
    MESSAGE( FATAL_ERROR "No python executable found. Set CMAKE_PROGRAM_PATH environment varaible to include non standard search paths." )
ENDIF( PYTHON_EXECUTABLE )



####################################################################################
#                             Python libraries                                     #
####################################################################################

IF( NOT PYTHON_INCLUDE_PATH )
    MESSAGE( STATUS "Looking for python libraries ... " )
ENDIF( NOT PYTHON_INCLUDE_PATH )

#
# First try find the python LIBRARIES from
#      - The PYTHON_INSTALL_PATH inferred from the location of the python executable found above  
#      -  the environment variables (LD_LIBRARY_PATH) 
#      -  cmake variables
#      
#      in that order.
# 

FIND_LIBRARY(PYTHON_LIBRARY
NAMES python${PYTHON_INTERP_VER_SHORT} python${PYTHON_INTERP_VER}
	  python27 python2.7
	  python26 python2.6 
      python25 python2.5
      python24 python2.4
      python23 python2.3
      python22 python2.2
      python21 python2.1
      python20 python2.0
      python16 python1.6
      python15 python1.5
            

PATHS
    ${PYTHON_INSTALL_PATH}/lib
    ${PYTHON_INSTALL_PATH}/libs
    ${PYTHON_INSTALL_PATH}/lib64
    ${CMAKE_LIBRARY_PATH} 
    ENV LD_LIBRARY_PATH  
    NO_DEFAULT_PATH
    
PATH_SUFFIXES
	python2.7/config
	python2.6/config
    python2.5/config
    python2.4/config
    python2.3/config
    python2.2/config
    python2.1/config
    python2.0/config
    python1.6/config
    python1.5/config

)

SET(PYTHON_LIBRARIES "${PYTHON_LIBRARY}")

IF( PYTHON_LIBRARIES )
    SPLIT_LIBRARY_NAME( ${PYTHON_LIBRARY} PYTHON_LIBRARY_PATH PYTHON_LIBRARY_LINK_NAME )
    # Get the version of the python library
	STRING(REGEX REPLACE ".*python" "" PYTHON_LIB_VERSION "${PYTHON_LIBRARY_LINK_NAME}")    
ENDIF( PYTHON_LIBRARIES )

FIND_PATH(PYTHON_INCLUDE_PATH
  NAMES Python.h pyconfig.h

  PATHS
    ${PYTHON_INSTALL_PATH} ${CMAKE_INCLUDE_PATH} NO_DEFAULT_PATH
    
  PATH_SUFFIXES
    include
    include/python${PYTHON_LIB_VERSION}
    
)

#Find_Package( PythonLibs QUIET REQUIRED )

IF( PYTHON_INCLUDE_PATH AND PYTHON_LIBRARY_PATH )
    MESSAGE( "   Python libraries : ${PYTHON_LIBRARY_PATH}" )
    MESSAGE( "   Python includes : ${PYTHON_INCLUDE_PATH}" )
ELSE( PYTHON_INCLUDE_PATH AND PYTHON_LIBRARY_PATH )
    MESSAGE( FATAL_ERROR "No python libraries found. Set CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH environment variables to include non-default installation paths." )
ENDIF( PYTHON_INCLUDE_PATH AND PYTHON_LIBRARY_PATH )

INCLUDE_DIRECTORIES( "${PYTHON_INCLUDE_PATH}" )

SET( PYTHON_INCLUDE_PATH ${PYTHON_INCLUDE_PATH} CACHE FILEPATH "Python include path" )
SET( PYTHON_LIBRARY_PATH ${PYTHON_LIBRARY_PATH} CACHE FILEPATH "Python library path" )
SET( PYTHON_LIBRARY_LINK_NAME ${PYTHON_LIBRARY_LINK_NAME} CACHE STRING "Name of link option for compiler" )


#####################################################################################
#                                                                                   #
#                            NUMPY                                                  #
#                                                                                   #
#####################################################################################

INCLUDE( buildConfig/FindNumPy.cmake )

IF( NUMPY_INCLUDE_DIRS )
    MESSAGE( "   Numpy includes : ${NUMPY_INCLUDE_DIRS}" )
ELSE( NUMPY_INCLUDE_DIRS )
    MESSAGE( FATAL_ERROR "No numpy python package found. Set CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH environment variables to include non-default installation paths." )
ENDIF( NUMPY_INCLUDE_DIRS )



#####################################################################################
#                                                                                   #
#                            BOOST C++ LIBRARIES                                    #
#                                                                                   #
#####################################################################################


IF ( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
     
    IF( CMAKE_GENERATOR STREQUAL "MinGW Makefiles" ) 
        SET( BOOST_TOOLSET_ABBREV "mgw" )
    ELSE( CMAKE_GENERATOR STREQUAL "MinGW Makefiles" )
        SET( BOOST_TOOLSET_ABBREV "gcc" )
    ENDIF( CMAKE_GENERATOR STREQUAL "MinGW Makefiles" )
    
    #
	# Try to figure out the version of the compiler
	#
    EXEC_PROGRAM( gcc ARGS "--version" OUTPUT_VARIABLE OUTPUT_GCC_VER )              
    STRING(REGEX REPLACE "^gcc.*([0-9])\\.([0-9])\\.[0-9].*$" "\\1\\2" COMPILER_VERSION_ABBREV ${OUTPUT_GCC_VER}  )
    
ENDIF( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )

SET( BOOST_LIB_SUFFIX "${BOOST_TOOLSET_ABBREV}" )

SET( BOOST_LIB_SUFFIX_2 "${BOOST_TOOLSET_ABBREV}${COMPILER_VERSION_ABBREV}" )

SET( BOOST_LIB_SUFFIX_3 "${BOOST_TOOLSET_ABBREV}-mt" )

SET( BOOST_LIB_SUFFIX_4 "${BOOST_TOOLSET_ABBREV}${COMPILER_VERSION_ABBREV}-mt" )

#MESSAGE( "BOOST_LIB_SUFFIX IS ${BOOST_LIB_SUFFIX}" )
#MESSAGE( "BOOST_LIB_SUFFIX_2 IS ${BOOST_LIB_SUFFIX_2}" )


IF( NOT BOOST_INCLUDE_PATH AND NOT BOOST_LIBRARY_PATH )
    MESSAGE( STATUS "Looking for Boost C++ libraries ... - toolset    = ${BOOST_TOOLSET_ABBREV}" )
    MESSAGE( STATUS "Looking for Boost C++ libraries ... - lib suffixes = ${BOOST_LIB_SUFFIX} ${BOOST_LIB_SUFFIX_2} ${BOOST_LIB_SUFFIX_3} ${BOOST_LIB_SUFFIX_4}")
ENDIF( NOT BOOST_INCLUDE_PATH AND NOT BOOST_LIBRARY_PATH )

SET( BOOST_LIB_PREFIX "" )
IF( PLATFORM STREQUAL "windows-x86" )
     SET( BOOST_LIB_PREFIX "lib" )
ENDIF( PLATFORM STREQUAL "windows-x86" )

MACRO( FIND_BOOST_LIBRARY NAME LIBNAME LIBPATH )
    
  FIND_LIBRARY( ${LIBNAME}
                NAMES "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX}" 
                      "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX_2}"
                      "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX_3}"
                      "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX_4}"
                      "${BOOST_LIB_PREFIX}boost_${NAME}-mt"
                      "${BOOST_LIB_PREFIX}boost_${NAME}"
                PATHS ENV LD_LIBRARY_PATH ENV PATH ${CMAKE_LIBRARY_PATH} NO_DEFAULT_PATH
                PATH_SUFFIXES  boost-1_50/lib
	           				  boost-1_49/lib
	                          boost-1_48/lib
	                          boost-1_47/lib
	           				  boost-1_46_1/lib
	           				  boost-1_46_0/lib
	           				  boost-1_46/lib
	                          boost-1_45/lib
	                          boost-1_44/lib
	                          boost-1_43/lib
	                          boost-1_42/lib
	           				  boost-1_41/lib
	                          boost-1_40/lib
	           				  boost-1_39/lib
	           				  boost-1_38/lib
	           				  boost-1_37/lib
	           				  boost-1_36/lib
	           				  boost-1_35/lib
	                          boost-1_34_1/lib
              )
                           
  FIND_LIBRARY( ${LIBNAME}
                NAMES "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX}"
                      "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX_2}"
                      "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX_3}"
                      "${BOOST_LIB_PREFIX}boost_${NAME}-${BOOST_LIB_SUFFIX_4}"  
                      "${BOOST_LIB_PREFIX}boost_${NAME}-mt" 
                      "${BOOST_LIB_PREFIX}boost_${NAME}"
                PATHS ${Boost_LIBRARY_DIRS}
                PATH_SUFFIXES boost-1_50/lib
	           				  boost-1_49/lib
	                          boost-1_48/lib
	                          boost-1_47/lib
	           				  boost-1_46_1/lib
	           				  boost-1_46_0/lib
	           				  boost-1_46/lib
	                          boost-1_45/lib
	                          boost-1_44/lib
	                          boost-1_43/lib
	                          boost-1_42/lib
	           				  boost-1_41/lib
	                          boost-1_40/lib
	           				  boost-1_39/lib
	           				  boost-1_38/lib
	           				  boost-1_37/lib
	           				  boost-1_36/lib
	           				  boost-1_35/lib
	                          boost-1_34_1/lib 
              )
   
  IF( ${LIBNAME} )
      SET( TMP_LIB_NAME ${${LIBNAME}} )
      SPLIT_LIBRARY_NAME( ${${LIBNAME}} ${LIBPATH} "${LIBNAME}_LINK_NAME" )
      SET( "${LIBNAME}_LINK_NAME" ${${LIBNAME}_LINK_NAME} CACHE STRING "Name of boost_${NAME} for compiler" )
      MARK_AS_ADVANCED( FORCE ${LIBNAME} )
      MESSAGE( "   Boost C++ library boost_${NAME} : ${${LIBNAME}}" )
  ELSE( ${LIBNAME} )
      MESSAGE( FATAL_ERROR "Cannot find boost_${NAME} library. Set CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH environment variables to include non-default installation paths." )
  ENDIF( ${LIBNAME} )
    
ENDMACRO( FIND_BOOST_LIBRARY )

#
# check needed libraries
#
FIND_BOOST_LIBRARY( date_time BOOST_DATETIME BOOST_DATETIME_LIBRARY_PATH )

SET( BOOST_LIBRARY_PATH ${BOOST_DATETIME_LIBRARY_PATH} )

GET_FILENAME_COMPONENT( BOOST_ROOT_DIR ${BOOST_LIBRARY_PATH} PATH)

#
# First check the system environment paths for the includes
#
SET( BOOST_INCLUDE_PATH )
FIND_PATH( BOOST_INCLUDE_PATH
           NAMES "boost/config.hpp"
           PATHS ${BOOST_ROOT_DIR} ${CMAKE_INCLUDE_PATH} ${Boost_INCLUDE_DIRS} NO_DEFAULT_PATH
           PATH_SUFFIXES include/boost-1_50
           				 include/boost-1_49
                         include/boost-1_48
                         include/boost-1_47
           				 include/boost-1_46_1
           				 include/boost-1_46_0
           				 include/boost-1_46
                         include/boost-1_45
                         include/boost-1_44
                         include/boost-1_43
                         include/boost-1_42
           				 include/boost-1_41
                         include/boost-1_40
           				 include/boost-1_39
           				 include/boost-1_38
           				 include/boost-1_37
           				 include/boost-1_36
           				 include/boost-1_35
                         include/boost-1_34_1
                         boost-1_50
           				 boost-1_49
                         boost-1_48
                         boost-1_47
           				 boost-1_46_1
           				 boost-1_46_0
           				 boost-1_46
                         boost-1_45
                         boost-1_44
                         boost-1_43
                         boost-1_42
           				 boost-1_41
                         boost-1_40
           				 boost-1_39
           				 boost-1_38
           				 boost-1_37
           				 boost-1_36
           				 boost-1_35
                         boost-1_34_1
           				 include )
           				 

IF( BOOST_INCLUDE_PATH AND BOOST_LIBRARY_PATH )
    MESSAGE( "   Boost C++ libraries : ${BOOST_LIBRARY_PATH} " )
    MESSAGE( "   Boost C++ include path : ${BOOST_INCLUDE_PATH} " )
ELSE( BOOST_INCLUDE_PATH AND BOOST_LIBRARY_PATH )
    MESSAGE( FATAL_ERROR "Cannot find boost libraries. Set CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH environment variables to include non-default installation paths." )
ENDIF( BOOST_INCLUDE_PATH AND BOOST_LIBRARY_PATH )

INCLUDE_DIRECTORIES( "${BOOST_INCLUDE_PATH}" )

# ADD_DEFINITIONS( -DBOOST_LIB_DIAGNOSTIC -DBOOST_ALL_NO_LIB -DBOOST_PYTHON_MAX_ARITY=30 )
ADD_DEFINITIONS( -DBOOST_PYTHON_MAX_ARITY=30 )

SET( BOOST_INCLUDE_PATH ${BOOST_INCLUDE_PATH} CACHE FILEPATH "Boost C++ include path" )
SET( BOOST_LIBRARY_PATH ${BOOST_LIBRARY_PATH} CACHE FILEPATH "Boost C++ library path" )
SET( Boost_INCLUDE_DIR ${Boost_INCLUDE_DIR} CACHE INTERNAL "Boost C++ library path used by package FindBoost" FORCE )
SET( BOOST_LIBS ${BOOST_THREAD_LINK_NAME} ${BOOST_DATETIME_LINK_NAME} ${BOOST_FILESYSTEM_LINK_NAME} ${BOOST_PYTHON_LINK_NAME} CACHE INTERNAL "Boost C++ libraries needed to build pypcsim" FORCE )

#####################################################################################
#                                  CPPUNIT                                          #
#####################################################################################

IF( NOT CPPUNIT_LIBRARY )
    MESSAGE( STATUS "Looking for CppUnit ..." )
ENDIF( NOT CPPUNIT_LIBRARY )

SET( CPPUNIT_LIBS cppunit dl )
IF( PLATFORM STREQUAL "windows-x86" )
     SET( CPPUNIT_LIBS cppunit )
ENDIF( PLATFORM STREQUAL "windows-x86" )

FIND_LIBRARY( CPPUNIT_LIBRARY 
              NAMES cppunit
              PATHS ${CMAKE_LIBRARY_PATH} ENV LD_LIBRARY_PATH ENV PATH ENV NO_DEFAULT_PATH
            )
FIND_LIBRARY( CPPUNIT_LIBRARY 
              NAMES cppunit
              PATHS /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64 /usr/local/cppunit/lib /usr/local/cppunit/lib64
            )

IF (CPPUNIT_LIBRARY)
    GET_FILENAME_COMPONENT( CPPUNIT_LIBRARY_PATH ${CPPUNIT_LIBRARY} PATH )
    GET_FILENAME_COMPONENT( CPPUNIT_INCLUDE_PATH "${CPPUNIT_LIBRARY_PATH}/../include" ABSOLUTE )
    MESSAGE( "   CppUnit : ${CPPUNIT_LIBRARY}" )
ELSE(CPPUNIT_LIBRARY)
    MESSAGE( FATAL_ERROR "CppUnit not found. Set CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH environment variables to include non-default installation paths." )
ENDIF(CPPUNIT_LIBRARY)

INCLUDE_DIRECTORIES( "${CPPUNIT_INCLUDE_PATH}" )

SET( CPPUNIT_INCLUDE_PATH ${CPPUNIT_INCLUDE_PATH} CACHE FILEPATH "CppUnit include path" )
SET( CPPUNIT_LIBRARY_PATH ${CPPUNIT_LIBRARY_PATH} CACHE FILEPATH "CppUnit library path" )
SET( CPPUNIT_LIBS ${CPPUNIT_LIBS} CACHE FILEPATH "CppUnit libraries to link" )
MARK_AS_ADVANCED( FORCE CPPUNIT_LIBRARY )

#####################################################################################
#                                  SWIG                                             #
#####################################################################################

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})


#####################################################################################
#                             COMPILER OPTIONS                                      #
#####################################################################################


#
# set member alignment to 4bytes
#
IF( MSVC OR MSVC80 )
     ADD_DEFINITIONS( -Zp4 )   
ENDIF( MSVC OR MSVC80 )

ADD_DEFINITIONS( -fpermissive )

#
# We collect all the dependent libs (BOOST, CPPUNIT)
#
LINK_DIRECTORIES( "${BOOST_LIBRARY_PATH}"
                  "${CPPUNIT_LIBRARY_PATH}" 
                )

#
# Compiler specific stuff
#
IF( CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
ADD_DEFINITIONS( -Wno-deprecated -DMS_WIN64 )
ENDIF( CMAKE_GENERATOR STREQUAL "MinGW Makefiles")

IF( BOOST_TOOLSET_ABBREV STREQUAL "gcc" )
	IF (CMAKE_BUILD_TYPE STREQUAL "Release")
    	ADD_DEFINITIONS( -pipe -Wall -Wnon-virtual-dtor -Wno-long-long -O3 )
    ELSE(CMAKE_BUILD_TYPE STREQUAL "Release")
    	ADD_DEFINITIONS( -g3 -O0 -fno-inline -DDEBUG )
    ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")	
ENDIF( BOOST_TOOLSET_ABBREV STREQUAL "gcc" )


