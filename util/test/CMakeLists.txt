#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestUtilLib.
#
#

set( sources
     TestUtilLib.cpp
   )

set( EpsilonDeltaLibs
     util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )


add_executable( TestUtilLib  ${sources} )
target_link_libraries( TestUtilLib  ${libs} )
