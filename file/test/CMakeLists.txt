#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestFileLib.
#
#

set( sources
     TestFileLib.cpp
   )

set( EpsilonDeltaLibs
     file time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )


add_executable( TestFileLib  ${sources} )
target_link_libraries( TestFileLib  ${libs} )
