#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestAstroLib.
#
#

set( sources
     TestAstroLib.cpp
   )

set( EpsilonDeltaLibs
     astro file time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )


add_executable( TestAstroLib  ${sources} )
target_link_libraries( TestAstroLib  ${libs} )
