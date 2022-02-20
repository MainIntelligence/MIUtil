cmake_minimum_required(VERSION 3.5)
project(Util)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
find_package (Threads)

include(GNUInstallDirs)

install(DIRECTORY Src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MIUtil
	FILES_MATCHING PATTERN "*.h")


include(CTest)
enable_testing()

if ( BUILD_TESTING )
  add_executable( testexec test.cpp )
  #target_link_libraries (testexec ${CMAKE_THREAD_LIBS_INIT})
  add_test( NAME test0 COMMAND ./testexec )
endif()
