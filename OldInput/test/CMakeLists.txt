#
# CMakeLists.txt
# Copyright (C) 2009 David M. Anderson
#
# CMake script for TestInputLib.
#
#

set( sources
     TestInputLib.cpp
   )

set( EpsilonDeltaLibs
     input graphics2d mmbase time math util
   )
set( libs "" )
foreach( lib ${EpsilonDeltaLibs} )
    include_directories( "${CMAKE_SOURCE_DIR}/${lib}" )
    set( libs  ${libs} EpsilonDelta_${lib} )
endforeach( lib )

if ( useSDL )
    set( libs  ${libs} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} )
endif ( useSDL )
if ( useCWiiD )
    set( libs  ${libs} ${CWiiD_LIBRARIES} )
endif ( useCWiiD )
if ( useWiiYourself )
    set( libs  ${libs} ${WiiYourself_LIBRARIES} )
endif ( useWiiYourself )

add_executable( TestInputLib  ${sources} )
target_link_libraries( TestInputLib  ${libs} )
