#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestMMBaseLib.
#
#

set( sources
     TestMMBaseLib.cpp
   )

set( EpsilonDeltaLibs
     mmbase time util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )

if ( useSDL )
    set( libs  ${libs} ${SDL_LIBRARY} )
endif ( useSDL )


add_executable( TestMMBaseLib  ${sources} )
target_link_libraries( TestMMBaseLib  ${libs} )
