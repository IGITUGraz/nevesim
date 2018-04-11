# CMakeList
#
# Created on: Jul 11, 2012
#     Author: dejan

SET(CMAKE_SWIG_FLAGS -c++ -Wall)

FILE(GLOB HEADERS "${PROJECT_SOURCE_DIR}/evesim/*.h" )
FILE(GLOB SOURCES "${PROJECT_SOURCE_DIR}/evesim/*.cpp" )

INCLUDE_DIRECTORIES( ${NUMPY_INCLUDE_DIRS} )

SET_SOURCE_FILES_PROPERTIES(pyevesim.i PROPERTIES CPLUSPLUS ON)

SET(SWIG_MODULE_pyevesim_EXTRA_DEPS "${HEADERS}")

SWIG_ADD_MODULE(pyevesim python pyevesim.i)

SWIG_LINK_LIBRARIES(pyevesim evesim ${PYTHON_LIBRARIES})

SET_TARGET_PROPERTIES(_pyevesim PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing -Wno-uninitialized")

INSTALL(TARGETS _pyevesim
        LIBRARY DESTINATION "lib/python${PYTHON_INTERP_VER}/site-packages"
		CONFIGURATIONS Debug Release RELWITHDEBINFO)
		
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyevesim.py 
             DESTINATION "lib/python${PYTHON_INTERP_VER}/site-packages"
             CONFIGURATIONS Debug Release RELWITHDEBINFO)
		
SUBDIRS( unittests )
