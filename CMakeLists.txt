cmake_minimum_required(VERSION 3.5)
project(Util)


include(GNUInstallDirs)

install(DIRECTORY Src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MIUtil
	FILES_MATCHING PATTERN "*.h")


include(CTest)
enable_testing()

if ( BUILD_TESTING )
  add_executable( testexec test.cpp )
  add_test( NAME test0 COMMAND ./testexec )
endif()
