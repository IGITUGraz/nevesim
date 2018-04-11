if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)


FILE(GLOB TEST_SOURCES RELATIVE "${EVESIM_SOURCE_DIR}/cppunit" "*.cpp" )

INCLUDE_DIRECTORIES( "." )

ADD_EXECUTABLE( evesim_test EXCLUDE_FROM_ALL ${TEST_SOURCES}  )

TARGET_LINK_LIBRARIES( evesim_test
  evesim
  ${CPPUNIT_LIBS}
  ${BOOST_DATETIME}
)

ADD_TEST(UnitTests evesim_test AutoBuild )
