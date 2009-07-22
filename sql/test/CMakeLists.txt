#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestSQLLib.
#
#

set( sources
     TestSQLLib.cpp
   )

set( EpsilonDeltaLibs
     sql file time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )

add_executable( TestSQLLib  ${sources} )
target_link_libraries( TestSQLLib  ${libs} )
