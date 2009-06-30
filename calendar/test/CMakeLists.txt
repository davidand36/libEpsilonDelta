#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestCalendarLib.
#
#

set( sources
     TestCalendarLib.cpp
   )

set( EpsilonDeltaLibs
     calendar astro time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )


add_executable( TestCalendarLib  ${sources} )
target_link_libraries( TestCalendarLib  ${libs} )
